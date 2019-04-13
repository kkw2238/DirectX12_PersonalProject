#pragma once
#include "D3DUtil.h"

class PipelineStateManager
{
	enum GraphicsPipelineIndex{ PIPELINE_RENDER_OBJ, PIPELINE_CREATE_SHDOWMAP, PIPELINE_RENDER_DEFERRED, PIPELINE_RENDER_SR_DEBUG };
	enum GraphicsRootSignatureIndex { SIGNATURE_RENDER_OBJ, SIGNATURE_CREATE_SHDOWMAP, SIGNATURE_RENDER_DEFERRED };
	enum GraphicsCBRegisterIndex { CB_CAM, CB_OBJ, CB_LIGHT };
	enum GraphicsSRRegisterIndex { INST_OBJ_INFO = 1, SR_TEXTURE_0, SR_TEXTURE_1, SR_TEXTURE_2, SR_TEXTURE_3 };

	enum ComputeRootSignatureIndex { SIGNATURE_CALCULATE_LUM_FIRSTPASS, SIGNATURE_CALCULATE_LUM_SECONDPASS };
	enum ComputePipelineIndex { PIPELINE_MIX, PIPELINE_BLOOM, PIPELINE_HDR };
	enum ComputeCBRegisterIndex { CB_MIX };
	enum ComputeSRRegisterIndex { SR_ORIGIN_TEXTURE };
	enum ComputeUAVRegisterIndex { UAV_OUTPUT_TEXTURE, UAV_LUM_FACTOR, UAV_TEST };

public:
	PipelineStateManager();
	~PipelineStateManager();

public:
	static PipelineStateManager* Instance();

	void CreatePipelineStates(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	void CreateGraphicsPipelineStates(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	void CreateGraphicsRootSignatures(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	void CreateComputePipelineStates(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	void CreateComputeRootSignatures(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);

public:
	D3D12_BLEND_DESC						GraphicsBlendDesc(UINT index);
	D3D12_RASTERIZER_DESC					GraphicsRasterRizerDesc(UINT index);
	D3D12_DEPTH_STENCIL_DESC				GraphicsDepthStencilDesc(UINT index);
	std::vector<D3D12_INPUT_ELEMENT_DESC>	GraphicsInputElementDesc(UINT index);
	std::vector<DXGI_FORMAT>				GraphicsRenderTargetFormat(UINT index);

	D3D12_CACHED_PIPELINE_STATE				ComputeCachedPSO(UINT index);

	D3D12_SHADER_BYTECODE					VS(UINT index);
	D3D12_SHADER_BYTECODE					PS(UINT index);
	D3D12_SHADER_BYTECODE					CS(UINT index);

	ID3D12RootSignature*					RootSignature(const std::wstring& name);
	ID3D12PipelineState*					Pipeline(const std::wstring& name);
protected:
	std::vector<std::wstring> m_ComputePipelineNames			= { L"CalLumFirstPass", L"CalLumSecondPass" };
	std::vector<std::wstring> m_ComputeRootSignatureNames		= { L"CalLumFirstPass", L"CalLumSecondPass" };

	std::unordered_map<std::wstring, std::vector<D3D12_INPUT_ELEMENT_DESC>>	m_D3DInputElementDescs;

	std::unordered_map<std::wstring, ComPtr<ID3D12RootSignature>> m_ID3DRootSignatures;
	std::unordered_map<std::wstring, ComPtr<ID3D12PipelineState>> m_ID3DPipelineStates;
};

#define PIPELINESTATE_MANAGER PipelineStateManager::Instance()