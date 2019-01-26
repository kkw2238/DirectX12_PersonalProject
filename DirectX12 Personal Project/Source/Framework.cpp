#include "Framework.h"
#include "D3DDescriptorFactory.h"

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

	m_Scene.BuildObjects(m_ID3DDevice, m_ID3DCommandList);
	
	return true;
}

void Framework::SetFrameStatesAtWindowText()
{
}

int Framework::Run()
{
	MSG msg = { 0 };

	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else {
			try{
				if (!m_AppPausedState) {
					SetFrameStatesAtWindowText();
					Update(m_FPS);
					Draw(m_FPS);
				}
				else {
					Sleep(100);
				}
			}
			catch (ExeptionUtility & e) {
				MessageBox(nullptr, e.D3DErrorToString().c_str(), L"HR Failed", MB_OK);
				return 0;
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
	ThrowIfFail(m_ID3DDevice->CreateDescriptorHeap(
		&D3DDescriptorFactory::RenderTargetDescriptorHeapDesc(DEFAULTOPT, m_SwapChainBufferCount), IID_PPV_ARGS(m_ID3DSwapChainBufferViewHeap.GetAddressOf())));

	ThrowIfFail(m_ID3DDevice->CreateDescriptorHeap(
		&D3DDescriptorFactory::DepthStencilDescriptorHeapDesc(DEFAULTOPT, m_DepthStencilBufferCount), IID_PPV_ARGS(m_ID3DDepthStencilViewHeap.GetAddressOf())));

	ThrowIfFail(m_ID3DDevice->CreateDescriptorHeap(
		&D3DDescriptorFactory::RenderTargetDescriptorHeapDesc(DEFAULTOPT, m_RenderTargetBufferCount), IID_PPV_ARGS(m_ID3DRenderTargetBufferViewHeap.GetAddressOf())));
}

void Framework::CreateCommandListObject()
{
	ThrowIfFail(m_ID3DDevice->CreateCommandQueue(&D3DDescriptorFactory::CommandQueueDesc(DEFAULTOPT), IID_PPV_ARGS(&m_ID3DCommandQueue)));

	ThrowIfFail(m_ID3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_ID3DCommandAllocator)));
	
	ThrowIfFail(m_ID3DDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_ID3DCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_ID3DCommandList)));

	m_ID3DCommandList->Close();
}

void Framework::CreateSwapChain()
{
	m_IDxgiSwapChain.Reset();

	ThrowIfFail(m_IDxgiFactory->CreateSwapChain(m_ID3DCommandQueue.Get(), 
		&D3DDescriptorFactory::SwapChainDesc(DEFAULTOPT, m_DXGIBackBufferFormat, m_SwapChainBufferCount, m_MainWindowHandle), 
		m_IDxgiSwapChain.GetAddressOf())
	);
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

	m_RenderTargetViewDescriptorSize	= m_ID3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_DepthStencilViewDescriptorSize	= m_ID3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_CbvSrvUavDescriptorSize			= m_ID3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

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

	m_Scene.RenderObjects(m_ID3DCommandList);

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

	D3D12_RESOURCE_DESC d3dDepthStencilDesc = D3DDescriptorFactory::DepthStencilDesc(DEFAULTOPT);

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

	m_ID3DDevice->CreateDepthStencilView(m_ID3DDepthStencilBuffer[MainDepthStencil].Get(), 
		&D3DDescriptorFactory::DepthStencilViewDesc(DEFAULTOPT, m_DXGIDepthStencilBufferFormat), GetDepthStencilView(MainDepthStencil));

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
