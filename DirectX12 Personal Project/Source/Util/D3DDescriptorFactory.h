#pragma once
/*
 *	D3DDescriptorFactory 클래스는 DirectX디바이스 관련 디스크립터들을 생성해주는 클래스임
 */

#include "../stdafx.h"

struct DefaultOption { };

#define DEFAULTOPT DefaultOption()

class D3DDescriptorFactory
{
public:
	D3DDescriptorFactory() {};
	~D3DDescriptorFactory() {};

public:
	static D3DDescriptorFactory* Instance();

public:
	D3D12_RESOURCE_DESC DepthStencilDesc(DefaultOption);
	D3D12_RESOURCE_DESC RenderTargetDesc(DefaultOption);
	DXGI_SWAP_CHAIN_DESC SwapChainDesc(DefaultOption, DXGI_FORMAT format, int swapChainCount, HWND& wndHandle);
	D3D12_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc(DefaultOption, DXGI_FORMAT format);
	D3D12_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc(DefaultOption, DXGI_FORMAT format);
	D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc(DefaultOption, D3D12_DESCRIPTOR_HEAP_TYPE heapType, const unsigned int bufferCount);
	D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE heapType, D3D12_DESCRIPTOR_HEAP_FLAGS flag, const unsigned int bufferCount);
	D3D12_COMMAND_QUEUE_DESC CommandQueueDesc(DefaultOption);
	std::vector<CD3DX12_STATIC_SAMPLER_DESC> SamplerDesc(DefaultOption);
	std::vector<CD3DX12_STATIC_SAMPLER_DESC> SamplerDescs(DefaultOption);

	D3D12_SHADER_RESOURCE_VIEW_DESC SRVResourceViewDesc(const D3D12_RESOURCE_DESC& d3dResourceDesc, D3D12_SRV_DIMENSION dimension);

	void SetCBVSRVUAVdescriptorHandleIncrementSize(UINT size);
	void SetRTVdescriptorHandleIncrementSize(UINT size);
	void SetDSVdescriptorHandleIncrementSize(UINT size);

	UINT DescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE heapType);

	void CraeteCPUGPUDescriptorHandle(
		D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		ID3D12DescriptorHeap* id3dDescriptorHeap, 
		CD3DX12_CPU_DESCRIPTOR_HANDLE& cpuHandle, 
		CD3DX12_GPU_DESCRIPTOR_HANDLE& gpuHandle, 
		UINT offset
	);

protected:
	UINT m_CBVSRVUAVdescriptorHandleIncrementSize = 0;
	UINT m_DSVdescriptorHandleIncrementSize = 0;
	UINT m_RTVdescriptorHandleIncrementSize = 0;
};

#define DESCFACTORY D3DDescriptorFactory::Instance()