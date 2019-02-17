#include "PipelineStateManager.h"
#include "D3DDescriptorFactory.h"
#include "CompiledShader.h"

PipelineStateManager::PipelineStateManager()
{
}

PipelineStateManager::~PipelineStateManager()
{
}

PipelineStateManager* PipelineStateManager::Instance()
{
	static PipelineStateManager pipelineManager;
	return &pipelineManager;
}

void PipelineStateManager::CreatePipelineStates(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	CreateRootSignatures(id3dDevice, id3dGraphicsCommandList);
	CreateGraphicsPipelineStates(id3dDevice, id3dGraphicsCommandList);
}

void PipelineStateManager::CreateGraphicsPipelineStates(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	for (UINT i = 0; i < m_PipelineNames.size(); ++i) {
		std::vector<D3D12_INPUT_ELEMENT_DESC> elements = GraphicsInputElementDesc(i);
		std::vector<DXGI_FORMAT> formats = GraphicsRenderTargetFormat(i);

		D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dGraphicsPipelineDesc;
		::ZeroMemory(&d3dGraphicsPipelineDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

		d3dGraphicsPipelineDesc.BlendState = GraphicsBlendDesc(i);
		d3dGraphicsPipelineDesc.DepthStencilState = GraphicsDepthStencilDesc(i);
		d3dGraphicsPipelineDesc.InputLayout = D3D12_INPUT_LAYOUT_DESC{ elements.data(), static_cast<UINT>(elements.size()) };
		d3dGraphicsPipelineDesc.RasterizerState = GraphicsRasterRizerDesc(i);
		d3dGraphicsPipelineDesc.pRootSignature = GraphicsRootSignature(m_PipelineNames[i]);
		d3dGraphicsPipelineDesc.VS = VS(i);
		d3dGraphicsPipelineDesc.PS = PS(i);
		d3dGraphicsPipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		d3dGraphicsPipelineDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		for (size_t j = 0; j < formats.size(); ++j)
			d3dGraphicsPipelineDesc.RTVFormats[j] = formats[j];
		d3dGraphicsPipelineDesc.NumRenderTargets = static_cast<UINT>(formats.size());
		d3dGraphicsPipelineDesc.SampleMask = UINT_MAX;
		d3dGraphicsPipelineDesc.SampleDesc.Count = 1;
		d3dGraphicsPipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		ThrowIfFail(id3dDevice->CreateGraphicsPipelineState(&d3dGraphicsPipelineDesc, IID_PPV_ARGS(m_ID3DPipelineStates[m_PipelineNames[i]].GetAddressOf())));
	}
}

void PipelineStateManager::CreateComputePipelineStates(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
}

void PipelineStateManager::CreateRootSignatures(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	std::vector<CD3DX12_STATIC_SAMPLER_DESC> d3dSamplerDescs = DESCFACTORY->SamplerDesc(DEFAULTOPT);

	for(size_t i = 0; i < m_RootSignatureNames.size(); ++i)
	{
		std::vector<CD3DX12_DESCRIPTOR_RANGE> d3dRootDescriptorRange;
		std::vector<CD3DX12_ROOT_PARAMETER> d3dRootParameter;
		CD3DX12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;

		if (i == RENDER_OBJ_SIGNATURE) {
			d3dRootDescriptorRange.resize(1);
			d3dRootDescriptorRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 2);

			d3dRootParameter.resize(4);
			d3dRootParameter[0].InitAsConstantBufferView(0);
			d3dRootParameter[1].InitAsConstantBufferView(1);
			d3dRootParameter[2].InitAsConstantBufferView(2);
			d3dRootParameter[3].InitAsDescriptorTable(1, &d3dRootDescriptorRange[0]);
		}

		else if (i == RENDER_DEFERRED_SIGNATURE) {
			d3dRootDescriptorRange.resize(1);
			d3dRootDescriptorRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);

			d3dRootParameter.resize(1);
			d3dRootParameter[0].InitAsDescriptorTable(1, &d3dRootDescriptorRange[0]);
		}

		d3dRootSignatureDesc.Init(d3dRootParameter.size(), d3dRootParameter.data(), (UINT)d3dSamplerDescs.size(), d3dSamplerDescs.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		
		ComPtr<ID3DBlob> id3dSignatureBlob;
		ComPtr<ID3DBlob> id3dErrorBlob;

		ThrowIfFail(D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, id3dSignatureBlob.GetAddressOf(), id3dErrorBlob.GetAddressOf()));

		if (id3dErrorBlob != nullptr) ::OutputDebugStringA((char*)id3dErrorBlob->GetBufferPointer());

		ThrowIfFail(id3dDevice->CreateRootSignature(NULL, id3dSignatureBlob->GetBufferPointer(), id3dSignatureBlob->GetBufferSize(), IID_PPV_ARGS(m_ID3DRootSignatures[m_RootSignatureNames[i]].GetAddressOf())));
	}
}

