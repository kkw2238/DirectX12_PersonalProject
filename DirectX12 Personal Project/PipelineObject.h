#pragma once
/*
 *	ShaderObject 클래스는 그래픽, 컴퓨트 파이프라인 오브젝트와 해당 파이프라인을 통해 그려질 객체의 정보를 담고 있는 클래스임.
 */

#include "Source/D3DUtil.h"
#include "Objects.h"
#include "Camera.h"

class ShaderObject
{
public:
	ShaderObject() = default;
	~ShaderObject() = default;

public:
	virtual void ExecutePipeline(ID3D12GraphicsCommandList* id3dGraphicsCommandList, Camera* camera = nullptr);

protected:
	ComPtr<ID3D12RootSignature> m_ID3DRootSignature;
	ComPtr<ID3D12PipelineState> m_ID3DPipelineState;

protected:
	/* 카메라 클래스 생성전 사용할 뷰포트, 시저렉 */
	D3D12_VIEWPORT						m_D3DViewport = { 0, 0, static_cast<float>(CLIENT_WIDTH),static_cast<float>(CLIENT_HEIGHT), 0.0f, 1.0f };
	D3D12_RECT							m_D3DScissorRect = { 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT };
};

class GraphicsShaderObjects : public ShaderObject
{
	enum CBUFFERINFO { CAMERAINFO_CB, OBJINFO_CB };
public:
	GraphicsShaderObjects() { };
	~GraphicsShaderObjects() { };

public:
	virtual void ExecutePipeline(ID3D12GraphicsCommandList* id3dGraphicsCommandList, Camera* camera = nullptr);

	void CreateGraphicsPipeline(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dCommandList, const int numRenderTarget);
	void CreateGraphicsRootSignature(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dCommandList);

	void BuildGraphicsObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dCommandList);

	virtual D3D12_BLEND_DESC					GraphicsBlendDesc();
	virtual D3D12_RASTERIZER_DESC				GraphicsRasterRizerDesc();
	virtual D3D12_DEPTH_STENCIL_DESC			GraphicsDepthStencilDesc();
	virtual D3D12_INPUT_LAYOUT_DESC				GraphicsInputLayoutDesc();

protected:
	std::vector<D3D12_INPUT_ELEMENT_DESC>	m_D3DInputElementDescs;

	std::vector<GraphicsObjects*>	m_TestObject;
};