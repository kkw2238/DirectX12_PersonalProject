#include "DeferredRenderShader.h"

DeferredRenderShader::DeferredRenderShader()
{
}

DeferredRenderShader::~DeferredRenderShader()
{
}

void DeferredRenderShader::BuildPipelineObject(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	std::vector<DXGI_FORMAT> formats = { DXGI_FORMAT_R8G8B8A8_UNORM };
	
	CreateDescriptorHeap(id3dDevice, id3dGraphicsCommandList, 0, 4, 0);

	BuildGraphicsObjects(id3dDevice, id3dGraphicsCommandList);

	CreateSRV(id3dDevice, m_ID3DDescriptorHeap.Get(), m_DescriptorHeapSRVStart, true);
}

void DeferredRenderShader::BuildGraphicsObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	m_TextureInfos.emplace_back(TEXMANAGER->GetTexture(std::wstring(L"RT_COLOR")), TEXTURE_SR);

	m_TextureObject.resize(1);
	m_TextureObject[0] = new GraphicsTextureObject();
	m_TextureObject[0]->SetMesh(nullptr);
}

void DeferredRenderShader::RenderGraphicsObj(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, Camera* camera)
{
	if (camera) {
		camera->SetViewportScissorRectToCommandList(id3dGraphicsCommandList);
	}

	for (int i = 0; i < m_TextureObject.size(); ++i)
		m_TextureObject[i]->Draw(id3dDevice, id3dGraphicsCommandList, 0);
}

void DebugDefferedResource::BuildPipelineObject(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	std::vector<DXGI_FORMAT> formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	CreateDescriptorHeap(id3dDevice, id3dGraphicsCommandList, 0, 4, 0);

	BuildGraphicsObjects(id3dDevice, id3dGraphicsCommandList);

	CreateSRV(id3dDevice, m_ID3DDescriptorHeap.Get(), m_DescriptorHeapSRVStart, true);
}

void DebugDefferedResource::BuildGraphicsObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	m_TextureInfos.emplace_back(TEXMANAGER->GetTexture(std::wstring(L"RT_COLOR")), TEXTURE_SR);
	m_TextureInfos.emplace_back(TEXMANAGER->GetTexture(std::wstring(L"DS_SHADOW")), TEXTURE_SR);

	m_TextureObject.resize(1);

	m_TextureObject[0] = new GraphicsTextureObject();
	m_TextureObject[0]->SetMesh(nullptr);
}

void DebugDefferedResource::RenderGraphicsObj(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, Camera* camera)
{
	for (int i = 0; i < m_TextureObject.size(); ++i)
		m_TextureObject[i]->Draw(id3dDevice, id3dGraphicsCommandList, 0);
}
