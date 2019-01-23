#include "D3DDescriptorFactory.h"

D3D12_RESOURCE_DESC D3DDescriptorFactory::DepthStencilDesc(DefaultOption)
{
	D3D12_RESOURCE_DESC d3dDepthStencilDesc;

	d3dDepthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	d3dDepthStencilDesc.Alignment = 0;
	d3dDepthStencilDesc.Width = CLIENT_WIDTH;
	d3dDepthStencilDesc.Height = CLIENT_HEIGHT;
	d3dDepthStencilDesc.DepthOrArraySize = 1;
	d3dDepthStencilDesc.MipLevels = 1;
	d3dDepthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	d3dDepthStencilDesc.SampleDesc.Count = App4xMsaaState ? 4 : 1;
	d3dDepthStencilDesc.SampleDesc.Quality = App4xMsaaState ? (App4xMsaaQuality - 1) : 0;
	d3dDepthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	d3dDepthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	return d3dDepthStencilDesc;
}

D3D12_DEPTH_STENCIL_VIEW_DESC D3DDescriptorFactory::DepthStencilViewDesc(DefaultOption, DXGI_FORMAT format)
{
	D3D12_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	d3dDepthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;
	d3dDepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Format = format;
	d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;

	return d3dDepthStencilViewDesc;
}

D3D12_RESOURCE_DESC D3DDescriptorFactory::RenderTargetDesc(DefaultOption)
{
	return D3D12_RESOURCE_DESC();
}

DXGI_SWAP_CHAIN_DESC D3DDescriptorFactory::SwapChainDesc(DefaultOption, DXGI_FORMAT format, int swapChainCount, HWND& wndHandle)
{
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;

	dxgiSwapChainDesc.BufferDesc.Width = CLIENT_WIDTH;
	dxgiSwapChainDesc.BufferDesc.Height = CLIENT_HEIGHT;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferDesc.Format = format;
	dxgiSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dxgiSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgiSwapChainDesc.SampleDesc.Count = App4xMsaaState ? 4 : 1;
	dxgiSwapChainDesc.SampleDesc.Quality = App4xMsaaState ? (App4xMsaaQuality - 1) : 0;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.BufferCount = swapChainCount;
	dxgiSwapChainDesc.OutputWindow = wndHandle;
	dxgiSwapChainDesc.Windowed = true;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	return dxgiSwapChainDesc;
}

D3D12_DESCRIPTOR_HEAP_DESC D3DDescriptorFactory::RenderTargetDescriptorHeapDesc(DefaultOption, const unsigned int bufferCount)
{
	return D3D12_DESCRIPTOR_HEAP_DESC{ D3D12_DESCRIPTOR_HEAP_TYPE_RTV, bufferCount, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 0 };
}

D3D12_DESCRIPTOR_HEAP_DESC D3DDescriptorFactory::DepthStencilDescriptorHeapDesc(DefaultOption, const unsigned int bufferCount)
{
	return D3D12_DESCRIPTOR_HEAP_DESC{ D3D12_DESCRIPTOR_HEAP_TYPE_DSV, bufferCount, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 0 };
}

D3D12_COMMAND_QUEUE_DESC D3DDescriptorFactory::CommandQueueDesc(DefaultOption)
{
	return D3D12_COMMAND_QUEUE_DESC{ D3D12_COMMAND_LIST_TYPE_DIRECT, 0,	D3D12_COMMAND_QUEUE_FLAG_NONE, 0 };
}
