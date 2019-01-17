#include "Framework.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return Framework::GetFramework()->MsgProc(hWnd, message, wParam, lParam);
}

Framework::Framework(HINSTANCE hInst) :
	m_AppInstanceHandle(hInst)
{
	assert(m_Framework == nullptr);
	m_Framework = this;
}

Framework::~Framework()
{
	if (m_ID3DDevice != nullptr)
		FlushCommandQueue();
}

Framework* Framework::m_Framework = nullptr;
Framework * Framework::GetFramework()
{
	return m_Framework;
}

HINSTANCE Framework::FrameworkInst() const
{
	return m_AppInstanceHandle;
}

HWND Framework::MainHWND() const
{
	return m_MainWindowHandle;
}

LRESULT Framework::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		return 0;

	case WM_SIZE:
		CLIENT_WIDTH = LOWORD(lParam);
		CLIENT_HEIGHT = HIWORD(lParam);
		if (m_ID3DDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				m_AppPausedState = true;
				m_AppMinimizedState = true;
				m_AppMaximizedState = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_AppPausedState = false;
				m_AppMinimizedState = false;
				m_AppMaximizedState = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (m_AppMinimizedState)
				{
					m_AppPausedState = false;
					m_AppMinimizedState = false;
					OnResize();
				}

				else if (m_AppMaximizedState)
				{
					m_AppPausedState = false;
					m_AppMaximizedState = false;
					OnResize();
				}
				else if (m_AppResizingState)
				{
				}
				else
				{
					OnResize();
				}
			}
		}
		return 0;

	case WM_ENTERSIZEMOVE:
		m_AppPausedState = true;
		m_AppResizingState = true;
		//mTimer.Stop();
		return 0;

	case WM_EXITSIZEMOVE:
		m_AppPausedState = false;
		m_AppResizingState = false;
		//mTimer.Start();
		OnResize();
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		else if ((int)wParam == VK_F2)
			App4xMsaaState = !App4xMsaaState;

		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

unsigned int Framework::OnMouseDown(WPARAM state, int xpos, int ypos)
{
	return 0;
}

unsigned int Framework::OnMouseUp(WPARAM state, int xpos, int ypos)
{
	return 0;
}

unsigned int Framework::OnMouseMove(WPARAM state, int xpos, int ypos)
{
	return 0;
}

bool Framework::Initialized()
{
	if (!InitMainWindow()) return false;
	if (!InitDirect12Device()) return false;

	OnResize();
	
	return true;
}

void Framework::SetFrameStatesAtWindowText()
{
}

int Framework::Run()
{
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else
		{
			if (!m_AppPausedState)
			{
				SetFrameStatesAtWindowText();
				Update(m_FPS);
				Draw(m_FPS);
			}
			else
			{
				Sleep(100);
			}
		}
	}

	return (int)msg.wParam;
}

bool Framework::InitMainWindow()
{
	WNDCLASS wc{
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		m_AppInstanceHandle,
		LoadIcon(0, IDI_APPLICATION),
		LoadCursor(0, IDC_ARROW),
		(HBRUSH)GetStockObject(NULL_BRUSH),
		0,	
		L"MainHWND"
	}; 

	//wc.style = CS_HREDRAW | CS_VREDRAW;
	//wc.lpfnWndProc = WndProc;
	//wc.cbClsExtra = 0;
	//wc.cbWndExtra = 0;
	//wc.hInstance = m_AppInstanceHandle;
	//wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	//wc.hCursor = LoadCursor(0, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	//wc.lpszMenuName = 0;
	//wc.lpszClassName = L"MainHWND";
	
	if (!RegisterClass(&wc))
	{
		try {
			ThrowIfFail(GetLastError() - 0x3e80);
		}
		catch (ExeptionUtility& e) {
			std::wstring errorMsg = L"WndClassFailed!! \n";
			errorMsg += e.WinErrorToString().c_str();
			MessageBox(0, errorMsg.c_str(), 0, 0);
			return false;
		}
	}

	RECT windowRect = { 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);
	int width	= windowRect.right - windowRect.left;
	int height	= windowRect.bottom - windowRect.top;

	m_MainWindowHandle = CreateWindow(L"MainHWND", m_MainWindowTitle.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_AppInstanceHandle, 0);
	if (!m_MainWindowHandle)
	{
		try {
			ThrowIfFail(GetLastError() - 0x3e80);
		}
		catch (ExeptionUtility& e) {
			std::wstring errorMsg = L"CreateWindowFailed!! \n";
			errorMsg += e.WinErrorToString().c_str();
			MessageBox(0, errorMsg.c_str(), 0, 0);
			return false;
		}
	}

	ShowWindow(m_MainWindowHandle, SW_SHOW);
	UpdateWindow(m_MainWindowHandle);

	return true;
}

void Framework::CreateRtvNDsvDescripotrHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dSwapChainHeapDesc{
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		m_SwapChainBufferCount,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0 
	}; ThrowIfFail(m_ID3DDevice->CreateDescriptorHeap(&d3dSwapChainHeapDesc, IID_PPV_ARGS(m_ID3DSwapChainBufferViewHeap.GetAddressOf())));

	D3D12_DESCRIPTOR_HEAP_DESC d3dDsvHeapDesc{
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		m_DepthStencilBufferCount,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0 
	}; ThrowIfFail(m_ID3DDevice->CreateDescriptorHeap(&d3dDsvHeapDesc, IID_PPV_ARGS(m_ID3DDepthStencilViewHeap.GetAddressOf())));

	D3D12_DESCRIPTOR_HEAP_DESC d3dRtvHeapDesc{
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		m_RenderTargetBufferCount,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0 
	}; ThrowIfFail(m_ID3DDevice->CreateDescriptorHeap(&d3dRtvHeapDesc, IID_PPV_ARGS(m_ID3DRenderTargetBufferViewHeap.GetAddressOf())));
}

void Framework::CreateCommandListObject()
{
	D3D12_COMMAND_QUEUE_DESC d3dQueueDesc{
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		0,
		D3D12_COMMAND_QUEUE_FLAG_NONE,
		0
	}; ThrowIfFail(m_ID3DDevice->CreateCommandQueue(&d3dQueueDesc, IID_PPV_ARGS(&m_ID3DCommandQueue)));

	ThrowIfFail(m_ID3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_ID3DCommandAllocator)));
	ThrowIfFail(m_ID3DDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_ID3DCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_ID3DCommandList)));

	m_ID3DCommandList->Close();
}

void Framework::CreateSwapChain()
{
	m_IDxgiSwapChain.Reset();

	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	dxgiSwapChainDesc.BufferDesc.Width	= CLIENT_WIDTH;
	dxgiSwapChainDesc.BufferDesc.Height = CLIENT_HEIGHT;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferDesc.Format = m_DXGIBackBufferFormat;
	dxgiSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dxgiSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgiSwapChainDesc.SampleDesc.Count = App4xMsaaState ? 4 : 1;
	dxgiSwapChainDesc.SampleDesc.Quality = App4xMsaaState ? (App4xMsaaQuality - 1) : 0;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.BufferCount = m_SwapChainBufferCount;
	dxgiSwapChainDesc.OutputWindow = m_MainWindowHandle;
	dxgiSwapChainDesc.Windowed = true;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ThrowIfFail(m_IDxgiFactory->CreateSwapChain(m_ID3DCommandQueue.Get(), &dxgiSwapChainDesc, m_IDxgiSwapChain.GetAddressOf()));
}

bool Framework::InitDirect12Device()
{
#if defined(_DEBUG)
	ComPtr<ID3D12Debug> id3dDebugController;
	D3D12GetDebugInterface(IID_PPV_ARGS(&id3dDebugController));
	id3dDebugController->EnableDebugLayer();
#endif

	CreateDXGIFactory1(IID_PPV_ARGS(&m_IDxgiFactory));
	HRESULT hardwareResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0,	IID_PPV_ARGS(&m_ID3DDevice));
	if (FAILED(hardwareResult))
	{
		ComPtr<IDXGIAdapter> idxgiWarpAdapter;
		m_IDxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&idxgiWarpAdapter));
		ThrowIfFail(D3D12CreateDevice(idxgiWarpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_ID3DDevice)));
	} 
	ThrowIfFail(m_ID3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_ID3DFence)));

	m_RenderTargetViewDescriptorSize = m_ID3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_DepthStencilViewDescriptorSize = m_ID3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_CbvSrvUavDescriptorSize = m_ID3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMultisampleQualityLevels{
		m_DXGIBackBufferFormat,
		4,
		D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE,
		0
	}; ThrowIfFail(m_ID3DDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &d3dMultisampleQualityLevels, sizeof(d3dMultisampleQualityLevels)));

	App4xMsaaQuality = d3dMultisampleQualityLevels.NumQualityLevels;
	assert(App4xMsaaQuality > 0 && "MSAA Quality level 0");

	CreateCommandListObject();
	CreateSwapChain();
	CreateRtvNDsvDescripotrHeaps();

	return true;
}

bool Framework::FlushCommandQueue()
{
	m_CurrentFenceCount++;

	ThrowIfFail(m_ID3DCommandQueue->Signal(m_ID3DFence.Get(), m_CurrentFenceCount));

	if (m_ID3DFence->GetCompletedValue() < m_CurrentFenceCount)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		ThrowIfFail(m_ID3DFence->SetEventOnCompletion(m_CurrentFenceCount, eventHandle));

		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	return true;
}

