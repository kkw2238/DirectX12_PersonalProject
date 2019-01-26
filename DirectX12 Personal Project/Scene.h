#pragma once
#include "PipelineObject.h"

class Scene
{
public:
	Scene() { };
	~Scene() { };

public:
	void BuildObjects(ComPtr<ID3D12Device> id3dDevice, ComPtr<ID3D12GraphicsCommandList> id3dGraphicsCommandList);
	void RenderObjects(ComPtr<ID3D12GraphicsCommandList> id3dGraphicsCommandList);

protected:
	/* �׽�Ʈ������ ������� ���̴� ������Ʈ */
	GraphicsShaderObjects				m_TestShaderObject;
};

