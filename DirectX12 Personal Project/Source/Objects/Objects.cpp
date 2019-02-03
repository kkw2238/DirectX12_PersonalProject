#include "Objects.h"

Objects::Objects()
{
}

Objects::~Objects()
{
}

void Objects::SetObjRightVector(UINT index, Vector3& vector)
{
	m_WorldMatrix[index]._xyz1 = vector;
}

void Objects::SetObjUpVector(UINT index, Vector3& vector)
{
	m_WorldMatrix[index]._xyz2 = vector;
}

void Objects::SetObjLookVector(UINT index, Vector3& vector)
{
	m_WorldMatrix[index]._xyz3 = vector;
}

void Objects::SetObjPosition(UINT index, Vector3& vector)
{
	m_WorldMatrix[index]._xyz4 = vector;
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

void Objects::Move(UINT index, DWORD direction, float elapsedtime)
{
	Vector3 moveDirection;
	Vector3 moveDistance;
	
	if (direction & DIR_UP)		moveDirection.z += 1.0f;
	if (direction & DIR_DOWN)	moveDirection.z -= 1.0f;
	if (direction & DIR_RIGHT)	moveDirection.x += 1.0f;
	if (direction & DIR_LEFT)	moveDirection.x -= 1.0f;

	moveDistance = GetObjPosition(index) + moveDirection.Normalize() * m_MoveSpeed * elapsedtime;

	SetObjPosition(index, moveDistance);
}

void Objects::Move(UINT index, const Vector3& distance)
{
	Vector3 newPosition;
	newPosition = GetObjPosition(index) + distance;

	SetObjPosition(index, newPosition);
}

void GraphicsTextureObject::SetMesh(Mesh* newMesh)
{
	m_pMesh = newMesh;
}

void GraphicsTextureObject::SetTextures(std::vector<TextureRootInfo>& newTextures)
{
	m_Textures = std::move(newTextures);
}

void GraphicsTextureObject::SetTexture(TextureRootInfo& newTexture)
{
	m_Textures.resize(1);
	m_Textures[0] = newTexture;
}

void GraphicsTextureObject::AddTexture(TextureRootInfo& newTexture)
{
	m_Textures.push_back(newTexture);
}

void GraphicsTextureObject::CreateSRV(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, ID3D12DescriptorHeap* id3dDescriptorHeap, UINT offset)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE SRVCPUDescriptorHandle;
	CD3DX12_GPU_DESCRIPTOR_HANDLE SRVGPUDescriptorHandle;
	DESCFACTORY->CraeteCPUGPUDescriptorHandle(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, id3dDescriptorHeap, SRVCPUDescriptorHandle, SRVGPUDescriptorHandle, offset);

	for (UINT i = 0; i < m_Textures.size(); ++i) {
		id3dDevice->CreateShaderResourceView(m_Textures[i].Resource(), &m_Textures[i].SRVDesc(), SRVCPUDescriptorHandle);

		m_Textures[i].SetDescriptorHandle(SRVCPUDescriptorHandle, SRVGPUDescriptorHandle);

		SRVCPUDescriptorHandle.Offset(DESCFACTORY->DescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
		SRVGPUDescriptorHandle.Offset(DESCFACTORY->DescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	}
}

void GraphicsTextureObject::Draw(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex)
{
	UpdateInfo(id3dGraphicsCommandList, rootParameterIndex);
	UpdateTextureInfo(id3dGraphicsCommandList);

	if (m_pMesh != nullptr)
		m_pMesh->DrawMesh(id3dGraphicsCommandList, m_ObjectCount);
	else
		id3dGraphicsCommandList->DrawInstanced(6, 1, 0, 0);
}

void GraphicsTextureObject::UpdateTextureInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	for (UINT i = 0; i < m_Textures.size(); ++i) {
		m_Textures[i].UpdateInfo(id3dGraphicsCommandList);
	}
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
	m_ObjectCount = objectCount;

	return objectCount;
}

void GraphicsMeshObject::UpdateInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex)
{
	for (UINT i = 0; i < m_ObjectCount; ++i) {
		CB_OBJ_INFO tmpData;
		tmpData.matWorld = m_WorldMatrix[i].Transpose();
		m_ObjUploadBuffer.CopyData(i, tmpData);
	}

	id3dGraphicsCommandList->SetGraphicsRootConstantBufferView(rootParameterIndex, m_ObjUploadBuffer.GPUVirtualAddress());
}
