#include "Scene.h"

void Scene::BuildObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	m_TestShaderObject.BuildPipelineObject(id3dDevice, id3dGraphicsCommandList, 1);
	m_DeferredShaderObject.BuildPipelineObject(id3dDevice, id3dGraphicsCommandList, 1);

	m_Camera = std::make_unique<Camera>();
	m_Camera->BuildObjects(id3dDevice, id3dGraphicsCommandList, 1);
	m_Camera->SetProjectionMatrix(60.0f, (float)CLIENT_WIDTH / (float)CLIENT_HEIGHT, 1.0f, 1000.0f);
}

void Scene::RenderObjects(ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	m_TestShaderObject.ExecutePipeline(id3dGraphicsCommandList, m_Camera.get());
}

void Scene::RenderDeferredObjects(ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	m_DeferredShaderObject.ExecutePipeline(id3dGraphicsCommandList, m_Camera.get());
}

void Scene::UpdateScissorRectViewport()
{
	if (m_Camera != nullptr) {
		m_Camera->CreateScissorRect();
		m_Camera->CreateViewport();
	}
}

void Scene::ProcessKeyboard(float elapsedTime)
{
	DWORD keyDown = 0;

	if (D3DUtil::IsKeyDown('W')) keyDown |= DIR_UP;
	if (D3DUtil::IsKeyDown('S')) keyDown |= DIR_DOWN;
	if (D3DUtil::IsKeyDown('D')) keyDown |= DIR_RIGHT;
	if (D3DUtil::IsKeyDown('A')) keyDown |= DIR_LEFT;

	m_Camera.get()->Move(0, keyDown, elapsedTime);
}
