#include "Camera.h"

Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::CreateViewport()
{
	m_D3DViewport.TopLeftX = 0;
	m_D3DViewport.TopLeftY = 0;
	m_D3DViewport.Width = static_cast<float>(CLIENT_WIDTH);
	m_D3DViewport.Height = static_cast<float>(CLIENT_HEIGHT);
	m_D3DViewport.MinDepth = 0.0f;
	m_D3DViewport.MaxDepth = 1.0f;
}

void Camera::CreateScissorRect()
{
	m_D3DScissorRect = { 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT };
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
	m_Projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(FOV), aspectRatio, nearZ, farZ);
}

void Camera::SetShadowMatrix(Matrix4x4 mat)
{
	m_ShadowMatrix = mat;
}

Matrix4x4 Camera::GetShadowMatrix()
{
	return m_ShadowMatrix;
}

void Camera::GenerationViewMatrix()
{
	m_ViewMatrix = XMMatrixLookAtLH(GetObjPosition(0).GetXMVector(), GetObjLookVector(0).GetXMVector(), GetObjUpVector(0).GetXMVector());
}

void Camera::RegenerationViewMatrix()
{
	Vector3 position = GetObjPosition(0);
	Vector3 right = GetObjRightVector(0).Normalize();
	Vector3 look = GetObjLookVector(0).Normalize();
	Vector3 up = (look * right).Normalize();

	m_ViewMatrix = XMMatrixLookToLH(position.GetXMVector(), look.GetXMVector(), up.GetXMVector());
}

UINT Camera::BuildObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT objectCount)
{
	m_MoveSpeed = 20.0f;

	m_CamUploadBuffer.CreateResourceBuffer(id3dDevice, objectCount);
	m_WorldMatrix.resize(objectCount);
	m_RotateAngle.resize(objectCount);
	m_RotateMatrix.resize(objectCount);

	Rotate(0, Vector3(-45.0f, 0.0f, 0.0f));
	GenerationViewMatrix();

	CreateScissorRect();
	CreateViewport();

	return objectCount;
}

void Camera::UpdateInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex)
{
	CB_CAMERA_INFO tmpInfo;

	RegenerationViewMatrix();
	
	tmpInfo.matView = (m_ViewMatrix * m_RotateMatrix[0]).Transpose();
	tmpInfo.matProjection = m_Projection.Transpose();
	tmpInfo.shadowMat = m_ShadowMatrix.Transpose();
	tmpInfo.position = GetObjPosition(0);

	m_CamUploadBuffer.CopyData(0, tmpInfo);

	id3dGraphicsCommandList->SetGraphicsRootConstantBufferView(rootParameterIndex, m_CamUploadBuffer.GPUVirtualAddress());
}
