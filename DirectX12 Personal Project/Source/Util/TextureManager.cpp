#include "TextureManager.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	for(auto p = m_Textures.end(); p != m_Textures.begin();)
		(--p)->second.reset();
}

TextureManager* TextureManager::Instance()
{
	static TextureManager texManager;
	return &texManager;
}

std::shared_ptr<Texture> TextureManager::LoadTexture(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& fileName, const std::wstring& textureName, DDS_ALPHA_MODE alphaMode, bool isCubeMap)
{
	if (m_Textures[textureName] == nullptr)
		m_Textures[textureName] = std::make_shared<Texture>(id3dDevice, id3dGraphicsCommandList, fileName, alphaMode, isCubeMap);
	
	return m_Textures[textureName];
}

void TextureManager::AddTexture(ID3D12Resource* texture, const std::wstring& textureName, D3D12_SRV_DIMENSION srvDimension)
{
	m_Textures[textureName] = std::make_shared<Texture>(texture, srvDimension);
}

std::vector<std::shared_ptr<Texture>>* TextureManager::GetTextureVector(std::vector<std::wstring>& textureNames)
{
	std::vector<std::shared_ptr<Texture>> result;

	for (auto p = textureNames.begin(); p != textureNames.end(); ++p)
		if (m_Textures[*p] != nullptr)
			result.push_back(m_Textures[*p]);

	return &result;
}

std::shared_ptr<Texture> TextureManager::GetTexture(const std::wstring& textureName)
{
	return m_Textures[textureName];
}
