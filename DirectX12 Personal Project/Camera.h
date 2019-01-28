#pragma once
#include "MathUtil.h"
#include "Objects.h"

class Camera : public Objects
{
public:
	Camera();
	~Camera();

public:
	void SetViewport(D3D12_VIEWPORT& viewPort);
	void SetScissorRect(D3D12_RECT& scissorRect);
	void SetViewportScissorRectToCommandList(ComPtr<ID3D12GraphicsCommandList> id3dCommandList);
	void SetViewMatrix(float FOV, float aspectRatio, float nearZ, float farZ);

protected:
	Matrix4x4		m_ViewMatrix;

	D3D12_VIEWPORT	m_D3DViewport;
	D3D12_RECT		m_D3DScissorRect;
};

