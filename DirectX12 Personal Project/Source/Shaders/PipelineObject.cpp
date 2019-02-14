#include "PipelineObject.h"
#include "CompiledShader.h"

void ShaderObject::ExecutePipeline(ID3D12GraphicsCommandList* id3dGraphicsCommandList, Camera* camera)
{
}

void ShaderObject::CreateDescriptorHeap(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT CBVCount, UINT SRVCount, UINT UAVCount)
{
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = DESCFACTORY->DescriptorHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, CBVCount + SRVCount + UAVCount);

	m_DescriptorHeapSRVStart = CBVCount;

	ThrowIfFail(id3dDevice->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(m_ID3DDescriptorHeap.GetAddressOf())));
}

//////////////////////////////////////


void GraphicsShaderBase::CreateGraphicsPipeline(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const int numRenderTarget, std::vector<DXGI_FORMAT>& RTFormats)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dGraphicsPipelineDesc;
	::ZeroMemory(&d3dGraphicsPipelineDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	d3dGraphicsPipelineDesc.BlendState = GraphicsBlendDesc();
	d3dGraphicsPipelineDesc.DepthStencilState = GraphicsDepthStencilDesc();
	d3dGraphicsPipelineDesc.InputLayout = GraphicsInputLayoutDesc();
	d3dGraphicsPipelineDesc.RasterizerState = GraphicsRasterRizerDesc();
	d3dGraphicsPipelineDesc.pRootSignature = m_ID3DRootSignature.Get();
	d3dGraphicsPipelineDesc.VS = VS();
	d3dGraphicsPipelineDesc.PS = PS();
	d3dGraphicsPipelineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	d3dGraphicsPipelineDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	for (int i = 0; i < numRenderTarget; ++i)
		d3dGraphicsPipelineDesc.RTVFormats[i] = RTFormats[i];
	d3dGraphicsPipelineDesc.NumRenderTargets = numRenderTarget;
	d3dGraphicsPipelineDesc.SampleMask = UINT_MAX;
	d3dGraphicsPipelineDesc.SampleDesc.Count = 1;
	d3dGraphicsPipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	ThrowIfFail(id3dDevice->CreateGraphicsPipelineState(&d3dGraphicsPipelineDesc, IID_PPV_ARGS(m_ID3DPipelineState.GetAddressOf())));
}

void GraphicsShaderBase::ExecutePipeline(ID3D12GraphicsCommandList* id3dGraphicsCommandList, Camera* camera)
{
	id3dGraphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	id3dGraphicsCommandList->SetGraphicsRootSignature(m_ID3DRootSignature.Get());
	id3dGraphicsCommandList->SetPipelineState(m_ID3DPipelineState.Get());

	if (m_ID3DDescriptorHeap != nullptr)
		id3dGraphicsCommandList->SetDescriptorHeaps(1, m_ID3DDescriptorHeap.GetAddressOf());

	RenderGraphicsObj(id3dGraphicsCommandList, camera);
}


D3D12_BLEND_DESC GraphicsShaderBase::GraphicsBlendDesc()
{
	return CD3DX12_BLEND_DESC(D3D12_DEFAULT);
}

D3D12_RASTERIZER_DESC GraphicsShaderBase::GraphicsRasterRizerDesc()
{
	return CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
}

D3D12_DEPTH_STENCIL_DESC GraphicsShaderBase::GraphicsDepthStencilDesc()
{
	return CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
}

D3D12_INPUT_LAYOUT_DESC GraphicsShaderBase::GraphicsInputLayoutDesc()
{
	return D3D12_INPUT_LAYOUT_DESC();
}

D3D12_SHADER_BYTECODE GraphicsShaderBase::VS()
{
	return D3D12_SHADER_BYTECODE();
}

D3D12_SHADER_BYTECODE GraphicsShaderBase::PS()
{
	return D3D12_SHADER_BYTECODE();
}