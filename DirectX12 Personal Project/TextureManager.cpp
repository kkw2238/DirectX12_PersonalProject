#include "TextureManager.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	for(auto p = m_Textures.end(); p != m_Textures.begin();)
	{
		(--p)->second.reset();
	}
}

TextureManager* TextureManager::Instance()
{
	static TextureManager texManager;
	return &texManager;
}

std::shared_ptr<Texture> TextureManager::GetTexture(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& fileName, const std::wstring& textureName, DDS_ALPHA_MODE alphaMode, bool isCubeMap)
{
	if (m_Textures[textureName] == nullptr)
		m_Textures[textureName] = std::make_shared<Texture>(id3dDevice, id3dGraphicsCommandList, fileName, alphaMode, isCubeMap);
	
	return m_Textures[textureName];
}
