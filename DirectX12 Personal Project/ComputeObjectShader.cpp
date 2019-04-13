#include "ComputeObjectShader.h"
#include "PipelineStateManager.h"

ComputeObjectShader::ComputeObjectShader()
{
}

ComputeObjectShader::~ComputeObjectShader()
{
}

void ComputeObjectShader::ExecutePipeline(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& pipelineName, const std::wstring& signatureName, Camera* camera)
{
	id3dGraphicsCommandList->SetPipelineState(PIPELINESTATE_MANAGER->Pipeline(pipelineName));
	id3dGraphicsCommandList->SetComputeRootSignature(PIPELINESTATE_MANAGER->RootSignature(signatureName));
	id3dGraphicsCommandList->SetDescriptorHeaps(1, m_ID3DDescriptorHeap.GetAddressOf());

	id3dGraphicsCommandList->Dispatch(m_ThreadCount.x, m_ThreadCount.y, m_ThreadCount.z);
}

void ComputeObjectShader::CreateDescriptorHeap(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT CBVCount, UINT SRVCount, UINT UAVCount)
{
}

void ComputeObjectShader::BuildPipelineObject(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const int numRenderTarget)
{
	CreateDescriptorHeap(id3dDevice, id3dGraphicsCommandList, 0, 1, 3);

	CreateSRV(id3dDevice, m_ID3DDescriptorHeap.Get(), m_DescriptorHeapSRVStart, true);
}

void ComputeObjectShader::CreateUAV(ID3D12Device* id3dDevice, ID3D12DescriptorHeap* id3dDescriptorHeap, UINT offset, bool isUsedDescriptorArray)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE UAVCPUDescriptorHandle;
	CD3DX12_GPU_DESCRIPTOR_HANDLE UAVGPUDescriptorHandle;
	DESCFACTORY->CraeteCPUGPUDescriptorHandle(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, id3dDescriptorHeap, UAVCPUDescriptorHandle, UAVGPUDescriptorHandle, offset);

	m_IsUsedDescriptorHeapArray = isUsedDescriptorArray;

	for (UINT i = 0; i < m_TextureInfos.size(); ++i) {
		m_TextureInfos[i].CreateSRV(id3dDevice, UAVCPUDescriptorHandle, UAVGPUDescriptorHandle);
		id3dDevice->CreateUnorderedAccessView()
		SRVCPUDescriptorHandle.Offset(DESCFACTORY->DescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
		if (!m_IsUsedDescriptorHeapArray)
			SRVGPUDescriptorHandle.Offset(DESCFACTORY->DescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	}

	for (size_t i = 0; i != m_TextureInfos.size(); ++i)
		TEXMANAGER->LinkedTexture(m_TextureInfos[i].Data()->Name(), &m_TextureInfos[i]);
}
