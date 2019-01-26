#include "Scene.h"

void Scene::BuildObjects(ComPtr<ID3D12Device> id3dDevice, ComPtr<ID3D12GraphicsCommandList> id3dGraphicsCommandList)
{
	m_TestShaderObject.CreateGraphicsRootSignature(id3dDevice, id3dGraphicsCommandList);
	m_TestShaderObject.CreateGraphicsPipeline(id3dDevice, id3dGraphicsCommandList, 1);
}

void Scene::RenderObjects(ComPtr<ID3D12GraphicsCommandList> id3dGraphicsCommandList)
{
	m_TestShaderObject.ExecutePipeline(id3dGraphicsCommandList);
}
