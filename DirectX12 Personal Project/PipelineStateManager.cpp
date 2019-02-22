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

		switch (i) {
		case SIGNATURE_RENDER_OBJ:
			d3dRootDescriptorRange.resize(1);
			d3dRootDescriptorRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 5, SR_TEXTURE_0);

			d3dRootParameter.resize(4);
			d3dRootParameter[0].InitAsConstantBufferView(CB_CAM);
			d3dRootParameter[1].InitAsConstantBufferView(CB_OBJ);
			d3dRootParameter[2].InitAsConstantBufferView(CB_LIGHT);
			d3dRootParameter[3].InitAsDescriptorTable(1, &d3dRootDescriptorRange[0]);
			break;

		case SIGNATURE_RENDER_DEFERRED:
			d3dRootDescriptorRange.resize(1);
			d3dRootDescriptorRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 5, SR_TEXTURE_0);

			d3dRootParameter.resize(3);
			d3dRootParameter[0].InitAsConstantBufferView(CB_CAM);
			d3dRootParameter[1].InitAsConstantBufferView(CB_LIGHT);
			d3dRootParameter[2].InitAsDescriptorTable(1, &d3dRootDescriptorRange[0]);
			break;

		case SIGNATURE_CREATE_SHDOWMAP:
			d3dRootParameter.resize(3);
			d3dRootParameter[0].InitAsConstantBufferView(CB_CAM);
			d3dRootParameter[1].InitAsConstantBufferView(CB_OBJ);
			d3dRootParameter[2].InitAsConstantBufferView(CB_LIGHT);
			break;
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
	case PIPELINE_RENDER_OBJ:
	case PIPELINE_CREATE_SHDOWMAP:
	case PIPELINE_RENDER_DEFERRED:
	case PIPELINE_RENDER_SR_DEBUG:
		return CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	}
	return CD3DX12_BLEND_DESC(D3D12_DEFAULT);
}

D3D12_RASTERIZER_DESC PipelineStateManager::GraphicsRasterRizerDesc(UINT index)
{
	D3D12_RASTERIZER_DESC rasterizerDesc = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	switch (index) {
	case PIPELINE_CREATE_SHDOWMAP:
		rasterizerDesc.DepthBias = 10000;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.SlopeScaledDepthBias = 1.0f;
		return rasterizerDesc;

	case PIPELINE_RENDER_OBJ:
	case PIPELINE_RENDER_DEFERRED:
	case PIPELINE_RENDER_SR_DEBUG:
		return rasterizerDesc;
	}
	return rasterizerDesc;
}

D3D12_DEPTH_STENCIL_DESC PipelineStateManager::GraphicsDepthStencilDesc(UINT index)
{
	CD3DX12_DEPTH_STENCIL_DESC depthstencilDesc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	switch (index) {
	case PIPELINE_RENDER_OBJ:
	case PIPELINE_CREATE_SHDOWMAP:
		return depthstencilDesc;
	case PIPELINE_RENDER_DEFERRED:
	case PIPELINE_RENDER_SR_DEBUG:
		depthstencilDesc.DepthEnable = false;
		return depthstencilDesc;
	}
	return depthstencilDesc;
}

std::vector<D3D12_INPUT_ELEMENT_DESC> PipelineStateManager::GraphicsInputElementDesc(UINT index)
{
	std::vector<D3D12_INPUT_ELEMENT_DESC> elements;
	switch (index) {
	case PIPELINE_RENDER_OBJ:
	case PIPELINE_CREATE_SHDOWMAP:
		elements = { 
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TANGENT"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "MATINDEX", 0, DXGI_FORMAT_R32_UINT		, 0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 } 
		};
		break;
	case PIPELINE_RENDER_DEFERRED:
	case PIPELINE_RENDER_SR_DEBUG:
		break;
	}
	return elements;
}

std::vector<DXGI_FORMAT> PipelineStateManager::GraphicsRenderTargetFormat(UINT index)
{
	std::vector<DXGI_FORMAT> formats;
	switch (index) {
	case PIPELINE_RENDER_DEFERRED:
	case PIPELINE_RENDER_SR_DEBUG:
	case PIPELINE_RENDER_OBJ:
		formats = { DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM };
		break;
	case PIPELINE_CREATE_SHDOWMAP:
		break;
	}
	return formats;
}

D3D12_SHADER_BYTECODE PipelineStateManager::VS(UINT index)
{
	switch (index) {
	case PIPELINE_RENDER_OBJ:
	case PIPELINE_CREATE_SHDOWMAP:
		return COMPILEDSHADER->GetShaderByteCode("VS");

	case PIPELINE_RENDER_DEFERRED:
		return COMPILEDSHADER->GetShaderByteCode("VSTextureFullScreen");

	case PIPELINE_RENDER_SR_DEBUG:
		return COMPILEDSHADER->GetShaderByteCode("VSTextureDebug");
	}

	return D3D12_SHADER_BYTECODE();
}

D3D12_SHADER_BYTECODE PipelineStateManager::PS(UINT index)
{
	switch (index) {
	case PIPELINE_RENDER_OBJ:
		return COMPILEDSHADER->GetShaderByteCode("PS");
	case PIPELINE_CREATE_SHDOWMAP:
		return COMPILEDSHADER->GetShaderByteCode("PSNone");
	case PIPELINE_RENDER_DEFERRED:
		return COMPILEDSHADER->GetShaderByteCode("PSTextureFullScreen");
	case PIPELINE_RENDER_SR_DEBUG:
		return COMPILEDSHADER->GetShaderByteCode("PSTextureDebug");
	}
	return D3D12_SHADER_BYTECODE();
}

ID3D12RootSignature* PipelineStateManager::GraphicsRootSignature(const std::wstring& name)
{
	if (name == L"DebugSR")
		return m_ID3DRootSignatures[L"Deferred"].Get();

	return m_ID3DRootSignatures[name].Get();
}

ID3D12PipelineState* PipelineStateManager::Pipeline(const std::wstring& name)
{
	return m_ID3DPipelineStates[name].Get();
}
