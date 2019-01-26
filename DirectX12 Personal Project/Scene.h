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
	/* 테스트용으로 집어넣은 셰이더 오브젝트 */
	GraphicsShaderObjects				m_TestShaderObject;
};

