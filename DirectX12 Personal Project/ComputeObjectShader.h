#pragma once
#include "PipelineObject.h"

class ComputeObjectShader : public ShaderObject
{
public:
	ComputeObjectShader();
	~ComputeObjectShader();

	virtual void ExecutePipeline(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& pipelineName, const std::wstring& signatureName, Camera* camera = nullptr);
	virtual void BuildPipelineObject(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const int numRenderTarget);
	virtual void BuildComputeObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);
	void CreateUAV(ID3D12Device* id3dDevice, ID3D12DescriptorHeap* id3dDescriptorHeap, UINT offset, bool isUsedDescriptorArray);

protected:
	XMUINT3 m_ThreadCount;
	UINT m_DescriptorHeapUAVStart = 0;
};