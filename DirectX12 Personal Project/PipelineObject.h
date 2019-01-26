#pragma once
/*
 *	ShaderObject Ŭ������ �׷���, ��ǻƮ ���������� ������Ʈ�� �ش� ������������ ���� �׷��� ��ü�� ������ ��� �ִ� Ŭ������.
 */

#include "Source/D3DUtil.h"

class ShaderObject
{
public:
	ShaderObject() = default;
	~ShaderObject() = default;

public:
	void ExecutePipeline(ComPtr<ID3D12GraphicsCommandList> id3dGraphicsCommandList);

protected:
	ComPtr<ID3D12RootSignature> m_ID3DRootSignature;
	ComPtr<ID3D12PipelineState> m_ID3DPipelineState;

protected:
	/* ī�޶� Ŭ���� ������ ����� ����Ʈ, ������ */
	D3D12_VIEWPORT						m_D3DViewport = { 0, 0, static_cast<float>(CLIENT_WIDTH),static_cast<float>(CLIENT_HEIGHT), 0.0f, 1.0f };
	D3D12_RECT							m_D3DScissorRect = { 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT };
};

class GraphicsShaderObjects : public ShaderObject
{
public:
	GraphicsShaderObjects() { };
	~GraphicsShaderObjects() { };

public:
	void CreateGraphicsPipeline(ComPtr<ID3D12Device> id3dDevice, ComPtr<ID3D12CommandList> id3dCommandList, const int numRenderTarget);
	void CreateGraphicsRootSignature(ComPtr<ID3D12Device> id3dDevice, ComPtr<ID3D12CommandList> id3dCommandList);
	
	virtual D3D12_BLEND_DESC					GraphicsBlendDesc();
	virtual D3D12_RASTERIZER_DESC				GraphicsRasterRizerDesc();
	virtual D3D12_DEPTH_STENCIL_DESC			GraphicsDepthStencilDesc();
	virtual D3D12_INPUT_LAYOUT_DESC				GraphicsInputLayoutDesc();
};
