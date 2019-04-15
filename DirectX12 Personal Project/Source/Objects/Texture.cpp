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

Texture::Texture(ID3D12Resource* texture, const std::wstring& texName, D3D12_SRV_DIMENSION srvDimension)
{
	m_ID3DTexture = texture;
	m_TextureName = texName;

	m_D3DSRVDesc = DESCFACTORY->SRVDesc(m_ID3DTexture->GetDesc(), srvDimension);
}

Texture::~Texture()
{
}

ID3D12Resource* Texture::Resource()
{
	return m_ID3DTexture.Get();
}

D3D12_RESOURCE_DESC Texture::ResouceDesc()
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

UnorderedAccessBuffer::UnorderedAccessBuffer(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& texName, UINT elementSize, UINT width, UINT height, D3D12_SRV_DIMENSION srvDimension, D3D12_UAV_DIMENSION uavDimension)
{
	m_ID3DTexture = D3DUtil::CreateDefaultBuffer(id3dDevice, id3dGraphicsCommandList, nullptr, static_cast<UINT64>(elementSize) * static_cast<UINT64>(width) * static_cast<UINT64>(height), m_ID3DTextureUploadBuffer);

	m_TextureName = texName;
	m_ID3DTexture->GetDesc();

	m_D3DSRVDesc = DESCFACTORY->SRVDesc(m_ID3DTexture->GetDesc(), srvDimension);
	m_D3DUAVDesc = DESCFACTORY->UAVDesc(m_ID3DTexture->GetDesc(), uavDimension);

	m_bISUnorderedAccessBuffer = true;
}

UnorderedAccessBuffer::UnorderedAccessBuffer(ID3D12Resource* buffer, const std::wstring& texName, D3D12_UAV_DIMENSION uavDimension)
{
	m_ID3DTexture = buffer;
	m_TextureName = texName;

	m_D3DUAVDesc = DESCFACTORY->UAVDesc(m_ID3DTexture->GetDesc(), uavDimension);

	m_bISUnorderedAccessBuffer = true;
}

D3D12_UNORDERED_ACCESS_VIEW_DESC UnorderedAccessBuffer::UAVDesc() const
{
	return m_D3DUAVDesc;
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

D3D12_RESOURCE_DESC TextureRootInfo::ResouceDesc()
{
	return m_Texture->ResouceDesc();
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
	else
		RefreshSRV(id3dDevice);
}

void TextureRootInfo::UpdateInfo(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	id3dGraphicsCommandList->SetGraphicsRootDescriptorTable(m_rootSignatureIndex, m_GPUDescHandle);
}
