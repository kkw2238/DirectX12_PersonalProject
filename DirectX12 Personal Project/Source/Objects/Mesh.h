#pragma once
#include "MathUtil.h"

struct InitIntMinusOne {
	InitIntMinusOne() {};
	InitIntMinusOne(const int newi) { i = newi; }

	int i = -1;

	bool operator==(const int other) const {
		return i == other;
	}
};


class Bones
{
public:
	Bones();
	~Bones();

public:
	void SetInvRootMatrix(Matrix4x4 invRootMatrix);
	void InsertBoneData(std::string boneName, int boneIndex, Matrix4x4 boneOffsetMatrix);

	int BoneCount() const { return m_BoneCount; }
	Matrix4x4* MatrixesData() { return m_BoneOffsetMatrixes.data(); }
	Matrix4x4 InvRootMatrix() const { return m_InvRootMatrix; }
protected:
	unsigned int m_BoneCount;
	Matrix4x4 m_InvRootMatrix;

	std::vector<Matrix4x4> m_BoneOffsetMatrixes;
	std::map<std::string, InitIntMinusOne> m_BoneNameNumbering;
};


class Mesh
{
public:
	Mesh();
	Mesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& path, const std::wstring& meshName, const std::wstring& extension);
	Mesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, std::vector<Vector3>& vertices, std::vector<Vector3>& normals, std::vector<Vector3>& tangents, std::vector<Vector2>& texCoords, std::vector<UINT>& indices, std::vector<UINT>& matindices, float correctionY = 0.0f);
	~Mesh();

public:
	void DrawMesh(ID3D12GraphicsCommandList* id3dCommandList, UINT nObjectCount);
	void ProcessNode(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, aiMesh* mesh, const aiScene* scene);
	void DrawMeshes(ID3D12GraphicsCommandList* id3dCommandList, UINT nObjectCount);

	void SetPlaneMesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, float width, float depth, UINT matIndex);
	void SetCubeMesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, float width, float height, float depth, UINT matIndex);
	void CreateMesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, std::vector<Vector3>& vertices, std::vector<Vector3>& normal, std::vector<Vector3>& tangents, std::vector<Vector2>& texCoords, std::vector<UINT>& indices, std::vector<UINT>& matindices);

	float CorrectionY() const;

	Bones* BonesData() { return &m_Bones; }

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

	float m_fCorrectionY = 0.0f;

<<<<<<< HEAD
	Bones m_Bones;

=======
>>>>>>> parent of ee99c41... 애니메이션 작업중
	std::vector<Mesh> m_ChildMeshes;
	std::wstring m_MeshName;
};
