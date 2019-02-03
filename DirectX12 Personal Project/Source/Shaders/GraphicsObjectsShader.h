#pragma once
#include "PipelineObject.h"

class GraphicsObjectsShader : public GraphicsShaderBase
{
	enum ROOTPAPAMETER_INDEX { CAMERAINFO_CB, OBJINFO_CB, TEXTURE_SR };
public:
	GraphicsObjectsShader() { };
	~GraphicsObjectsShader() { };

public:
	virtual void BuildPipelineObject(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const int numRenderTarget);
	virtual void RenderGraphicsObj(ID3D12GraphicsCommandList* id3dGraphicsCommandList, Camera* camera = nullptr);

	void CreateGraphicsRootSignature(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	void BuildGraphicsObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);

	virtual D3D12_INPUT_LAYOUT_DESC				GraphicsInputLayoutDesc();

	virtual D3D12_SHADER_BYTECODE				VS();
	virtual D3D12_SHADER_BYTECODE				PS();

protected:
	std::vector<D3D12_INPUT_ELEMENT_DESC>	m_D3DInputElementDescs;

	std::vector<GraphicsMeshObject*>	m_TestObject;
};