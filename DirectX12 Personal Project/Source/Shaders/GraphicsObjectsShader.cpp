#include "GraphicsObjectsShader.h"
#include "CompiledShader.h"
#include "LightManager.h"

GraphicsObjectsShader::GraphicsObjectsShader()
{
}

void GraphicsObjectsShader::BuildPipelineObject(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	std::vector<DXGI_FORMAT> formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	CreateDescriptorHeap(id3dDevice, id3dGraphicsCommandList, 0, 5, 0);
	
	BuildGraphicsObjects(id3dDevice, id3dGraphicsCommandList);

	CreateSRV(id3dDevice, m_ID3DDescriptorHeap.Get(), m_DescriptorHeapSRVStart, true);
}

void GraphicsObjectsShader::RenderGraphicsObj(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, Camera* camera)
{
	if (camera) {
		camera->SetViewportScissorRectToCommandList(id3dGraphicsCommandList);
		camera->UpdateInfo(id3dGraphicsCommandList, CAMERAINFO_CB);
	}

	LIGHT_MANAGER->UpdateLightInfo(id3dGraphicsCommandList, LIGHT_CB);

	if (m_TestObject.size()) {
		for (size_t s = 0; s < m_TestObject.size(); ++s)
			m_TestObject[s]->Draw(id3dDevice, id3dGraphicsCommandList, OBJINFO_CB);
	}
}

void GraphicsObjectsShader::BuildGraphicsObjects(ID3D12Device * id3dDevice, ID3D12GraphicsCommandList * id3dGraphicsCommandList)
{
	Mesh* cubemesh = new Mesh();
	Mesh* planemesh = new Mesh();

	cubemesh->SetCubeMesh(id3dDevice, id3dGraphicsCommandList, 5.0f, 5.0f, 5.0f, 0);
	planemesh->SetPlaneMesh(id3dDevice, id3dGraphicsCommandList, 10.0f, 10.0f, 1);
	
	MESHMANAGER->SetMesh(cubemesh, std::wstring(L"Cube5.0"));
	MESHMANAGER->SetMesh(planemesh, std::wstring(L"Plane10.0"));

	m_TextureInfos.emplace_back(TEXMANAGER->LoadTexture(id3dDevice, id3dGraphicsCommandList, std::wstring(L"Textures\\tile.DDS"), L"DEFAULT", DDS_ALPHA_MODE_UNKNOWN, false), TEXTURE_SR);
	m_TextureInfos.emplace_back(TEXMANAGER->LoadTexture(id3dDevice, id3dGraphicsCommandList, std::wstring(L"Textures\\tile_nmap.DDS"), L"DEFAULT_NORM", DDS_ALPHA_MODE_UNKNOWN, false), TEXTURE_SR);

	m_TextureInfos.emplace_back(TEXMANAGER->LoadTexture(id3dDevice, id3dGraphicsCommandList, std::wstring(L"Textures\\tile.DDS"), L"TILE", DDS_ALPHA_MODE_UNKNOWN, false), TEXTURE_SR);
	m_TextureInfos.emplace_back(TEXMANAGER->LoadTexture(id3dDevice, id3dGraphicsCommandList, std::wstring(L"Textures\\tile_nmap.DDS"), L"TILE_NORM", DDS_ALPHA_MODE_UNKNOWN, false), TEXTURE_SR);

	m_TextureInfos.emplace_back(TEXMANAGER->GetTexture(L"DS_SHADOW", TEXTURE_SR);

	m_TestObject.resize(2);
	m_TestObject[0] = new GraphicsMeshObject();
	m_TestObject[0]->BuildObjects(id3dDevice, id3dGraphicsCommandList, 1);

	m_TestObject[1] = new GraphicsMeshObject();
	m_TestObject[1]->BuildObjects(id3dDevice, id3dGraphicsCommandList, 1);
	m_TestObject[1]->SetMesh(MESHMANAGER->GetMesh(std::wstring(L"Plane10.0")).get());
	m_TestObject[1]->SetObjScale(0, Vector3(30.0f, 1.0f, 30.0f));
}

GraphicsMeshObject* GraphicsObjectsShader::Objects(UINT index)
{
	if (index < m_TestObject.size())
		return m_TestObject[index];
	else
		return nullptr;
}
