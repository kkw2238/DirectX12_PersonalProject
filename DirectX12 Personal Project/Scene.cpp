#include "Scene.h"

void Scene::BuildObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	m_TestShaderObject.CreateGraphicsRootSignature(id3dDevice, id3dGraphicsCommandList);
	m_TestShaderObject.CreateGraphicsPipeline(id3dDevice, id3dGraphicsCommandList, 1);
	m_TestShaderObject.BuildGraphicsObjects(id3dDevice, id3dGraphicsCommandList);

	m_pCamera = std::make_unique<Camera>();
	m_pCamera->BuildObjects(id3dDevice, id3dGraphicsCommandList, 1);
}

void Scene::RenderObjects(ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	m_TestShaderObject.ExecutePipeline(id3dGraphicsCommandList, m_pCamera.get());
}

void Scene::SetViewPortScissorRect(D3D12_VIEWPORT& viewport, D3D12_RECT& scissorRect)
{
	m_pCamera->SetScissorRect(scissorRect);
	m_pCamera->SetViewport(viewport);
	m_pCamera->SetProjectionMatrix(60.0f, (float)CLIENT_WIDTH / (float)CLIENT_HEIGHT, 1.0f, 1000.0f);
}
