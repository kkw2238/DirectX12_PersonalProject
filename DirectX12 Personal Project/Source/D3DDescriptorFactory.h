#pragma once
/*
 *	D3DDescriptorFactory Ŭ������ DirectX����̽� ���� ��ũ���͵��� �������ִ� Ŭ������
 */

#include "../stdafx.h"

struct DefaultOption { };

#define DEFAULTOPT DefaultOption()

class D3DDescriptorFactory
{
public:
	static D3D12_RESOURCE_DESC DepthStencilDesc(DefaultOption);
	static D3D12_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc(DefaultOption, DXGI_FORMAT format);

	static D3D12_RESOURCE_DESC RenderTargetDesc(DefaultOption);

	static DXGI_SWAP_CHAIN_DESC SwapChainDesc(DefaultOption, DXGI_FORMAT format, int swapChainCount, HWND& wndHandle);

	static D3D12_DESCRIPTOR_HEAP_DESC RenderTargetDescriptorHeapDesc(DefaultOption, const unsigned int bufferCount);
	static D3D12_DESCRIPTOR_HEAP_DESC DepthStencilDescriptorHeapDesc(DefaultOption, const unsigned int bufferCount);

	static D3D12_COMMAND_QUEUE_DESC CommandQueueDesc(DefaultOption);
};
