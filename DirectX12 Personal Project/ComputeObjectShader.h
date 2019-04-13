#pragma once
#include "PipelineObject.h"

class ComputeObjectShader : public ShaderObject
{
public:
	ComputeObjectShader();
	~ComputeObjectShader();

	virtual void ExecutePipeline(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& pipelineName, const std::wstring& signatureName, Camera* camera = nullptr);
	virtual void CreateDescriptorHeap(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT CBVCount, UINT SRVCount, UINT UAVCount);
	virtual void BuildPipelineObject(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const int numRenderTarget);
	void CreateUAV(ID3D12Device* id3dDevice, ID3D12DescriptorHeap* id3dDescriptorHeap, UINT offset, bool isUsedDescriptorArray);

protected:
	XMUINT3 m_ThreadCount;

};

