#pragma once
#include "PipelineObject.h"
#include "Camera.h"

class Scene
{
public:
	Scene() { };
	~Scene() { };

public:
	void BuildObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	void RenderObjects(ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	void SetViewPortScissorRect(D3D12_VIEWPORT& viewport, D3D12_RECT& scissorRect);
	void ProcessKeyboard(float elapsedTime);

protected:
	/* 테스트용으로 집어넣은 셰이더 오브젝트 */
	GraphicsShaderObjects				m_TestShaderObject;
	std::unique_ptr<Camera> m_pCamera;
};

