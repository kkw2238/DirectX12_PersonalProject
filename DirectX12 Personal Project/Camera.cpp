#include "Camera.h"

Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::SetViewport(D3D12_VIEWPORT& viewPort)
{
	m_D3DViewport = viewPort;
}

void Camera::SetScissorRect(D3D12_RECT& scissorRect)
{
	m_D3DScissorRect = scissorRect;
}

void Camera::SetViewportScissorRectToCommandList(ID3D12GraphicsCommandList* id3dCommandList)
{
	id3dCommandList->RSSetViewports(1, &m_D3DViewport);
	id3dCommandList->RSSetScissorRects(1, &m_D3DScissorRect);
}

void Camera::SetProjectionMatrix(float FOV, float aspectRatio, float nearZ, float farZ)
{
	m_Projection = XMMatrixPerspectiveFovLH(FOV, aspectRatio, nearZ, farZ);
}

void Camera::GenerationViewMatrix()
{
	m_ViewMatrix = XMMatrixLookAtLH(GetObjPosition(0).GetXMVector(), GetObjLookVector(0).GetXMVector(), GetObjUpVector(0).GetXMVector());
}

void Camera::RegenerationViewMatrix()
{
	Vector3 position = GetObjPosition(0);
	Vector3 right = GetObjRightVector(0);
	Vector3 look = GetObjLookVector(0);
	Vector3 up = GetObjUpVector(0);

	m_ViewMatrix.SetColum(0, Vector4(right.x, right.y, right.z, -Vector3::DotProduct(position, right)));
	m_ViewMatrix.SetColum(0, Vector4(up.x, up.y, up.z, -Vector3::DotProduct(position, up)));
	m_ViewMatrix.SetColum(0, Vector4(look.x, look.y, look.z, -Vector3::DotProduct(position, look)));
}

void Camera::BuildObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT objectCount)
{
	m_CamUploadBuffer.CreateResourceBuffer(id3dDevice, objectCount);
	m_WorldMatrix.resize(objectCount);
	GenerationViewMatrix();
}

void Camera::UpdateInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex)
{
	CB_CAMERA_INFO tmpInfo;

	/* test */
	m_WorldMatrix[0]._43 += 0.01f;
	RegenerationViewMatrix();

	tmpInfo.matView = m_ViewMatrix.Transpose();
	tmpInfo.matProjection = m_Projection.Transpose();
	m_CamUploadBuffer.CopyData(0, tmpInfo);

	id3dGraphicsCommandList->SetGraphicsRootConstantBufferView(rootParameterIndex, m_CamUploadBuffer.GPUVirtualAddress());
}

