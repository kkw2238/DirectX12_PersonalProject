#pragma once
#include "MathUtil.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh
{
public:
	Mesh();
	Mesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& path, const std::wstring& meshName, const std::wstring& extension);
	Mesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, std::vector<Vector3>& vertices, std::vector<Vector3>& normals, std::vector<Vector3>& tangents, std::vector<Vector2>& texCoords, std::vector<UINT>& indices);
	~Mesh();

public:
	void DrawMesh(ID3D12GraphicsCommandList* id3dCommandList, UINT nObjectCount);
	void ProcessNode(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, aiMesh* mesh, const aiScene* scene);
	void DrawMeshes(ID3D12GraphicsCommandList* id3dCommandList, UINT nObjectCount);

	void SetCubeMesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, float width, float height, float depth);
	void CreateMesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, std::vector<Vector3>& vertices, std::vector<Vector3>& normal, std::vector<Vector3>& tangents, std::vector<Vector2>& texCoords, std::vector<UINT>& indices);

	std::vector<Mesh> Get();
protected:
	ComPtr<ID3D12Resource> m_ID3DVertexBuffer = nullptr;
	ComPtr<ID3D12Resource> m_ID3DVertexUploadBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_D3DVertexBufferView;
	UINT m_nVerticesCount;

	ComPtr<ID3D12Resource> m_ID3DIndexBuffer = nullptr;
	ComPtr<ID3D12Resource> m_ID3DIndexUploadBuffer = nullptr;
	D3D12_INDEX_BUFFER_VIEW m_D3DIndexBufferView;
	UINT m_nIndicesCount;

	D3D_PRIMITIVE_TOPOLOGY m_PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT m_nStartSlot;

	std::vector<Mesh> m_ChildMeshes;
	std::wstring m_MeshName;
};