void Framework::Draw(const float fps)
{
	ThrowIfFail(m_ID3DCommandAllocator->Reset());
	ThrowIfFail(m_ID3DCommandList->Reset(m_ID3DCommandAllocator.Get(), nullptr));

	D3DUtil::ChangeResourceState(GetCurrentBackBuffer(), m_ID3DCommandList.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	m_ID3DCommandList->RSSetScissorRects(1, &m_D3DScissorRect);
	m_ID3DCommandList->RSSetViewports(1, &m_D3DViewport);

	m_ID3DCommandList->ClearRenderTargetView(GetCurrentBackBufferView(), DirectX::Colors::Black, 0, nullptr);
	m_ID3DCommandList->ClearDepthStencilView(GetDepthStencilView(MainDepthStencil), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	m_ID3DCommandList->OMSetRenderTargets(1, &GetCurrentBackBufferView(), true, &GetDepthStencilView(MainDepthStencil));

	D3DUtil::ChangeResourceState(GetCurrentBackBuffer(), m_ID3DCommandList.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	ThrowIfFail(m_ID3DCommandList->Close());

	ID3D12CommandList* cmdsLists[] = { m_ID3DCommandList.Get() };
	m_ID3DCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	ThrowIfFail(m_IDxgiSwapChain->Present(0, 0));

	m_CurrentSwapChainBufferIndex = (m_CurrentSwapChainBufferIndex + 1) % m_SwapChainBufferCount;

	FlushCommandQueue();

}

void Framework::OnResize()
{
	assert(m_ID3DDevice);
	assert(m_IDxgiSwapChain);
	assert(m_ID3DCommandAllocator);

	FlushCommandQueue();

	ThrowIfFail(m_ID3DCommandList->Reset(m_ID3DCommandAllocator.Get(), nullptr));
	for (int i = 0; i < m_SwapChainBufferCount; ++i)
		m_ID3DSwapChainBuffer[i].Reset();
	m_ID3DDepthStencilBuffer[MainDepthStencil].Reset();

	m_IDxgiSwapChain->ResizeBuffers(
		m_SwapChainBufferCount,
		CLIENT_WIDTH, CLIENT_HEIGHT,
		m_DXGIBackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	);

	m_CurrentSwapChainBufferIndex = 0;

	/*  */
	CD3DX12_CPU_DESCRIPTOR_HANDLE cD3DRenderTargetViewHeapHandle(m_ID3DSwapChainBufferViewHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < m_SwapChainBufferCount; ++i)
	{
		ThrowIfFail(m_IDxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_ID3DSwapChainBuffer[i])));
		m_ID3DDevice->CreateRenderTargetView(m_ID3DSwapChainBuffer[i].Get(), nullptr, cD3DRenderTargetViewHeapHandle);
		cD3DRenderTargetViewHeapHandle.Offset(1, m_RenderTargetViewDescriptorSize);
	}

	D3D12_RESOURCE_DESC d3dDepthStencilDesc = D3DDescriptorOption::DepthStencilDesc(DEFAULTOPT);

	D3D12_CLEAR_VALUE d3dClearOption;
	d3dClearOption.Format = m_DXGIDepthStencilBufferFormat;
	d3dClearOption.DepthStencil.Depth = 1.0f;
	d3dClearOption.DepthStencil.Stencil = 0;
	m_ID3DDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&d3dDepthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&d3dClearOption,
		IID_PPV_ARGS(m_ID3DDepthStencilBuffer[MainDepthStencil].GetAddressOf())
	);

	D3D12_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	d3dDepthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;
	d3dDepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Format = m_DXGIDepthStencilBufferFormat;
	d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;

	m_ID3DDevice->CreateDepthStencilView(m_ID3DDepthStencilBuffer[MainDepthStencil].Get(), &d3dDepthStencilViewDesc, GetDepthStencilView(MainDepthStencil));

	D3DUtil::ChangeResourceState(m_ID3DDepthStencilBuffer[MainDepthStencil].Get(), m_ID3DCommandList.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);

	ThrowIfFail(m_ID3DCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { m_ID3DCommandList.Get() };
	m_ID3DCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	FlushCommandQueue();
	
	m_D3DViewport.TopLeftX = 0;
	m_D3DViewport.TopLeftY = 0;
	m_D3DViewport.Width = static_cast<float>(CLIENT_WIDTH);
	m_D3DViewport.Height = static_cast<float>(CLIENT_HEIGHT);
	m_D3DViewport.MinDepth = 0.0f;
	m_D3DViewport.MaxDepth = 1.0f;

	m_D3DScissorRect = { 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT };
}

void Framework::Update(const float fps)
{
}

ID3D12Resource * Framework::GetCurrentBackBuffer() const
{
	return m_ID3DSwapChainBuffer[m_CurrentSwapChainBufferIndex].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE Framework::GetCurrentBackBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		m_ID3DSwapChainBufferViewHeap->GetCPUDescriptorHandleForHeapStart(), 
		m_CurrentSwapChainBufferIndex,
		m_RenderTargetViewDescriptorSize
	);
}

D3D12_CPU_DESCRIPTOR_HANDLE Framework::GetDepthStencilView(const int index) const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		m_ID3DDepthStencilViewHeap->GetCPUDescriptorHandleForHeapStart(),
		index,
		m_DepthStencilViewDescriptorSize
	);
}
