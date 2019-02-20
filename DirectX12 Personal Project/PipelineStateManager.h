#pragma once
#include "D3DUtil.h"

class PipelineStateManager
{
	enum PipelineIndex{ PIPELINE_RENDER_OBJ, PIPELINE_CREATE_SHDOWMAP, PIPELINE_RENDER_DEFERRED, PIPELINE_RENDER_SR_DEBUG };
	enum RootSignatureIndex { SIGNATURE_RENDER_OBJ, SIGNATURE_CREATE_SHDOWMAP, SIGNATURE_RENDER_DEFERRED };
	enum CBRegisterIndex { CB_CAM, CB_OBJ, CB_LIGHT };
	enum SRRegisterIndex { INST_OBJ_INFO = 1, SR_TEXTURE_0, SR_TEXTURE_1, SR_TEXTURE_2, SR_TEXTURE_3 };

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
	std::vector<std::wstring> m_PipelineNames			= { L"Obj", L"Shadow", L"Deferred", L"DebugSR" };
	std::vector<std::wstring> m_RootSignatureNames	= { L"Obj", L"Shadow", L"Deferred" };

	std::unordered_map<std::wstring, std::vector<D3D12_INPUT_ELEMENT_DESC>>	m_D3DInputElementDescs;

	std::unordered_map<std::wstring, ComPtr<ID3D12RootSignature>> m_ID3DRootSignatures;
	std::unordered_map<std::wstring, ComPtr<ID3D12PipelineState>> m_ID3DPipelineStates;
};

#define PIPELINESTATE_MANAGER PipelineStateManager::Instance()