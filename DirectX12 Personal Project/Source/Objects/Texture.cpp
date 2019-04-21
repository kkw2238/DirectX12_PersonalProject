#include "Texture.h"
#include "D3DDescriptorFactory.h"
#include "LightManager.h"
#include "TextureManager.h"

Texture::Texture()
{
}

Texture::Texture(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& texName, const std::wstring& fileName, DDS_ALPHA_MODE alphaMode, bool isCubeMap, D3D12_SRV_DIMENSION srvDimension)
{
	if(FAILED(CreateDDSTextureFromFile12(id3dDevice, id3dGraphicsCommandList, fileName.c_str(), m_ID3DTexture, m_ID3DTextureUploadBuffer, 0, &alphaMode)))
		return;

	m_TextureName = texName;

	m_D3DSRVDesc = DESCFACTORY->SRVDesc(m_ID3DTexture->GetDesc(), srvDimension);
}

Texture::Texture(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& bufferName, UINT width, D3D12_RESOURCE_FLAGS resourceFlag, D3D12_RESOURCE_STATES resourceState, D3D12_SRV_DIMENSION srvDimension)
{
	id3dDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(width, resourceFlag),
		resourceState,
		nullptr,
		IID_PPV_ARGS(m_ID3DTexture.GetAddressOf())
	);

	m_TextureName = bufferName;
	m_D3DSRVDesc = DESCFACTORY->SRVDesc(m_ID3DTexture->GetDesc(), srvDimension);
}

Texture::Texture(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& texName, UINT width, UINT height, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS resourceFlag, D3D12_RESOURCE_STATES resourceState, D3D12_SRV_DIMENSION srvDimension)
{
	id3dDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Tex2D(format, width, height, 1, 0, 1, 0, resourceFlag),
		resourceState,
		nullptr,
		IID_PPV_ARGS(m_ID3DTexture.GetAddressOf())
	);

	m_TextureName = texName;

	m_D3DSRVDesc = DESCFACTORY->SRVDesc(m_ID3DTexture->GetDesc(), srvDimension);
}

Texture::Texture(ID3D12Resource* texture, const std::wstring& texName, D3D12_SRV_DIMENSION srvDimension)
{
	m_ID3DTexture.Swap(texture);
	m_TextureName = texName;

	m_D3DSRVDesc = DESCFACTORY->SRVDesc(m_ID3DTexture->GetDesc(), srvDimension);
}

Texture::~Texture()
{
}

ID3D12Resource* Texture::Resource() const
{
	return m_ID3DTexture.Get();
}

D3D12_RESOURCE_DESC Texture::ResourceDesc() const
{
	return m_ID3DTexture->GetDesc();
}

D3D12_GPU_VIRTUAL_ADDRESS Texture::GPUVirtualAddress()
{
	return m_ID3DTexture->GetGPUVirtualAddress();
}

D3D12_SHADER_RESOURCE_VIEW_DESC Texture::SRVDesc() const
{
	return m_D3DSRVDesc;
}

std::wstring Texture::Name() const
{
	return m_TextureName;
}


/////////////////////////////////////////////////////////

UnorderedAccessBuffer::UnorderedAccessBuffer(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& bufferName, UINT width, D3D12_RESOURCE_FLAGS resourceFlag, D3D12_RESOURCE_STATES resourceState, D3D12_SRV_DIMENSION srvDimension, D3D12_UAV_DIMENSION uavDimension)
	: Texture(id3dDevice, id3dGraphicsCommandList, bufferName, width, resourceFlag, resourceState)
{
	id3dDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(width),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(m_ID3DReadBackTexture.GetAddressOf())
	);

	m_TextureName = bufferName;

	m_D3DUAVDesc = DESCFACTORY->UAVDesc(m_ID3DTexture->GetDesc(), uavDimension);
	m_D3DSRVDesc = DESCFACTORY->SRVDesc(m_ID3DReadBackTexture->GetDesc(), srvDimension);

	m_bISUnorderedAccessBuffer = true;
}

UnorderedAccessBuffer::UnorderedAccessBuffer(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& texName, UINT width, UINT height, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS resourceFlag, D3D12_RESOURCE_STATES resourceState, D3D12_SRV_DIMENSION srvDimension, D3D12_UAV_DIMENSION uavDimension)
	: Texture(id3dDevice, id3dGraphicsCommandList, texName, width, height, format, resourceFlag, resourceState, srvDimension)
{
	id3dDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Tex2D(format, width, height),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(m_ID3DReadBackTexture.GetAddressOf())
	);

	m_TextureName = texName;

	m_D3DUAVDesc = DESCFACTORY->UAVDesc(m_ID3DTexture->GetDesc(), uavDimension);
	m_D3DSRVDesc = DESCFACTORY->SRVDesc(m_ID3DReadBackTexture->GetDesc(), srvDimension);

	m_bISUnorderedAccessBuffer = true;
}

UnorderedAccessBuffer::UnorderedAccessBuffer(ID3D12Resource* buffer, const std::wstring& texName, D3D12_UAV_DIMENSION uavDimension)
{
	m_ID3DTexture.Swap(buffer);
	m_TextureName = texName;

	m_D3DUAVDesc = DESCFACTORY->UAVDesc(m_ID3DTexture->GetDesc(), uavDimension);

	m_bISUnorderedAccessBuffer = true;
}

