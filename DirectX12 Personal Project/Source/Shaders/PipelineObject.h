#pragma once
/*
 *	ShaderObject 클래스는 그래픽, 컴퓨트 파이프라인 오브젝트와 해당 파이프라인을 통해 그려질 객체의 정보를 담고 있는 클래스임.
 */

#include "D3DUtil.h"
#include "Objects.h"
#include "Camera.h"
#include "D3DDescriptorFactory.h"

class ShaderObject
{
public:
	ShaderObject() = default;
	~ShaderObject() = default;

public:
	virtual void ExecutePipeline(ID3D12GraphicsCommandList* id3dGraphicsCommandList, Camera* camera = nullptr);
	virtual void CreateDescriptorHeap(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT CBVCount, UINT SRVCount, UINT UAVCount);
	virtual void BuildPipelineObject(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const int numRenderTarget) {};

protected:
	ComPtr<ID3D12RootSignature> m_ID3DRootSignature;
	ComPtr<ID3D12PipelineState> m_ID3DPipelineState;
	ComPtr<ID3D12DescriptorHeap> m_ID3DDescriptorHeap;

protected:
	UINT m_DescriptorHeapSRVStart = 0;
};

class GraphicsShaderBase : public ShaderObject
{
public:
	GraphicsShaderBase() {};
	~GraphicsShaderBase() {};

public:
	virtual void BuildPipelineObject(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const int numRenderTarget) {};
	
	virtual void ExecutePipeline(ID3D12GraphicsCommandList* id3dGraphicsCommandList, Camera* camera = nullptr);
	virtual void RenderGraphicsObj(ID3D12GraphicsCommandList* id3dGraphicsCommandList, Camera* camera = nullptr) {};
	virtual void CreateGraphicsRootSignature(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList) {};

	void CreateGraphicsPipeline(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const int numRenderTarget, std::vector<DXGI_FORMAT>& RTFormats);
	
public:
	virtual D3D12_BLEND_DESC					GraphicsBlendDesc();
	virtual D3D12_RASTERIZER_DESC				GraphicsRasterRizerDesc();
	virtual D3D12_DEPTH_STENCIL_DESC			GraphicsDepthStencilDesc();
	virtual D3D12_INPUT_LAYOUT_DESC				GraphicsInputLayoutDesc();

	virtual D3D12_SHADER_BYTECODE				VS();
	virtual D3D12_SHADER_BYTECODE				PS();
};
