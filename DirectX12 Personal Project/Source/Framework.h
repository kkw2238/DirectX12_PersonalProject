#pragma once

/*
 *	�� Ŭ�������� Window, Framework, DirectX12 ��ü ���� �� �ʱ�ȭ �۾�
 *  Window Msgó��, Update, Draw������ ��� ����
 */

#include "D3DUtil.h"
#include "..//Scene.h"

class Framework
{
	enum RenderTargets{ MainRenderTarget = 0 };
	enum DepthStencils{ MainDepthStencil = 0 };

public:
	Framework(HINSTANCE hInst);
	~Framework();
 
	/* Connecting Window */
public:
	static Framework*	GetFramework();

	HINSTANCE			FrameworkInst()	const;
	HWND				MainHWND() const;
	LRESULT				MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	unsigned int		OnMouseDown(WPARAM state, int xpos, int ypos);
	unsigned int		OnMouseUp(WPARAM state, int xpos, int ypos);
	unsigned int		OnMouseMove(WPARAM state, int xpos, int ypos);

	bool				Initialized();
	void				SetFrameStatesAtWindowText();

	int					Run();

	/* Initialized Window */
protected:
	bool				InitMainWindow();

	/* Create Direct12 */
protected:
	void				CreateRtvNDsvDescripotrHeaps();
	void				CreateCommandListObject();
	void				CreateSwapChain();

	/* Initialized Direct12 */
protected:
	bool				InitDirect12Device();

	/* Used when drawing */
protected:
	bool				FlushCommandQueue();
	
	void				Draw(const float fps);
	void				OnResize();
	void				Update(const float fps);

	/* Get Resource */
protected:
	ID3D12Resource*				GetCurrentBackBuffer() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView(const int index) const;

protected:
	
	static Framework*	m_Framework;

	HINSTANCE			m_AppInstanceHandle;
	HWND				m_MainWindowHandle;
	
	bool				m_AppPausedState;
	bool				m_AppMinimizedState;
	bool				m_AppMaximizedState;
	bool				m_AppResizingState;
	bool				m_AppFullScreenState;

	const float			m_FPS = 1.0f / 60.0f;	// �ӽ� ������ ����Ʈ

protected:
	
	ComPtr<IDXGIFactory4>				m_IDxgiFactory;
	ComPtr<IDXGISwapChain>				m_IDxgiSwapChain;
	ComPtr<ID3D12Device>				m_ID3DDevice;

	ComPtr<ID3D12Fence>					m_ID3DFence;
	UINT64								m_CurrentFenceCount = 0;

	ComPtr<ID3D12CommandQueue>			m_ID3DCommandQueue;
	ComPtr<ID3D12CommandAllocator>		m_ID3DCommandAllocator;
	ComPtr<ID3D12GraphicsCommandList>	m_ID3DCommandList;

	static const int					m_RenderTargetBufferCount = 1;
	static const int					m_DepthStencilBufferCount = 1;
	static const int					m_SwapChainBufferCount = 2;
	int									m_CurrentSwapChainBufferIndex = 0;

	ComPtr<ID3D12Resource>				m_ID3DSwapChainBuffer[m_SwapChainBufferCount];
	ComPtr<ID3D12Resource>				m_ID3DRenderTargetBuffer[m_RenderTargetBufferCount];
	ComPtr<ID3D12Resource>				m_ID3DDepthStencilBuffer[m_DepthStencilBufferCount];

	ComPtr<ID3D12DescriptorHeap>		m_ID3DSwapChainBufferViewHeap;
	ComPtr<ID3D12DescriptorHeap>		m_ID3DRenderTargetBufferViewHeap;
	ComPtr<ID3D12DescriptorHeap>		m_ID3DDepthStencilViewHeap;

	D3D12_VIEWPORT						m_D3DViewport;
	D3D12_RECT							m_D3DScissorRect;

	UINT								m_RenderTargetViewDescriptorSize = 0;
	UINT								m_DepthStencilViewDescriptorSize = 0;
	UINT								m_CbvSrvUavDescriptorSize = 0;	// ConstantBufferView, ShaderResourceView, UnorderedAccessView

	std::wstring						m_MainWindowTitle = L"DirectX12 Project ";

	D3D_DRIVER_TYPE						m_D3DDriverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT							m_DXGIBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT							m_DXGIDepthStencilBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	Scene								m_Scene;
};