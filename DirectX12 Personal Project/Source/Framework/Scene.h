#pragma once
#include "GraphicsObjectsShader.h"
#include "DeferredRenderShader.h"
#include "Camera.h"

class Scene
{
public:
	Scene() { };
	~Scene() { };

public:
	void BuildObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	void RenderObjects(ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	void RenderDeferredObjects(ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	void UpdateScissorRectViewport();
	void ProcessKeyboard(float elapsedTime);

protected:
	/* 테스트용으로 집어넣은 셰이더 오브젝트 */
	GraphicsObjectsShader				m_TestShaderObject;
	DeferredRenderShader				m_DeferredShaderObject;
	std::unique_ptr<Camera> m_Camera;
};

