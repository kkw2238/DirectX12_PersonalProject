#include "GraphicsObjectsShader.h"
#include "CompiledShader.h"

void GraphicsObjectsShader::BuildPipelineObject(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const int numRenderTarget)
{
	std::vector<DXGI_FORMAT> formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	CreateGraphicsRootSignature(id3dDevice, id3dGraphicsCommandList);
	CreateGraphicsPipeline(id3dDevice, id3dGraphicsCommandList, numRenderTarget, formats);

	CreateDescriptorHeap(id3dDevice, id3dGraphicsCommandList, 0, 1, 0);

	BuildGraphicsObjects(id3dDevice, id3dGraphicsCommandList);
}

void GraphicsObjectsShader::RenderGraphicsObj(ID3D12GraphicsCommandList* id3dGraphicsCommandList, Camera* camera)
{
	if (camera) {
		camera->SetViewportScissorRectToCommandList(id3dGraphicsCommandList);
		camera->UpdateInfo(id3dGraphicsCommandList, CAMERAINFO_CB);
	}

	if (m_TestObject.size()) {
		for (size_t s = 0; s < m_TestObject.size(); ++s)
			m_TestObject[s]->Draw(id3dGraphicsCommandList, OBJINFO_CB);
	}
}

void GraphicsObjectsShader::CreateGraphicsRootSignature(ID3D12Device * id3dDevice, ID3D12GraphicsCommandList * id3dGraphicsCommandList)
{
	CD3DX12_DESCRIPTOR_RANGE d3dRootDescriptorRange;
	d3dRootDescriptorRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);

	CD3DX12_ROOT_PARAMETER d3dRootParameter[3];
	d3dRootParameter[CAMERAINFO_CB].InitAsConstantBufferView(0);
	d3dRootParameter[OBJINFO_CB].InitAsConstantBufferView(1);
	d3dRootParameter[TEXTURE_SR].InitAsDescriptorTable(1, &d3dRootDescriptorRange);

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
	Mesh* mesh = new Mesh();
	mesh->SetCubeMesh(id3dDevice, id3dGraphicsCommandList, 5.0f, 5.0f, 5.0f);

	TextureRootInfo tex(TEXMANAGER->LoadTexture(id3dDevice, id3dGraphicsCommandList, std::wstring(L"Textures\\MainBackgound_COLOR.DDS"), L"MAINBG", DDS_ALPHA_MODE_UNKNOWN, false), TEXTURE_SR);

	m_TestObject.resize(1);
	m_TestObject[0] = new GraphicsMeshObject();
	m_TestObject[0]->BuildObjects(id3dDevice, id3dGraphicsCommandList, 3);
	m_TestObject[0]->SetMesh(mesh);
	m_TestObject[0]->SetTexture(tex);
	m_TestObject[0]->CreateSRV(id3dDevice, id3dGraphicsCommandList, m_ID3DDescriptorHeap.Get(), m_DescriptorHeapSRVStart);

	for (UINT i = 0; i < 3; ++i)
		m_TestObject[0]->Move(i, Vector3(10.0f * i, 0.0f, 0.0f));
}

D3D12_INPUT_LAYOUT_DESC GraphicsObjectsShader::GraphicsInputLayoutDesc()
{
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	m_D3DInputElementDescs = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
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
	return COMPILEDSHADER->GetShaderByteCode(L"hlsl\\DefaultShader.hlsl", nullptr, "PS", "ps_5_0");
}
