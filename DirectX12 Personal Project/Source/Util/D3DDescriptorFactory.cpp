#include "D3DDescriptorFactory.h"

D3DDescriptorFactory* D3DDescriptorFactory::Instance()
{
	static D3DDescriptorFactory instance;
	return &instance;
}

D3D12_RESOURCE_DESC D3DDescriptorFactory::DepthStencilDesc(DefaultOption)
{
	return CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R24G8_TYPELESS,
		CLIENT_WIDTH,
		CLIENT_HEIGHT,
		1,
		1,
		App4xMsaaState ? 4 : 1,
		App4xMsaaState ? (App4xMsaaQuality - 1) : 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);
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

D3D12_RENDER_TARGET_VIEW_DESC D3DDescriptorFactory::RenderTargetViewDesc(DefaultOption, DXGI_FORMAT format)
{
	D3D12_RENDER_TARGET_VIEW_DESC d3dRenderTargetViewDesc;
	d3dRenderTargetViewDesc.Format = format;
	d3dRenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	d3dRenderTargetViewDesc.Texture2D.MipSlice = 0;
	d3dRenderTargetViewDesc.Texture2D.PlaneSlice = 0;

	return d3dRenderTargetViewDesc;
}

D3D12_RESOURCE_DESC D3DDescriptorFactory::RenderTargetDesc(DefaultOption)
{
	return CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM,
		CLIENT_WIDTH,
		CLIENT_HEIGHT,
		1,
		1,
		App4xMsaaState ? 4 : 1,
		App4xMsaaState ? (App4xMsaaQuality - 1) : 0,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);
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

D3D12_DESCRIPTOR_HEAP_DESC D3DDescriptorFactory::DescriptorHeapDesc(DefaultOption, D3D12_DESCRIPTOR_HEAP_TYPE heapType, const unsigned int bufferCount)
{
	return D3D12_DESCRIPTOR_HEAP_DESC{ heapType, bufferCount, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 0 };
}

D3D12_DESCRIPTOR_HEAP_DESC D3DDescriptorFactory::DescriptorHeapDesc(D3D12_DESCRIPTOR_HEAP_TYPE heapType, D3D12_DESCRIPTOR_HEAP_FLAGS flag, const unsigned int bufferCount)
{
	return D3D12_DESCRIPTOR_HEAP_DESC{ heapType, bufferCount, flag, 0 };
}


D3D12_COMMAND_QUEUE_DESC D3DDescriptorFactory::CommandQueueDesc(DefaultOption)
{
	return D3D12_COMMAND_QUEUE_DESC{ D3D12_COMMAND_LIST_TYPE_DIRECT, 0,	D3D12_COMMAND_QUEUE_FLAG_NONE, 0 };
}

std::vector<CD3DX12_STATIC_SAMPLER_DESC> D3DDescriptorFactory::SamplerDesc(DefaultOption)
{
	return std::vector<CD3DX12_STATIC_SAMPLER_DESC>{
		CD3DX12_STATIC_SAMPLER_DESC{
			0,
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			0.0f,
			1,
			D3D12_COMPARISON_FUNC_ALWAYS,
			D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
			0.0f,
			D3D12_FLOAT32_MAX,
			D3D12_SHADER_VISIBILITY_PIXEL
		},
		CD3DX12_STATIC_SAMPLER_DESC{
			1,
			D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,
			0.0f,
			1,
			D3D12_COMPARISON_FUNC_LESS_EQUAL,
			D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
			0.0f,
			D3D12_FLOAT32_MAX,
			D3D12_SHADER_VISIBILITY_PIXEL
		}
	};
}

std::vector<CD3DX12_STATIC_SAMPLER_DESC> D3DDescriptorFactory::SamplerDescs(DefaultOption)
{
	return std::vector<CD3DX12_STATIC_SAMPLER_DESC>();
}

