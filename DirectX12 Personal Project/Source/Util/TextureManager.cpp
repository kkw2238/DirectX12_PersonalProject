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
#ifdef _DEBUG
	std::wcout << L"Successfully LoadFile : " << fileName.c_str() << std::endl;
#endif

	std::shared_ptr<Texture>originalData = nullptr;

	if (m_Textures[textureName] != nullptr)
		originalData = std::move(m_Textures[textureName]);

	if (m_Textures[textureName] == nullptr) {
		std::shared_ptr<Texture> makedTexture = std::make_shared<Texture>(id3dDevice, id3dGraphicsCommandList, textureName, fileName, alphaMode, isCubeMap);

		if (makedTexture->Name().length() > 0)
			m_Textures[textureName] = std::move(makedTexture);

		else {
#ifdef _DEBUG
			std::wcout << L"Not Found NRMFile : " << fileName.c_str() << std::endl;
#endif
			m_Textures[textureName] = std::move(originalData);
			return nullptr;
		}
	}
	RefreshLinkedTexture(id3dDevice, textureName);

	return m_Textures[textureName];
}

void TextureManager::LinkedTexture(const std::wstring& textureName, TextureRootInfo* rootInfo)
{
	m_LinkedTextures[textureName].emplace_back(std::make_unique<TextureRootInfo*>(rootInfo));
}

void TextureManager::AddTexture(ID3D12Device* id3dDevice, ID3D12Resource* texture, const std::wstring& textureName, D3D12_SRV_DIMENSION srvDimension)
{
	if (m_Textures[textureName])
		m_Textures[textureName].reset();

	m_Textures[textureName] = std::make_shared<Texture>(texture, textureName, srvDimension);
	
	RefreshLinkedTexture(id3dDevice, textureName);
}

void TextureManager::RefreshLinkedTexture(ID3D12Device* id3dDevice, const std::wstring& textureName)
{
	for (size_t i = 0; i != m_LinkedTextures[textureName].size(); ++i)
		(*(m_LinkedTextures[textureName])[i])->RefreshTexture(id3dDevice, m_Textures[textureName]);
}

std::vector<std::shared_ptr<Texture>> TextureManager::GetTextureVector(std::vector<std::wstring>& textureNames)
{
	std::vector<std::shared_ptr<Texture>> result;

	for (auto p = textureNames.begin(); p != textureNames.end(); ++p)
		if (m_Textures[*p] != nullptr)
			result.push_back(m_Textures[*p]);

	return result;
}

std::shared_ptr<Texture> TextureManager::GetTexture(const std::wstring& textureName)
{
	return m_Textures[textureName];
}
