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

void Camera::SetViewportScissorRectToCommandList(ComPtr<ID3D12GraphicsCommandList> id3dCommandList)
{
	id3dCommandList->RSSetViewports(1, &m_D3DViewport);
	id3dCommandList->RSSetScissorRects(1, &m_D3DScissorRect);
}

void Camera::SetViewMatrix(float FOV, float aspectRatio, float nearZ, float farZ)
{
	m_ViewMatrix = XMMatrixPerspectiveFovLH(FOV, aspectRatio, nearZ, farZ);
}