D3D12_BLEND_DESC PipelineStateManager::GraphicsBlendDesc(UINT index)
{
	switch (index) {
	case RENDER_OBJ_PIPELINE:
	case CREATE_SHDOWMAP_PIPELINE:
	case RENDER_DEFERRED_PIPELINE:
		return CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	}
	return CD3DX12_BLEND_DESC(D3D12_DEFAULT);
}

D3D12_RASTERIZER_DESC PipelineStateManager::GraphicsRasterRizerDesc(UINT index)
{
	switch (index) {
	case RENDER_OBJ_PIPELINE:
	case CREATE_SHDOWMAP_PIPELINE:
	case RENDER_DEFERRED_PIPELINE:
		return CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	}
	return CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
}

D3D12_DEPTH_STENCIL_DESC PipelineStateManager::GraphicsDepthStencilDesc(UINT index)
{
	switch (index) {
	case RENDER_OBJ_PIPELINE:
	case CREATE_SHDOWMAP_PIPELINE:
	case RENDER_DEFERRED_PIPELINE:
		return CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	}
	return CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
}

std::vector<D3D12_INPUT_ELEMENT_DESC> PipelineStateManager::GraphicsInputElementDesc(UINT index)
{
	std::vector<D3D12_INPUT_ELEMENT_DESC> elements;
	switch (index) {
	case RENDER_OBJ_PIPELINE:
	case CREATE_SHDOWMAP_PIPELINE:
		elements = { 
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TANGENT"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "MATINDEX", 0, DXGI_FORMAT_R32_UINT		, 0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 } 
		};
		break;
	case RENDER_DEFERRED_PIPELINE:
		break;
	}
	return elements;
}

std::vector<DXGI_FORMAT> PipelineStateManager::GraphicsRenderTargetFormat(UINT index)
{
	std::vector<DXGI_FORMAT> formats;
	switch (index) {
	case RENDER_DEFERRED_PIPELINE:
	case RENDER_OBJ_PIPELINE:
		formats = { DXGI_FORMAT_R8G8B8A8_UNORM };
		break;
	case CREATE_SHDOWMAP_PIPELINE:
		break;
	}
	return formats;
}

D3D12_SHADER_BYTECODE PipelineStateManager::VS(UINT index)
{
	switch (index) {
	case RENDER_OBJ_PIPELINE:
	case CREATE_SHDOWMAP_PIPELINE:
		return COMPILEDSHADER->GetShaderByteCode("VS");

	case RENDER_DEFERRED_PIPELINE:
		return COMPILEDSHADER->GetShaderByteCode("VSTextureFullScreen");
	}

	return D3D12_SHADER_BYTECODE();
}

D3D12_SHADER_BYTECODE PipelineStateManager::PS(UINT index)
{
	switch (index) {
	case RENDER_OBJ_PIPELINE:
	case CREATE_SHDOWMAP_PIPELINE:
		return COMPILEDSHADER->GetShaderByteCode("PS");
	case RENDER_DEFERRED_PIPELINE:
		return COMPILEDSHADER->GetShaderByteCode("PSTextureFullScreen");
	}
	return D3D12_SHADER_BYTECODE();
}

ID3D12RootSignature* PipelineStateManager::GraphicsRootSignature(const std::wstring& name)
{
	if (name == m_RootSignatureNames[RENDER_OBJ_SIGNATURE] || name == L"Shadow")
		return m_ID3DRootSignatures[m_RootSignatureNames[RENDER_OBJ_SIGNATURE]].Get();

	else if (name == m_RootSignatureNames[RENDER_DEFERRED_SIGNATURE])
		return m_ID3DRootSignatures[m_RootSignatureNames[RENDER_DEFERRED_SIGNATURE]].Get();
	
	return nullptr;
}

ID3D12PipelineState* PipelineStateManager::Pipeline(const std::wstring& name)
{
	return m_ID3DPipelineStates[name].Get();
}
