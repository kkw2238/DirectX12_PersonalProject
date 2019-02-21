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

	void CreateShadowMap(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	void RenderObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	void RenderDeferredObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);

	void UpdateScissorRectViewport();
	void ProcessKeyboard(float elapsedTime);
	void ProcessMouseWheel(SHORT wheelRotatedir, float elapsedTime);
	void ProcesMouseMove(int xpos, int ypos, float elapsedTime);
	void ProcesMouseDown(HWND hWnd, UINT msg, int xpos, int ypos, float elapsedTime);
	void ProcesMouseUp(HWND hWnd, UINT msg, int xpos, int ypos, float elapsedTime);

protected:
	POINT								m_MousePos;
	/* 테스트용으로 집어넣은 셰이더 오브젝트 */
	GraphicsObjectsShader				m_TestShaderObject;
	DeferredRenderShader				m_DeferredShaderObject;

	DebugDefferedResource				m_DebugResourceObject;

	bool m_EnableDebug = false;
	bool m_IsCapture = false;

	float m_MousSeensitivity = 20.0f;

	std::unique_ptr<Camera> m_Camera;
};