D3D12_UNORDERED_ACCESS_VIEW_DESC UnorderedAccessBuffer::UAVDesc() const
{
	return m_D3DUAVDesc;
}

void UnorderedAccessBuffer::ReadBackUABuffer(ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	D3DUtil::ChangeResourceState(m_ID3DTexture.Get(), id3dGraphicsCommandList, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_SOURCE);
	
	id3dGraphicsCommandList->CopyResource(m_ID3DReadBackTexture.Get(), m_ID3DTexture.Get());

	D3DUtil::ChangeResourceState(m_ID3DTexture.Get(), id3dGraphicsCommandList, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_COMMON);
}


/////////////////////////////////////////////////////////

TextureRootInfo::TextureRootInfo(std::shared_ptr<Texture> texture, UINT rootSignautreIndex)
{
	m_Texture = texture;
	m_rootSignatureIndex = rootSignautreIndex;
}

std::shared_ptr<Texture> TextureRootInfo::Data()
{
	return m_Texture;
}

ID3D12Resource* TextureRootInfo::Resource()
{
	return m_Texture->Resource();
}

ID3D12Resource* TextureRootInfo::ReadBackResource()
{
	return m_Texture->ReadBackResource();
}

D3D12_RESOURCE_DESC TextureRootInfo::ResourceDesc()
{
	return m_Texture->ResourceDesc();
}

D3D12_RESOURCE_DESC TextureRootInfo::ReadBackResourceDesc()
{
	return m_Texture->ResourceDesc();
}

D3D12_GPU_VIRTUAL_ADDRESS TextureRootInfo::GPUVirtualAddress()
{
	return m_Texture->GPUVirtualAddress();
}

D3D12_SHADER_RESOURCE_VIEW_DESC TextureRootInfo::SRVDesc() const
{
	return m_Texture->SRVDesc();
}

D3D12_UNORDERED_ACCESS_VIEW_DESC TextureRootInfo::UAVDesc() const
{
	return m_Texture->UAVDesc();
}

CD3DX12_CPU_DESCRIPTOR_HANDLE TextureRootInfo::CPUHandle() const
{
	return m_CPUDescHandle;
}

CD3DX12_GPU_DESCRIPTOR_HANDLE TextureRootInfo::GPUHandle() const
{
	return m_GPUDescHandle;
}

void TextureRootInfo::CreateSRV(ID3D12Device* id3dDevice, const CD3DX12_CPU_DESCRIPTOR_HANDLE& cpuHandle, const CD3DX12_GPU_DESCRIPTOR_HANDLE& gpuHandle)
{
	if(m_Texture->IsUnorderedAccessBuffer())
		id3dDevice->CreateShaderResourceView(ReadBackResource(), &SRVDesc(), cpuHandle);
	else
		id3dDevice->CreateShaderResourceView(Resource(), &SRVDesc(), cpuHandle);

	m_CPUDescHandle = cpuHandle;
	m_GPUDescHandle = gpuHandle;
}

void TextureRootInfo::CreateUAV(ID3D12Device* id3dDevice, const CD3DX12_CPU_DESCRIPTOR_HANDLE& cpuHandle, const CD3DX12_GPU_DESCRIPTOR_HANDLE& gpuHandle)
{
	id3dDevice->CreateUnorderedAccessView(Resource(), nullptr, &UAVDesc(), cpuHandle);

	m_CPUDescHandle = cpuHandle;
	m_GPUDescHandle = gpuHandle;
}

void TextureRootInfo::RefreshSRV(ID3D12Device* id3dDevice)
{
	if (m_Texture->IsUnorderedAccessBuffer())
		id3dDevice->CreateShaderResourceView(ReadBackResource(), &SRVDesc(), m_CPUDescHandle);
	else
		id3dDevice->CreateShaderResourceView(Resource(), &SRVDesc(), m_CPUDescHandle);
}

void TextureRootInfo::RefreshUAV(ID3D12Device* id3dDevice)
{
	id3dDevice->CreateUnorderedAccessView(Resource(), nullptr, &UAVDesc(), m_CPUDescHandle);
}

void TextureRootInfo::RefreshTexture(ID3D12Device* id3dDevice, std::shared_ptr<Texture> texture)
{
	if(m_Texture != nullptr)
		m_Texture.reset();

	m_Texture = texture;

	if (m_Texture->IsUnorderedAccessBuffer())
		RefreshUAV(id3dDevice);

	RefreshSRV(id3dDevice);
}

void TextureRootInfo::UpdateInfo(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	id3dGraphicsCommandList->SetGraphicsRootDescriptorTable(m_rootSignatureIndex, m_GPUDescHandle);
}

void TextureRootInfo::UpdateUAVInfo(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	if (m_Texture->IsUnorderedAccessBuffer())
		id3dGraphicsCommandList->SetComputeRootDescriptorTable(m_rootSignatureIndex, m_GPUDescHandle);
}
