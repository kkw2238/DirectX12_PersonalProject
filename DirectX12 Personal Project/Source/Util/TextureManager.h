#pragma once
#include "Texture.h"

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

public:
	static TextureManager* Instance();
	std::shared_ptr<Texture> CreateTexture(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& texName, UINT width, UINT height, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS resourceFlag, D3D12_RESOURCE_STATES resourceState);
	std::shared_ptr<Texture> CreateBuffer(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& bufferName, UINT width, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS resourceFlag, D3D12_RESOURCE_STATES resourceState);

	std::shared_ptr<Texture> LoadTexture(
		ID3D12Device* id3dDevice, 
		ID3D12GraphicsCommandList* id3dGraphicsCommandList, 
		const std::wstring& fileName, 
		const std::wstring& textureName, 
		DDS_ALPHA_MODE alphaMode, 
		bool isCubeMap
	);

	void LinkedTexture(const std::wstring& textureName, TextureRootInfo* rootInfo);
	void AddTexture(ID3D12Device* id3dDevice, ID3D12Resource* texture, const std::wstring& textureName, D3D12_SRV_DIMENSION srvDimension = D3D12_SRV_DIMENSION_TEXTURE2D);
	void AddTexture(ID3D12Device* id3dDevice, Texture& newTexture);
	void AddUnorderedAccessBuffer(ID3D12Device* id3dDevice, UnorderedAccessBuffer& newTexture);
	void RefreshLinkedTexture(ID3D12Device* id3dDevice, const std::wstring& textureName);

	std::vector<std::shared_ptr<Texture>> GetTextureVector(std::vector<std::wstring>& textureNames);
	std::shared_ptr<Texture> GetTexture(const std::wstring& textureName);

protected:
	std::unordered_map<std::wstring, std::shared_ptr<Texture>> m_Textures;
	std::unordered_map<std::wstring, std::vector<std::shared_ptr<TextureRootInfo*>>> m_LinkedTextures;
};

#define TEXMANAGER TextureManager::Instance()