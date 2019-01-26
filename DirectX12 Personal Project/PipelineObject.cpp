#include "PipelineObject.h"
#include "CompiledShader.h"

void ShaderObject::ExecutePipeline(ComPtr<ID3D12GraphicsCommandList> id3dGraphicsCommandList)
{
	id3dGraphicsCommandList->SetGraphicsRootSignature(m_ID3DRootSignature.Get());
	id3dGraphicsCommandList->SetPipelineState(m_ID3DPipelineState.Get());

	id3dGraphicsCommandList->RSSetScissorRects(1, &m_D3DScissorRect);
	id3dGraphicsCommandList->RSSetViewports(1, &m_D3DViewport);

	id3dGraphicsCommandList->DrawInstanced(6, 1, 0, 0);
}

//////////////////////////////////////

void GraphicsShaderObjects::CreateGraphicsPipeline(ComPtr<ID3D12Device> id3dDevice, ComPtr<ID3D12CommandList> id3dCommandList, const int numRenderTarget)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dGraphicsPipelineDesc;
	::ZeroMemory(&d3dGraphicsPipelineDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	d3dGraphicsPipelineDesc.BlendState			= GraphicsBlendDesc();
	d3dGraphicsPipelineDesc.DepthStencilState	= GraphicsDepthStencilDesc();
	d3dGraphicsPipelineDesc.InputLayout			= GraphicsInputLayoutDesc();
	d3dGraphicsPipelineDesc.RasterizerState		= GraphicsRasterRizerDesc();
	d3dGraphicsPipelineDesc.pRootSignature		= m_ID3DRootSignature.Get();
	d3dGraphicsPipelineDesc.VS					= COMPILEDSHADER->GetShaderByteCode(L"hlsl\\DefaultShader.hlsl", nullptr, "VS", "vs_5_0");
	d3dGraphicsPipelineDesc.PS					= COMPILEDSHADER->GetShaderByteCode(L"hlsl\\DefaultShader.hlsl", nullptr, "PS", "ps_5_0");
	d3dGraphicsPipelineDesc.Flags				= D3D12_PIPELINE_STATE_FLAG_NONE;
	d3dGraphicsPipelineDesc.RTVFormats[0]		= DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dGraphicsPipelineDesc.DSVFormat			= DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dGraphicsPipelineDesc.NumRenderTargets	= numRenderTarget;
	d3dGraphicsPipelineDesc.SampleMask			= UINT_MAX;
	d3dGraphicsPipelineDesc.SampleDesc.Count	= 1;
	d3dGraphicsPipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	ThrowIfFail(id3dDevice->CreateGraphicsPipelineState(&d3dGraphicsPipelineDesc, IID_PPV_ARGS(m_ID3DPipelineState.GetAddressOf())));
}

void GraphicsShaderObjects::CreateGraphicsRootSignature(ComPtr<ID3D12Device> id3dDevice, ComPtr<ID3D12CommandList> id3dCommandList)
{
	CD3DX12_DESCRIPTOR_RANGE d3dRootDescriptorRange;

	CD3DX12_ROOT_PARAMETER d3dRootParameter;
	d3dRootParameter.InitAsConstants(0, 0);

	CD3DX12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	d3dRootSignatureDesc.Init(1, &d3dRootParameter);

	ComPtr<ID3DBlob> id3dSignatureBlob;
	ComPtr<ID3DBlob> id3dErrorBlob;

	ThrowIfFail(D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, id3dSignatureBlob.GetAddressOf(), id3dErrorBlob.GetAddressOf()));

	if(id3dErrorBlob != nullptr) ::OutputDebugStringA((char*)id3dErrorBlob->GetBufferPointer());

	ThrowIfFail(id3dDevice->CreateRootSignature(NULL, id3dSignatureBlob->GetBufferPointer(), id3dSignatureBlob->GetBufferSize(), IID_PPV_ARGS(m_ID3DRootSignature.GetAddressOf())));
}


D3D12_BLEND_DESC GraphicsShaderObjects::GraphicsBlendDesc()
{
	return CD3DX12_BLEND_DESC(D3D12_DEFAULT);
}

D3D12_RASTERIZER_DESC GraphicsShaderObjects::GraphicsRasterRizerDesc()
{
	return CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
}

D3D12_DEPTH_STENCIL_DESC GraphicsShaderObjects::GraphicsDepthStencilDesc()
{
	return CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
}

D3D12_INPUT_LAYOUT_DESC GraphicsShaderObjects::GraphicsInputLayoutDesc()
{
	return D3D12_INPUT_LAYOUT_DESC();
}

