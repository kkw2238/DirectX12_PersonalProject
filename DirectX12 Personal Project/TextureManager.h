#pragma once
#include "Texture.h"

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

public:
	static TextureManager* Instance();
	std::shared_ptr<Texture> GetTexture(
		ID3D12Device* id3dDevice, 
		ID3D12GraphicsCommandList* id3dGraphicsCommandList, 
		const std::wstring& fileName, 
		const std::wstring& textureName, 
		DDS_ALPHA_MODE alphaMode, 
		bool isCubeMap
	);

protected:
	std::unordered_map<std::wstring, std::shared_ptr<Texture>> m_Textures;
};

#define TEXMANAGER TextureManager::Instance()
