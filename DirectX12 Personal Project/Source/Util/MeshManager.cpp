#include "MeshManager.h"

MeshManager::MeshManager()
{
}


MeshManager::~MeshManager()
{
}

MeshManager* MeshManager::Instance()
{
	static MeshManager meshManager;
	return &meshManager;
}

std::vector<std::shared_ptr<Mesh>> MeshManager::GetMeshVector(std::vector<std::wstring>& mesheNames)
{
	std::vector<std::shared_ptr<Mesh>> result;

	for (auto p = mesheNames.begin(); p != mesheNames.end(); ++p)
		if (m_Meshes[*p] != nullptr)
			result.push_back(m_Meshes[*p]);

	return result;
}

std::shared_ptr<Mesh> MeshManager::LoadMesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& path, const std::wstring& meshName, const std::wstring& extension, bool onlyloadOne)
{

#ifdef _DEBUG
	std::wcout << L"Successfully LoadFile : " << path.c_str() << std::endl;
#endif

	if (onlyloadOne && m_Meshes[meshName] != nullptr)
		m_Meshes.erase(m_LatelyLoadMeshName);

	if (m_Meshes[meshName] == nullptr)
		m_Meshes[meshName] = std::make_shared<Mesh>(id3dDevice, id3dGraphicsCommandList, path, meshName, extension);

	m_LatelyLoadMeshName = meshName;

	return m_Meshes[meshName];
}

std::shared_ptr<Mesh> MeshManager::GetMesh(const std::wstring& meshName)
{
	return m_Meshes[meshName];
}

std::shared_ptr<Mesh> MeshManager::GetLatelyMesh()
{
	if (m_LatelyLoadMeshName.length() == 0)
		return nullptr;

	return m_Meshes[m_LatelyLoadMeshName];
}
