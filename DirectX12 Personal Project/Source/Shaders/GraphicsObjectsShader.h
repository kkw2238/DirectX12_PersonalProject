#pragma once
#include "PipelineObject.h"

const unsigned int NUM_USED_MAP = 2;

class GraphicsObjectsShader : public GraphicsShaderBase
{
	enum ROOTPAPAMETER_INDEX { CAMERAINFO_CB, OBJINFO_CB, LIGHT_CB , TEXTURE_SR };
	enum DESCRIPTORHEAP_INDEX { DEFAULT_DSEC, PLANE_DESC };
public:
	GraphicsObjectsShader();
	~GraphicsObjectsShader() { };

public:
	virtual void RenderGraphicsObj(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, Camera* camera = nullptr);
	virtual void BuildPipelineObject(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	virtual void BuildGraphicsObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);

	GraphicsMeshObject* Objects(UINT index);

protected:
	std::vector<D3D12_INPUT_ELEMENT_DESC>	m_D3DInputElementDescs;

	std::vector<GraphicsMeshObject*>	m_TestObject;
};