#include "DeferredRenderShader.h"

DeferredRenderShader::DeferredRenderShader()
{
}


DeferredRenderShader::~DeferredRenderShader()
{
}

void DeferredRenderShader::BuildPipelineObject(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const int numRenderTarget)
{
	std::vector<DXGI_FORMAT> formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	CreateGraphicsRootSignature(id3dDevice, id3dGraphicsCommandList);
	CreateGraphicsPipeline(id3dDevice, id3dGraphicsCommandList, numRenderTarget, formats);
	CreateDescriptorHeap(id3dDevice, id3dGraphicsCommandList, 0, 1, 0);

	BuildGraphicsObjects(id3dDevice, id3dGraphicsCommandList);
}

void DeferredRenderShader::BuildGraphicsObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	TextureRootInfo tex(TEXMANAGER->GetTexture(std::wstring(L"RT_COLOR")), TEXTURE_SR);

	m_TextureObject.resize(1);
	m_TextureObject[0] = new GraphicsTextureObject();
	m_TextureObject[0]->SetTexture(id3dDevice, tex, m_ID3DDescriptorHeap.Get(), 0);
	m_TextureObject[0]->SetMesh(nullptr);
}

void DeferredRenderShader::CreateGraphicsRootSignature(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	CD3DX12_DESCRIPTOR_RANGE d3dRootDescriptorRange;
	d3dRootDescriptorRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);

	CD3DX12_ROOT_PARAMETER d3dRootParameter[1];
	d3dRootParameter[TEXTURE_SR].InitAsDescriptorTable(1, &d3dRootDescriptorRange);

	std::vector<CD3DX12_STATIC_SAMPLER_DESC> d3dSamplerDescs = DESCFACTORY->SamplerDesc(DEFAULTOPT);

	CD3DX12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	d3dRootSignatureDesc.Init(_countof(d3dRootParameter), d3dRootParameter, d3dSamplerDescs.size(), d3dSamplerDescs.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> id3dSignatureBlob;
	ComPtr<ID3DBlob> id3dErrorBlob;

	ThrowIfFail(D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, id3dSignatureBlob.GetAddressOf(), id3dErrorBlob.GetAddressOf()));

	if (id3dErrorBlob != nullptr) ::OutputDebugStringA((char*)id3dErrorBlob->GetBufferPointer());

	ThrowIfFail(id3dDevice->CreateRootSignature(NULL, id3dSignatureBlob->GetBufferPointer(), id3dSignatureBlob->GetBufferSize(), IID_PPV_ARGS(m_ID3DRootSignature.GetAddressOf())));
}

void DeferredRenderShader::RenderGraphicsObj(ID3D12GraphicsCommandList* id3dGraphicsCommandList, Camera* camera)
{
	if (camera) {
		camera->SetViewportScissorRectToCommandList(id3dGraphicsCommandList);
	}

	for (int i = 0; i < m_TextureObject.size(); ++i)
		m_TextureObject[i]->Draw(id3dGraphicsCommandList, 0);
}

D3D12_SHADER_BYTECODE DeferredRenderShader::VS()
{
	return COMPILEDSHADER->GetShaderByteCode(L"hlsl\\DefaultShader.hlsl", nullptr, "VSTextureFullScreen", "vs_5_0");
}

D3D12_SHADER_BYTECODE DeferredRenderShader::PS()
{
	return COMPILEDSHADER->GetShaderByteCode(L"hlsl\\DefaultShader.hlsl", nullptr, "PSTextureFullScreen", "ps_5_0");
}
