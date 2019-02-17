#pragma once
#include "PipelineObject.h"
#include "CompiledShader.h"

class DeferredRenderShader : public GraphicsShaderBase
{
	enum ROOTPAPAMETER_INDEX { TEXTURE_SR };
public:
	DeferredRenderShader();
	~DeferredRenderShader();

public:
	virtual void BuildPipelineObject(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	virtual void BuildGraphicsObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList) ;

	virtual void RenderGraphicsObj(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, Camera* camera = nullptr);
protected:
	std::vector<GraphicsTextureObject*> m_TextureObject;
};

