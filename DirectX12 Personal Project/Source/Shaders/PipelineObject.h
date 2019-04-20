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
	virtual void ExecutePipeline(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& pipelineName, const std::wstring& signatureName, Camera* camera = nullptr);
	virtual void CreateDescriptorHeap(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT CBVCount, UINT SRVCount, UINT UAVCount);
	virtual void BuildPipelineObject(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const int numRenderTarget) {};
	void CreateSRV(ID3D12Device* id3dDevice, ID3D12DescriptorHeap* id3dDescriptorHeap, UINT offset, bool isUsedDescriptorArray);

protected:
	ComPtr<ID3D12DescriptorHeap>	m_ID3DDescriptorHeap;
	std::vector<TextureRootInfo>	m_TextureInfos;
	bool							m_IsUsedDescriptorHeapArray = false;

protected:
	UINT m_DescriptorHeapSRVStart = 0;
	UINT m_DescriptorHeapUAVStart = 0;
};

class GraphicsShaderBase : public ShaderObject
{
public:
	GraphicsShaderBase();
	~GraphicsShaderBase() {};

public:
	virtual void BuildPipelineObject(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList) {};
	virtual void ExecutePipeline(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& pipelineName, const std::wstring& signatureName, Camera* camera = nullptr);
	virtual void RenderGraphicsObj(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, Camera* camera = nullptr) {};
	virtual void CreateShadow(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& pipelineName, const std::wstring& signatureName, Camera* camera = nullptr);

	void UpdateTextureInfo(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);

};
