#pragma once
#include "D3DUtil.h"

class PipelineStateManager
{
	enum PipelineIndex{ RENDER_OBJ_PIPELINE, CREATE_SHDOWMAP_PIPELINE, RENDER_DEFERRED_PIPELINE };
	enum RootSignatureIndex { RENDER_OBJ_SIGNATURE, RENDER_DEFERRED_SIGNATURE };
public:
	PipelineStateManager();
	~PipelineStateManager();

public:
	static PipelineStateManager* Instance();

	void CreatePipelineStates(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	void CreateGraphicsPipelineStates(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	void CreateComputePipelineStates(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	void CreateRootSignatures(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);

public:
	D3D12_BLEND_DESC						GraphicsBlendDesc(UINT index);
	D3D12_RASTERIZER_DESC					GraphicsRasterRizerDesc(UINT index);
	D3D12_DEPTH_STENCIL_DESC				GraphicsDepthStencilDesc(UINT index);
	std::vector<D3D12_INPUT_ELEMENT_DESC>	GraphicsInputElementDesc(UINT index);
	std::vector<DXGI_FORMAT>				GraphicsRenderTargetFormat(UINT index);

	D3D12_SHADER_BYTECODE					VS(UINT index);
	D3D12_SHADER_BYTECODE					PS(UINT index);

	ID3D12RootSignature*					GraphicsRootSignature(const std::wstring& name);
	ID3D12PipelineState*					Pipeline(const std::wstring& name);
protected:
	std::vector<std::wstring> m_PipelineNames			= { L"Obj", L"Shadow", L"Deferred" };
	std::vector<std::wstring> m_RootSignatureNames	= { L"Obj", L"Deferred" };

	std::unordered_map<std::wstring, std::vector<D3D12_INPUT_ELEMENT_DESC>>	m_D3DInputElementDescs;

	std::unordered_map<std::wstring, ComPtr<ID3D12RootSignature>> m_ID3DRootSignatures;
	std::unordered_map<std::wstring, ComPtr<ID3D12PipelineState>> m_ID3DPipelineStates;
};

#define PIPELINESTATE_MANAGER PipelineStateManager::Instance()