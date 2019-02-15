#include "GraphicsObjectsShader.h"
#include "CompiledShader.h"
#include "LightManager.h"

GraphicsObjectsShader::GraphicsObjectsShader()
{
}

void GraphicsObjectsShader::BuildPipelineObject(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const int numRenderTarget)
{
	std::vector<DXGI_FORMAT> formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	CreateGraphicsRootSignature(id3dDevice, id3dGraphicsCommandList);
	CreateGraphicsPipeline(id3dDevice, id3dGraphicsCommandList, numRenderTarget, formats);

	CreateDescriptorHeap(id3dDevice, id3dGraphicsCommandList, 0, 2, 0);
	
	BuildGraphicsObjects(id3dDevice, id3dGraphicsCommandList);
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

void GraphicsObjectsShader::CreateGraphicsRootSignature(ID3D12Device * id3dDevice, ID3D12GraphicsCommandList * id3dGraphicsCommandList)
{
	CD3DX12_DESCRIPTOR_RANGE d3dRootDescriptorRange[2];
	d3dRootDescriptorRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);
	d3dRootDescriptorRange[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3);

	CD3DX12_ROOT_PARAMETER d3dRootParameter[5];
	d3dRootParameter[CAMERAINFO_CB].InitAsConstantBufferView(0);
	d3dRootParameter[OBJINFO_CB].InitAsConstantBufferView(1);
	d3dRootParameter[LIGHT_CB].InitAsConstantBufferView(2);
	d3dRootParameter[TEXTURE_SR].InitAsDescriptorTable(1, &d3dRootDescriptorRange[0]);
	d3dRootParameter[TEXTURE_NORM_SR].InitAsDescriptorTable(1, &d3dRootDescriptorRange[1]);

	std::vector<CD3DX12_STATIC_SAMPLER_DESC> d3dSamplerDescs = DESCFACTORY->SamplerDesc(DEFAULTOPT);

	CD3DX12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	d3dRootSignatureDesc.Init(_countof(d3dRootParameter), d3dRootParameter, (UINT)d3dSamplerDescs.size(), d3dSamplerDescs.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> id3dSignatureBlob;
	ComPtr<ID3DBlob> id3dErrorBlob;

	ThrowIfFail(D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, id3dSignatureBlob.GetAddressOf(), id3dErrorBlob.GetAddressOf()));

	if (id3dErrorBlob != nullptr) ::OutputDebugStringA((char*)id3dErrorBlob->GetBufferPointer());

	ThrowIfFail(id3dDevice->CreateRootSignature(NULL, id3dSignatureBlob->GetBufferPointer(), id3dSignatureBlob->GetBufferSize(), IID_PPV_ARGS(m_ID3DRootSignature.GetAddressOf())));
}

void GraphicsObjectsShader::BuildGraphicsObjects(ID3D12Device * id3dDevice, ID3D12GraphicsCommandList * id3dGraphicsCommandList)
{
	Mesh* cubemesh = new Mesh();
	Mesh* planemesh = new Mesh();

	cubemesh->SetCubeMesh(id3dDevice, id3dGraphicsCommandList, 5.0f, 5.0f, 5.0f);
	planemesh->SetPlaneMesh(id3dDevice, id3dGraphicsCommandList, 10.0f, 10.0f);
	
	MESHMANAGER->SetMesh(cubemesh, std::wstring(L"Cube5.0"));
	MESHMANAGER->SetMesh(planemesh, std::wstring(L"Plane10.0"));

	std::vector<TextureRootInfo> textures;
	textures.emplace_back(TEXMANAGER->LoadTexture(id3dDevice, id3dGraphicsCommandList, std::wstring(L"Textures\\tile.DDS"), L"DEFAULT", DDS_ALPHA_MODE_UNKNOWN, false), TEXTURE_SR);
	textures.emplace_back(TEXMANAGER->LoadTexture(id3dDevice, id3dGraphicsCommandList, std::wstring(L"Textures\\tile_nmap.DDS"), L"DEFAULT_NORM", DDS_ALPHA_MODE_UNKNOWN, false), TEXTURE_NORM_SR);
	
	std::vector<TextureRootInfo> tiletextures;
	tiletextures.emplace_back(TEXMANAGER->LoadTexture(id3dDevice, id3dGraphicsCommandList, std::wstring(L"Textures\\tile.DDS"), L"TILE", DDS_ALPHA_MODE_UNKNOWN, false), TEXTURE_SR);
	tiletextures.emplace_back(TEXMANAGER->LoadTexture(id3dDevice, id3dGraphicsCommandList, std::wstring(L"Textures\\tile_nmap.DDS"), L"TILE_NORM", DDS_ALPHA_MODE_UNKNOWN, false), TEXTURE_NORM_SR);

	m_TestObject.resize(2);
	m_TestObject[0] = new GraphicsMeshObject();
	m_TestObject[0]->BuildObjects(id3dDevice, id3dGraphicsCommandList, 1);
	m_TestObject[0]->SetMesh(MESHMANAGER->GetMesh(std::wstring(L"Cube5.0")).get());
	m_TestObject[0]->SetTextures(id3dDevice, textures, m_ID3DDescriptorHeap.Get(), m_DescriptorHeapSRVStart);

	m_TestObject[1] = new GraphicsMeshObject();
	m_TestObject[1]->BuildObjects(id3dDevice, id3dGraphicsCommandList, 1);
	m_TestObject[1]->SetMesh(MESHMANAGER->GetMesh(std::wstring(L"Plane10.0")).get());
	m_TestObject[1]->SetTextures(id3dDevice, tiletextures, m_ID3DDescriptorHeap.Get(), m_DescriptorHeapSRVStart);
}

D3D12_INPUT_LAYOUT_DESC GraphicsObjectsShader::GraphicsInputLayoutDesc()
{
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	m_D3DInputElementDescs = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	d3dInputLayoutDesc = { m_D3DInputElementDescs.data(), (UINT)m_D3DInputElementDescs.size() };

	return d3dInputLayoutDesc;
}

D3D12_SHADER_BYTECODE GraphicsObjectsShader::VS()
{
	return COMPILEDSHADER->GetShaderByteCode(L"hlsl\\DefaultShader.hlsl", nullptr, "VS", "vs_5_0");
}

D3D12_SHADER_BYTECODE GraphicsObjectsShader::PS()
{
	return COMPILEDSHADER->GetShaderByteCode(L"hlsl\\DefaultShader.hlsl", LIGHT_MANAGER->GetShaderDefined().data(), "PS", "ps_5_0");
}

GraphicsMeshObject* GraphicsObjectsShader::Objects(UINT index)
{
	if (index < m_TestObject.size())
		return m_TestObject[index];
	else
		return nullptr;
}
