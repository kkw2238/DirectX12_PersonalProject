#include "Scene.h"

void Scene::BuildObjects(ComPtr<ID3D12Device> id3dDevice, ComPtr<ID3D12GraphicsCommandList> id3dGraphicsCommandList)
{
	m_TestShaderObject.CreateGraphicsRootSignature(id3dDevice, id3dGraphicsCommandList);
	m_TestShaderObject.CreateGraphicsPipeline(id3dDevice, id3dGraphicsCommandList, 1);
	m_TestShaderObject.BuildGraphicsObjects(id3dDevice, id3dGraphicsCommandList);

	m_pCamera = std::make_unique<Camera>();
}

void Scene::RenderObjects(ComPtr<ID3D12GraphicsCommandList> id3dGraphicsCommandList)
{
	m_TestShaderObject.ExecutePipeline(id3dGraphicsCommandList, m_pCamera.get());
}

void Scene::SetViewPortScissorRect(D3D12_VIEWPORT& viewport, D3D12_RECT& scissorRect)
{
	m_pCamera->SetScissorRect(scissorRect);
	m_pCamera->SetViewport(viewport);
}
