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

void ComputeObjectShader::BuildPipelineObject(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const int numRenderTarget)
{
	m_DescriptorHeapUAVStart = 1;
	
	CreateDescriptorHeap(id3dDevice, id3dGraphicsCommandList, 0, 1, 1);
	BuildComputeObjects(id3dDevice, id3dGraphicsCommandList);

	CreateSRV(id3dDevice, m_ID3DDescriptorHeap.Get(), m_DescriptorHeapSRVStart, false);
	CreateUAV(id3dDevice, m_ID3DDescriptorHeap.Get(), m_DescriptorHeapUAVStart, false);
}

void ComputeObjectShader::BuildComputeObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	m_ThreadCount = XMUINT3((CLIENT_WIDTH * CLIENT_HEIGHT) / 1024, 0, 0);

	m_TextureInfos.emplace_back(TEXMANAGER->LoadTexture(id3dDevice, id3dGraphicsCommandList, std::wstring(L"Textures\\tile_nmap.DDS"), L"DEFAULT_NORM", DDS_ALPHA_MODE_UNKNOWN, false), 0);
	m_TextureInfos.emplace_back(std::make_shared<UnorderedAccessBuffer>(id3dDevice, id3dGraphicsCommandList, L"UATestbuffer", 16, CLIENT_WIDTH, CLIENT_HEIGHT), 3);
}

void ComputeObjectShader::CreateUAV(ID3D12Device* id3dDevice, ID3D12DescriptorHeap* id3dDescriptorHeap, UINT offset, bool isUsedDescriptorArray)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE UAVCPUDescriptorHandle;
	CD3DX12_GPU_DESCRIPTOR_HANDLE UAVGPUDescriptorHandle;
	DESCFACTORY->CraeteCPUGPUDescriptorHandle(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, id3dDescriptorHeap, UAVCPUDescriptorHandle, UAVGPUDescriptorHandle, offset);

	m_IsUsedDescriptorHeapArray = isUsedDescriptorArray;

	for (UINT i = 0; i < m_TextureInfos.size(); ++i) {
		m_TextureInfos[i].CreateUAV(id3dDevice, UAVCPUDescriptorHandle, UAVGPUDescriptorHandle);
			UAVCPUDescriptorHandle.Offset(DESCFACTORY->DescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
		if (!m_IsUsedDescriptorHeapArray)
			UAVGPUDescriptorHandle.Offset(DESCFACTORY->DescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	}

	for (size_t i = 0; i != m_TextureInfos.size(); ++i)
		TEXMANAGER->LinkedTexture(m_TextureInfos[i].Data()->Name(), &m_TextureInfos[i]);
}
