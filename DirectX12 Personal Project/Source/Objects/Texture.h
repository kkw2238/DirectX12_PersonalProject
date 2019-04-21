#pragma once
#include "D3DUtil.h"
#include "DDSTextureLoader.h"

class TextureRootInfo;

class Texture
{
public:
	Texture();
	Texture(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& texName, const std::wstring& fileName, DDS_ALPHA_MODE alphaMode, bool isCubeMap, D3D12_SRV_DIMENSION srvDimension = D3D12_SRV_DIMENSION_TEXTURE2D);
	Texture(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& texName, UINT width, UINT height, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS resourceFlag, D3D12_RESOURCE_STATES resourceState, D3D12_SRV_DIMENSION srvDimension = D3D12_SRV_DIMENSION_TEXTURE2D);
	Texture(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& bufferName, UINT width, D3D12_RESOURCE_FLAGS resourceFlag, D3D12_RESOURCE_STATES resourceState, D3D12_SRV_DIMENSION srvDimension = D3D12_SRV_DIMENSION_BUFFER);
	Texture(ID3D12Resource* texture, const std::wstring& texName, D3D12_SRV_DIMENSION srvDimension);
	~Texture();

public:
	ID3D12Resource* Resource() const;
	virtual ID3D12Resource* ReadBackResource() const { return nullptr; };

	D3D12_RESOURCE_DESC ResourceDesc() const;
	virtual D3D12_RESOURCE_DESC ReadBackResourceDesc() const { return D3D12_RESOURCE_DESC(); };

	D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress();
	bool IsUnorderedAccessBuffer() const { return m_bISUnorderedAccessBuffer; }

	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc() const;
	virtual D3D12_UNORDERED_ACCESS_VIEW_DESC UAVDesc() const { return D3D12_UNORDERED_ACCESS_VIEW_DESC(); };
	virtual void ReadBackUABuffer(ID3D12GraphicsCommandList* id3dGraphicsCommandList) {};

	std::wstring Name() const;

protected:
	ComPtr<ID3D12Resource> m_ID3DTexture;
	ComPtr<ID3D12Resource> m_ID3DTextureUploadBuffer;

	std::wstring m_TextureName;
	D3D12_SHADER_RESOURCE_VIEW_DESC m_D3DSRVDesc;

	bool m_bISUnorderedAccessBuffer = false;
};

class UnorderedAccessBuffer : public Texture {
public:
	UnorderedAccessBuffer() = default;
	UnorderedAccessBuffer(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& bufferName, UINT width, D3D12_RESOURCE_FLAGS resourceFlag, D3D12_RESOURCE_STATES resourceState, D3D12_SRV_DIMENSION srvDimension = D3D12_SRV_DIMENSION_BUFFER, D3D12_UAV_DIMENSION uavDimension = D3D12_UAV_DIMENSION_BUFFER);
	UnorderedAccessBuffer(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& texName, UINT width, UINT height, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS resourceFlag, D3D12_RESOURCE_STATES resourceState, D3D12_SRV_DIMENSION srvDimension = D3D12_SRV_DIMENSION_TEXTURE2D, D3D12_UAV_DIMENSION uavDimension = D3D12_UAV_DIMENSION_TEXTURE2D);
	UnorderedAccessBuffer(ID3D12Resource* buffer, const std::wstring& texName, D3D12_UAV_DIMENSION uavDimension);
public:
	D3D12_UNORDERED_ACCESS_VIEW_DESC UAVDesc() const;

	void ReadBackUABuffer(ID3D12GraphicsCommandList* id3dGraphicsCommandList);

	virtual ID3D12Resource* ReadBackResource() const { return m_ID3DReadBackTexture.Get(); };
	virtual D3D12_RESOURCE_DESC ReadBackResouceDesc() const { return m_ID3DReadBackTexture->GetDesc(); };

protected:
	D3D12_UNORDERED_ACCESS_VIEW_DESC m_D3DUAVDesc;

	ComPtr<ID3D12Resource> m_ID3DReadBackTexture;
};

class TextureRootInfo
{
public:
	TextureRootInfo() {};
	TextureRootInfo(std::shared_ptr<Texture> texture, UINT rootSignautreIndex);
	~TextureRootInfo() {};

public:
	std::shared_ptr<Texture> Data();
	ID3D12Resource* Resource();
	ID3D12Resource* ReadBackResource();
	D3D12_RESOURCE_DESC ResourceDesc();
	D3D12_RESOURCE_DESC ReadBackResourceDesc();

	D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress();

	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc() const;
	D3D12_UNORDERED_ACCESS_VIEW_DESC UAVDesc() const;

	CD3DX12_CPU_DESCRIPTOR_HANDLE CPUHandle() const;
	CD3DX12_GPU_DESCRIPTOR_HANDLE GPUHandle() const;

	void CreateSRV(ID3D12Device* id3dDevice, const CD3DX12_CPU_DESCRIPTOR_HANDLE& cpuHandle, const CD3DX12_GPU_DESCRIPTOR_HANDLE& gpuHandle);
	void CreateUAV(ID3D12Device* id3dDevice, const CD3DX12_CPU_DESCRIPTOR_HANDLE& cpuHandle, const CD3DX12_GPU_DESCRIPTOR_HANDLE& gpuHandle);

	void RefreshSRV(ID3D12Device* id3dDevice);
	void RefreshUAV(ID3D12Device* id3dDevice);

	void RefreshTexture(ID3D12Device* id3dDevice, std::shared_ptr<Texture> texture);
	void UpdateInfo(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);

	void UpdateUAVInfo(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList);

protected:
	std::shared_ptr<Texture> m_Texture;
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_CPUDescHandle;
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_GPUDescHandle;

	UINT m_rootSignatureIndex;
};