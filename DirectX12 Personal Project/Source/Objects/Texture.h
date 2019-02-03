#pragma once
#include "D3DUtil.h"
#include "DDSTextureLoader.h"

class Texture
{
public:
	Texture();
	Texture(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& fileName, DDS_ALPHA_MODE alphaMode, bool isCubeMap, D3D12_SRV_DIMENSION srvDimension = D3D12_SRV_DIMENSION_TEXTURE2D);
	Texture(ID3D12Resource* texture, D3D12_SRV_DIMENSION srvDimension);
	~Texture();

public:
	ID3D12Resource* Resource();
	D3D12_RESOURCE_DESC ResouceDesc();
	D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress();
	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc() const;

protected:
	ComPtr<ID3D12Resource> m_ID3DTexture;
	ComPtr<ID3D12Resource> m_ID3DTextureUploadBuffer;

	D3D12_SHADER_RESOURCE_VIEW_DESC m_D3DResouceViewDesc;
};


class TextureRootInfo
{
public:
	TextureRootInfo() {};
	TextureRootInfo(std::shared_ptr<Texture> texture, UINT rootSignautreIndex);
	~TextureRootInfo() {};

public:
	ID3D12Resource* Resource();
	D3D12_RESOURCE_DESC ResouceDesc();
	D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress();
	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc() const;

	void SetDescriptorHandle(CD3DX12_CPU_DESCRIPTOR_HANDLE& cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE& gpuHandle);
	void UpdateInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList);

protected:
	std::shared_ptr<Texture> m_Texture;
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_CPUDescHandle;
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_GPUDescHandle;

	UINT m_rootSignatureIndex;
};