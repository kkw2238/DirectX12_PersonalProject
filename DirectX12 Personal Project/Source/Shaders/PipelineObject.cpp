#include "PipelineObject.h"
#include "CompiledShader.h"
#include "PipelineStateManager.h"

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

void GraphicsShaderBase::CreateSRV(ID3D12Device* id3dDevice, ID3D12DescriptorHeap* id3dDescriptorHeap, UINT offset, bool isUsedDescriptorArray)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE SRVCPUDescriptorHandle;
	CD3DX12_GPU_DESCRIPTOR_HANDLE SRVGPUDescriptorHandle;
	DESCFACTORY->CraeteCPUGPUDescriptorHandle(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, id3dDescriptorHeap, SRVCPUDescriptorHandle, SRVGPUDescriptorHandle, offset);

	m_IsUsedDescriptorHeapArray = isUsedDescriptorArray;

	for (UINT i = 0; i < m_TextureInfos.size(); ++i) {
		m_TextureInfos[i].CreateSRV(id3dDevice, SRVCPUDescriptorHandle, SRVGPUDescriptorHandle);

		SRVCPUDescriptorHandle.Offset(DESCFACTORY->DescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
		if (!m_IsUsedDescriptorHeapArray)
			SRVGPUDescriptorHandle.Offset(DESCFACTORY->DescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	}

	for (size_t i = 0; i != m_TextureInfos.size(); ++i)
		TEXMANAGER->LinkedTexture(m_TextureInfos[i].Data()->Name(), &m_TextureInfos[i]);
}

void GraphicsShaderBase::UpdateTextureInfo(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	if (m_IsUsedDescriptorHeapArray && m_TextureInfos.size() > 0) {
		m_TextureInfos[0].UpdateInfo(id3dDevice, id3dGraphicsCommandList);
		return;
	}

	for (UINT i = 0; i < m_TextureInfos.size(); ++i) {
		m_TextureInfos[i].UpdateInfo(id3dDevice, id3dGraphicsCommandList);
	}
}

GraphicsShaderBase::GraphicsShaderBase()
{}

void GraphicsShaderBase::ExecutePipeline(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, Camera* camera)
{
	id3dGraphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	id3dGraphicsCommandList->SetGraphicsRootSignature(PIPELINESTATE_MANAGER->GraphicsRootSignature(m_RootSignatureName));
	id3dGraphicsCommandList->SetPipelineState(PIPELINESTATE_MANAGER->Pipeline(m_PipelineName));

	if (m_ID3DDescriptorHeap != nullptr)
		id3dGraphicsCommandList->SetDescriptorHeaps(1, m_ID3DDescriptorHeap.GetAddressOf());

	UpdateTextureInfo(id3dDevice, id3dGraphicsCommandList);
	RenderGraphicsObj(id3dDevice, id3dGraphicsCommandList, camera);
}