D3D12_SHADER_RESOURCE_VIEW_DESC D3DDescriptorFactory::SRVDesc(const D3D12_RESOURCE_DESC& d3dResourceDesc, D3D12_SRV_DIMENSION dimension)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc;

	d3dSRVDesc.Format = (d3dResourceDesc.Format == DXGI_FORMAT_R24G8_TYPELESS) ? DXGI_FORMAT_R24_UNORM_X8_TYPELESS : d3dResourceDesc.Format;
	d3dSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	d3dSRVDesc.ViewDimension = dimension;
	switch (dimension)
	{
	case D3D12_SRV_DIMENSION_BUFFER:
		d3dSRVDesc.Buffer = D3D12_BUFFER_SRV{ 0, 0, 0, D3D12_BUFFER_SRV_FLAG_NONE };
		break;
	case D3D12_SRV_DIMENSION_TEXTURE1D:
		d3dSRVDesc.Texture1D = D3D12_TEX1D_SRV{ 0, static_cast<UINT>(-1), 0.0f };
		break;
	case D3D12_SRV_DIMENSION_TEXTURE2D:
		d3dSRVDesc.Texture2D = D3D12_TEX2D_SRV{ 0, static_cast<UINT>(-1), 0, 0.0f };
		break;
	case D3D12_SRV_DIMENSION_TEXTURE2DARRAY:
		d3dSRVDesc.Texture2DArray = D3D12_TEX2D_ARRAY_SRV{ 0, static_cast<UINT>(-1), 0, 0, 0, 0.0f };
		break;
	case D3D12_SRV_DIMENSION_TEXTURECUBE:
		d3dSRVDesc.TextureCube = D3D12_TEXCUBE_SRV{ 0, static_cast<UINT>(-1), 0.0f };
		break;
	default:
		break;
	}

	return d3dSRVDesc;
}

D3D12_UNORDERED_ACCESS_VIEW_DESC D3DDescriptorFactory::UAVDesc(const D3D12_RESOURCE_DESC& d3dResourceDesc, D3D12_UAV_DIMENSION dimension)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC d3dUAVDesc;
	d3dUAVDesc.Format = d3dResourceDesc.Format;
	d3dUAVDesc.ViewDimension = dimension;
	d3dUAVDesc.Buffer.NumElements = d3dResourceDesc.Height * d3dResourceDesc.Width;

	return d3dUAVDesc;
}

void D3DDescriptorFactory::SetCBVSRVUAVdescriptorHandleIncrementSize(UINT size)
{
	m_CBVSRVUAVdescriptorHandleIncrementSize = size;
}

void D3DDescriptorFactory::SetRTVdescriptorHandleIncrementSize(UINT size)
{
	m_RTVdescriptorHandleIncrementSize = size;
}

void D3DDescriptorFactory::SetDSVdescriptorHandleIncrementSize(UINT size)
{
	m_DSVdescriptorHandleIncrementSize = size;
}

UINT D3DDescriptorFactory::DescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE heapType)
{
	switch (heapType) {
	case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
		return m_CBVSRVUAVdescriptorHandleIncrementSize;

	case D3D12_DESCRIPTOR_HEAP_TYPE_RTV:
		return m_CBVSRVUAVdescriptorHandleIncrementSize;
		
	case D3D12_DESCRIPTOR_HEAP_TYPE_DSV:
		return m_DSVdescriptorHandleIncrementSize;
	}

	return 0;
}

void D3DDescriptorFactory::CraeteCPUGPUDescriptorHandle(D3D12_DESCRIPTOR_HEAP_TYPE heapType, ID3D12DescriptorHeap* id3dDescriptorHeap, CD3DX12_CPU_DESCRIPTOR_HANDLE& cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE& gpuHandle, UINT offset)
{
	D3D12_CPU_DESCRIPTOR_HANDLE cpuhandle = id3dDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE Gpuhandle = id3dDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	cpuHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(id3dDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), offset, DescriptorHandleIncrementSize(heapType));
	gpuHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(id3dDescriptorHeap->GetGPUDescriptorHandleForHeapStart(), offset, DescriptorHandleIncrementSize(heapType));
}
