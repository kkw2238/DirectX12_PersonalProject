#include "Objects.h"

Objects::Objects()
{
}

Objects::~Objects()
{
}

void Objects::SetObjRightVector(UINT index, const Vector3& vector)
{
	m_WorldMatrix[index]._xyz1 = vector;
}

void Objects::SetObjUpVector(UINT index, const Vector3& vector)
{
	m_WorldMatrix[index]._xyz2 = vector;
}

void Objects::SetObjLookVector(UINT index, const Vector3& vector)
{
	m_WorldMatrix[index]._xyz3 = vector;
}

void Objects::SetObjPosition(UINT index, const Vector3& vector)
{
	m_WorldMatrix[index]._xyz4 = vector;
}

void Objects::SetObjRotateAngle(UINT index, const Vector3& angles)
{
	m_RotateAngle[index] = angles;
	m_RotateMatrix[index] = XMMatrixRotationRollPitchYawFromVector(m_RotateAngle[index].ToRadian().GetXMVector());
}

void Objects::SetObjLookAt(UINT index, const Vector3& target)
{
	Vector3 direction = ((GetObjPosition(index) - target) * -1.0f).Normalize();
	Vector3 right = (Vector3(0.0f, 1.0f, 0.0f) * direction).Normalize();
	Vector3 up = (direction * right).Normalize();

	SetObjLookVector(index, direction);
	SetObjUpVector(index, up);
	SetObjRightVector(index, right);
}

void Objects::SetObjLookDirection(UINT index, const Vector3& direction)
{
	Vector3 right = (Vector3(0.0f, 1.0f, 0.0f) * static_cast<Vector3>(direction).Normalize()).Normalize();
	Vector3 up = (static_cast<Vector3>(direction) * right).Normalize();

	SetObjLookVector(index, static_cast<Vector3>(direction).Normalize());
	SetObjUpVector(index, up);
	SetObjRightVector(index, right);
}

Vector3 Objects::GetObjRightVector(UINT index)
{
	return m_WorldMatrix[index]._xyz1;
}

Vector3 Objects::GetObjUpVector(UINT index)
{
	return m_WorldMatrix[index]._xyz2;
}

Vector3 Objects::GetObjLookVector(UINT index)
{
	return m_WorldMatrix[index]._xyz3;
}

Vector3 Objects::GetObjPosition(UINT index)
{
	return m_WorldMatrix[index]._xyz4;
}

Vector3 Objects::GetObjRotateAngle(UINT index)
{
	return m_RotateAngle[index];
}

void Objects::Move(UINT index, DWORD direction, float elapsedtime)
{
	Vector3 moveDirection;
	Vector3 moveDistance;
	
	if (direction & DIR_UP)		moveDirection.y += 1.0f;
	if (direction & DIR_DOWN)	moveDirection.y -= 1.0f;
	if (direction & DIR_RIGHT)	moveDirection.x += 1.0f;
	if (direction & DIR_LEFT)	moveDirection.x -= 1.0f;
	if (direction & DIR_FORWARD)moveDirection.z += 1.0f;
	if (direction & DIR_BACK)	moveDirection.z -= 1.0f;

	moveDistance = GetObjPosition(index) + moveDirection.Normalize() * m_MoveSpeed * elapsedtime;

	SetObjPosition(index, moveDistance);
}

void Objects::Move(UINT index, const Vector3& distance)
{
	Vector3 newPosition;
	newPosition = GetObjPosition(index) + distance;

	SetObjPosition(index, newPosition);
}

void Objects::Rotate(UINT index, const Vector3& angles)
{
	m_RotateAngle[index] += angles;
	m_RotateMatrix[index] = XMMatrixRotationRollPitchYawFromVector(m_RotateAngle[index].ToRadian().GetXMVector());
}

void GraphicsTextureObject::SetMesh(Mesh* newMesh)
{
	m_pMesh = newMesh;
}

void GraphicsTextureObject::Draw(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex)
{
	UpdateInfo(id3dGraphicsCommandList, rootParameterIndex);
	
	if (!m_Is2D) {
		if (m_pMesh != nullptr)
			m_pMesh->DrawMeshes(id3dGraphicsCommandList, m_ObjectCount);
		else if (MESHMANAGER->GetLatelyMesh() != nullptr) {
			MESHMANAGER->GetLatelyMesh()->DrawMeshes(id3dGraphicsCommandList, m_ObjectCount);
		}
	}
	else
		id3dGraphicsCommandList->DrawInstanced(6, 1, 0, 0);
}

/////////////////////////////////////////

void GraphicsMeshObject::CreateCBV(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, ID3D12DescriptorHeap* id3dDescriptorHeap, UINT offset)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvViewDesc;
	cbvViewDesc.SizeInBytes = m_ObjUploadBuffer.DataSize();
	cbvViewDesc.BufferLocation = m_ObjUploadBuffer.GPUVirtualAddress();

	DESCFACTORY->CraeteCPUGPUDescriptorHandle(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, id3dDescriptorHeap, m_CBVCPUDescriptorHandle, m_CBVGPUDescriptorHandle, offset);
	
	for (UINT i = 0; i < m_ObjectCount; ++i) {
		CD3DX12_CPU_DESCRIPTOR_HANDLE tmpCPUDescHandle = m_CBVCPUDescriptorHandle;
		id3dDevice->CreateConstantBufferView(&cbvViewDesc, tmpCPUDescHandle.Offset(i));
	}
}


UINT GraphicsMeshObject::BuildObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT objectCount)
{
	m_ObjUploadBuffer.CreateResourceBuffer(id3dDevice, objectCount);
	m_WorldMatrix.resize(objectCount);
	m_RotateMatrix.resize(objectCount);
	m_RotateAngle.resize(objectCount);

	m_ObjectCount = objectCount;

	return objectCount;
}

void GraphicsMeshObject::UpdateInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex)
{
	for (UINT i = 0; i < m_ObjectCount; ++i) {
		CB_OBJ_INFO tmpData;
		Matrix4x4 scaleMatrix = XMMatrixScalingFromVector(m_ScaleSize.GetXMVector());
		tmpData.matWorld = (scaleMatrix * m_WorldMatrix[i] * m_RotateMatrix[i]).Transpose();
		m_ObjUploadBuffer.CopyData(i, tmpData);
	}

	id3dGraphicsCommandList->SetGraphicsRootConstantBufferView(rootParameterIndex, m_ObjUploadBuffer.GPUVirtualAddress());
}

void GraphicsMeshObject::SetObjScale(UINT index, const Vector3& vector)
{
	m_ScaleSize = vector;
}

Vector3 GraphicsMeshObject::GetObjScale(UINT index)
{
	return m_ScaleSize;
}
