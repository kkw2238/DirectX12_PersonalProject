#pragma once
#include "Objects.h"

class Camera : public Objects
{
public:
	Camera();
	~Camera();

public:
	void SetViewport(D3D12_VIEWPORT& viewPort);
	void SetScissorRect(D3D12_RECT& scissorRect);
	void SetViewportScissorRectToCommandList(ID3D12GraphicsCommandList* id3dCommandList);
	void SetProjectionMatrix(float FOV, float aspectRatio, float nearZ, float farZ);
	
	void GenerationViewMatrix();
	void RegenerationViewMatrix();

	UINT BuildObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT objectCount);
	void UpdateInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex);

protected:
	Matrix4x4		m_ViewMatrix;
	Matrix4x4		m_Projection;

	D3D12_VIEWPORT	m_D3DViewport;
	D3D12_RECT		m_D3DScissorRect;

	ObjectResourceBuffer<CB_CAMERA_INFO> m_CamUploadBuffer;
};

