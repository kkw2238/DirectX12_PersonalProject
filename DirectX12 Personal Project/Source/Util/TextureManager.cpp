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

std::shared_ptr<Texture> TextureManager::CreateTexture(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& texName, UINT width, UINT height, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS resourceFlag, D3D12_RESOURCE_STATES resourceState)
{
	ComPtr<ID3D12Resource> resource;

	id3dDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Tex2D(format, width, height, 1, 0, 1, 0, resourceFlag),
		resourceState,
		nullptr,
		IID_PPV_ARGS(resource.GetAddressOf())
	);

	if (resourceFlag == D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS) {
		ComPtr<ID3D12Resource> readBackResource;
		id3dDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Tex2D(format, width, height),
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(readBackResource.GetAddressOf())
		);

		AddUnorderedAccessBuffer(id3dDevice, resource.Get(), texName, D3D12_UAV_DIMENSION_TEXTURE2D);
		AddTexture(id3dDevice, readBackResource.Get(), texName + L"RB");
	}

	else
		AddTexture(id3dDevice, resource.Get(), texName, D3D12_SRV_DIMENSION_TEXTURE2D);

	return m_Textures[texName];
}

std::shared_ptr<Texture> TextureManager::CreateBuffer(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& bufferName, UINT width, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS resourceFlag, D3D12_RESOURCE_STATES resourceState)
{
	ComPtr<ID3D12Resource> resource;

	id3dDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(width, resourceFlag),
		resourceState,
		nullptr,
		IID_PPV_ARGS(resource.GetAddressOf())
	);

	if (resourceFlag == D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS) {
		ComPtr<ID3D12Resource> readBackResource;
		id3dDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(width),
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(resource.GetAddressOf())
		);

		AddUnorderedAccessBuffer(id3dDevice, resource.Get(), bufferName, D3D12_UAV_DIMENSION_BUFFER);
		AddTexture(id3dDevice, readBackResource.Get(), bufferName + L"RB");
	}

	else
		AddTexture (id3dDevice, resource.Get(), bufferName, D3D12_SRV_DIMENSION_BUFFER);
	
	return m_Textures[bufferName];
}

std::shared_ptr<Texture> TextureManager::LoadTexture(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& fileName, const std::wstring& textureName, DDS_ALPHA_MODE alphaMode, bool isCubeMap)
{

	std::shared_ptr<Texture>originalData = nullptr;

	if (m_Textures[textureName] != nullptr)
		originalData = std::move(m_Textures[textureName]);

	if (m_Textures[textureName] == nullptr) {
		std::shared_ptr<Texture> makedTexture = std::make_shared<Texture>(id3dDevice, id3dGraphicsCommandList, textureName, fileName, alphaMode, isCubeMap);

		if (makedTexture->Name().length() > 0) {
			m_Textures[textureName] = std::move(makedTexture);
#ifdef _DEBUG
			std::wcout << L"Successfully LoadFile : " << fileName.c_str() << std::endl;
#endif
		}
		else {
#ifdef _DEBUG
			std::wcout << L"Not Found File : " << fileName.c_str() << std::endl;
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

void TextureManager::AddUnorderedAccessBuffer(ID3D12Device* id3dDevice, ID3D12Resource* UABuffer, const std::wstring& textureName, D3D12_UAV_DIMENSION uavDimension)
{
	if (m_Textures[textureName])
		m_Textures[textureName].reset();

	m_Textures[textureName] = std::make_shared<UnorderedAccessBuffer>(UABuffer, textureName, uavDimension);

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
