#pragma once
#include "PipelineObject.h"
#include "Camera.h"

class Scene
{
public:
	Scene() { };
	~Scene() { };

public:
	void BuildObjects(ComPtr<ID3D12Device> id3dDevice, ComPtr<ID3D12GraphicsCommandList> id3dGraphicsCommandList);
	void RenderObjects(ComPtr<ID3D12GraphicsCommandList> id3dGraphicsCommandList);
	void SetViewPortScissorRect(D3D12_VIEWPORT& viewport, D3D12_RECT& scissorRect);

protected:
	/* �׽�Ʈ������ ������� ���̴� ������Ʈ */
	GraphicsShaderObjects				m_TestShaderObject;
	std::unique_ptr<Camera> m_pCamera;
};

