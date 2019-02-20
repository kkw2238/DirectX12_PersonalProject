#pragma once
/*
 *	ShaderObject Ŭ������ �׷���, ��ǻƮ ���������� ������Ʈ�� �ش� ������������ ���� �׷��� ��ü�� ������ ��� �ִ� Ŭ������.
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

protected:
	ComPtr<ID3D12DescriptorHeap> m_ID3DDescriptorHeap;

protected:
	UINT m_DescriptorHeapSRVStart = 0;
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

	void CreateSRV(ID3D12Device* id3dDevice, ID3D12DescriptorHeap* id3dDescriptorHeap, UINT offset, bool isUsedDescriptorArray);
	void UpdateTextureInfo(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);
protected:
	std::vector<TextureRootInfo>	m_TextureInfos;
	bool							m_IsUsedDescriptorHeapArray = false;
};
