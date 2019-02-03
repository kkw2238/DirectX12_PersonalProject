#include "Texture.h"
#include "D3DDescriptorFactory.h"


Texture::Texture()
{
}

Texture::Texture(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& fileName, DDS_ALPHA_MODE alphaMode, bool isCubeMap, D3D12_SRV_DIMENSION srvDimension)
{
	ThrowIfFail(CreateDDSTextureFromFile12(id3dDevice, id3dGraphicsCommandList, fileName.c_str(), m_ID3DTexture, m_ID3DTextureUploadBuffer, 0, &alphaMode));

	m_D3DResouceViewDesc = DESCFACTORY->SRVResourceViewDesc(m_ID3DTexture->GetDesc(), srvDimension);
}

Texture::Texture(ID3D12Resource* texture, D3D12_SRV_DIMENSION srvDimension)
{
	m_ID3DTexture = texture;

	m_D3DResouceViewDesc = DESCFACTORY->SRVResourceViewDesc(m_ID3DTexture->GetDesc(), srvDimension);
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
	return m_D3DResouceViewDesc;
}

/////////////////////////////////////////////////////////

TextureRootInfo::TextureRootInfo(std::shared_ptr<Texture> texture, UINT rootSignautreIndex)
{
	m_Texture = texture;
	m_rootSignatureIndex = rootSignautreIndex;
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

void TextureRootInfo::SetDescriptorHandle(CD3DX12_CPU_DESCRIPTOR_HANDLE& cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE& gpuHandle)
{
	m_CPUDescHandle = cpuHandle;
	m_GPUDescHandle = gpuHandle;
}

void TextureRootInfo::UpdateInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	id3dGraphicsCommandList->SetGraphicsRootDescriptorTable(m_rootSignatureIndex, m_GPUDescHandle);
}
