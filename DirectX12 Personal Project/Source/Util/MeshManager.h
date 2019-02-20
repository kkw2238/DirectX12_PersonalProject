#pragma once
/*
 *	MeshManager 클래스에는 fbx로드 후 Mesh객체로 만들어 갖고 있음.
 */
#include "Mesh.h"

class MeshManager
{
public:
	MeshManager();
	~MeshManager();

public:
	static MeshManager* Instance();

	std::shared_ptr<Mesh> LoadMesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& fileName, const std::wstring& meshName, const std::wstring& extension, bool onlyloadOne);
	std::shared_ptr<Mesh> GetMesh(const std::wstring& meshName);
	std::shared_ptr<Mesh> GetLatelyMesh();
	std::vector<std::shared_ptr<Mesh>> GetMeshVector(std::vector<std::wstring>& mesheNames);

	void SetMesh(Mesh* mesh, const std::wstring meshName);
protected:
	std::unordered_map<std::wstring, std::shared_ptr<Mesh>> m_Meshes;

	std::wstring m_LatelyLoadMeshName;
};

#define MESHMANAGER MeshManager::Instance()