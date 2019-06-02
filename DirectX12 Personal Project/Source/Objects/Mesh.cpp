#include "Mesh.h"
#include "Animation.h"
#include <fstream>


Bones::Bones()
{
	m_BoneOffsetMatrixes.resize(NUM_OF_BONES);
}

Bones::~Bones()
{
}

void Bones::SetInvRootMatrix(Matrix4x4 invRootMatrix)
{
	m_InvRootMatrix = invRootMatrix;
}

void Bones::InsertBoneData(float animationTime, const aiMesh* mesh, const aiNode* node, const Matrix4x4& parentMat, std::vector<VERTEX_IN_BONE>& verData)
{
	unsigned int numBones = mesh->mNumBones;

	for (size_t boneIndex = 0; boneIndex < numBones; ++boneIndex) {
		aiBone* bone = mesh->mBones[boneIndex];
		UINT numWeight = mesh->mBones[boneIndex]->mNumWeights;

		if (m_BoneNameNumbering[bone->mName.data] == -1) {
			m_BoneNameNumbering[bone->mName.data] = boneIndex;
			m_BoneOffsetMatrixes[boneIndex] = bone->mOffsetMatrix;
			++m_BoneCount;
		}
		
		for (size_t weightIndex = 0; weightIndex < numWeight; ++weightIndex) {
			aiVertexWeight weight = bone->mWeights[weightIndex];
			verData[weight.mVertexId].InputVertexInBoneData(boneIndex, weight.mWeight);
		}
	}
}

void Bones::ProcessBoneData(float animationTime, const aiNode* node, const int parentIndex)
{
	int parentID = -1;
	unsigned int numChildren = node->mNumChildren;

	
	for (int i = 0; i < numChildren; ++i) {
		std::string a = node->mChildren[i]->mName.data;
		if(m_BoneNameNumbering.find(node->mName.data) != m_BoneNameNumbering.end())

		ProcessBoneData(animationTime, node->mChildren[i], parentIndex);
	}
}

int Bones::findBoneNumber(std::string& boneName)
{
	if (m_BoneNameNumbering.find(boneName) != m_BoneNameNumbering.end())
		return m_BoneNameNumbering[boneName].i;

	return -1;
}

Mesh::Mesh()
{
}

Mesh::Mesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& path, const std::wstring& meshName, const std::wstring& extension)
{
	Assimp::Importer importer;

	std::string filenameString(path.begin(), path.end());
	std::string extensionString(extension.begin(), extension.end());

	const aiScene* scene = importer.ReadFile(filenameString.c_str(),
		aiProcess_Triangulate |						
		aiProcess_ConvertToLeftHanded				
	);

	importer.GetImporterIndex(extensionString.c_str());

	ProcessNode(id3dDevice, id3dGraphicsCommandList, scene->mRootNode, scene);

	m_MeshName = meshName;
}

Mesh::Mesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, std::vector<Vector3>& vertices, std::vector<Vector3>& normals, std::vector<Vector3>& tangents, std::vector<Vector2>& texCoords, std::vector<UINT>& indices, std::vector<UINT>& matindices, const VERTEX_IN_BONE* boneDatas, float correctionY)
{
	m_fCorrectionY = correctionY;
	CreateMesh(id3dDevice, id3dGraphicsCommandList, vertices, normals, tangents, texCoords, indices, matindices, boneDatas);
}

Mesh::~Mesh()
{
}

void Mesh::DrawMesh(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT nObjectCount)
{
	id3dGraphicsCommandList->IASetPrimitiveTopology(m_PrimitiveType);
	id3dGraphicsCommandList->IASetIndexBuffer(&m_D3DIndexBufferView);
	id3dGraphicsCommandList->IASetVertexBuffers(m_nStartSlot, 1, &m_D3DVertexBufferView);

	id3dGraphicsCommandList->DrawIndexedInstanced(m_nIndicesCount, nObjectCount, 0, 0, 0);
}

void Mesh::CreateMesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, std::vector<Vector3>& vertices, std::vector<Vector3>&normals, std::vector<Vector3>& tangents, std::vector<Vector2>& texCoords, std::vector<UINT>& indices, std::vector<UINT>& matindices, const VERTEX_IN_BONE* boneDatas)
{
	m_nVerticesCount = vertices.size();
	m_nIndicesCount = indices.size();
	m_nStartSlot = 0;

	std::vector<IA_TEXTURE_OBJ> vertexInfo(m_nVerticesCount);
	
	for (int i = 0; i < m_nVerticesCount; ++i)
		vertices[i].y -= m_fCorrectionY;

	if (normals.size() == 0)
		CreateNormalVectors(vertices, normals);

	if (tangents.size() == 0)
		CreateTangentVectors(vertices, normals, texCoords, indices, m_nIndicesCount / 3, tangents);


	for (UINT i = 0; i < m_nVerticesCount; ++i) {
		if (boneDatas != nullptr) {
			IA_TEXTURE_OBJ data = IA_TEXTURE_OBJ(vertices[i], texCoords[i], normals[i], tangents[i], matindices[i], boneDatas[i].GetData());
			vertexInfo[i] = IA_TEXTURE_OBJ(vertices[i], texCoords[i], normals[i], tangents[i], matindices[i], boneDatas[i].GetData());
		}
		else
			vertexInfo[i] = IA_TEXTURE_OBJ(vertices[i], texCoords[i], normals[i], tangents[i], matindices[i]);
	}


	m_ID3DVertexBuffer = D3DUtil::CreateDefaultBuffer(id3dDevice, id3dGraphicsCommandList, vertexInfo.data(), sizeof(IA_TEXTURE_OBJ) * m_nVerticesCount, m_ID3DVertexUploadBuffer);
	m_D3DVertexBufferView.BufferLocation = m_ID3DVertexBuffer->GetGPUVirtualAddress();
	m_D3DVertexBufferView.StrideInBytes = sizeof(IA_TEXTURE_OBJ);
	m_D3DVertexBufferView.SizeInBytes = m_D3DVertexBufferView.StrideInBytes * m_nVerticesCount;

	m_ID3DIndexBuffer = D3DUtil::CreateDefaultBuffer(id3dDevice, id3dGraphicsCommandList, indices.data(), sizeof(UINT) * m_nIndicesCount, m_ID3DIndexUploadBuffer);
	m_D3DIndexBufferView.BufferLocation = m_ID3DIndexBuffer->GetGPUVirtualAddress();
	m_D3DIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_D3DIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndicesCount;
}

float Mesh::CorrectionY() const
{
	return m_fCorrectionY;
}

std::vector<Mesh> Mesh::Get()
{
	return m_ChildMeshes;
}


void Mesh::DrawMeshes(ID3D12GraphicsCommandList* id3dCommandList, UINT nObjectCount)
{
	for (size_t i = 0; i < m_ChildMeshes.size(); ++i)
		m_ChildMeshes[i].DrawMesh(id3dCommandList, nObjectCount);

	//DrawMesh(id3dCommandList, nObjectCount);
}

void Mesh::ProcessNode(ID3D12Device * id3dDevice, ID3D12GraphicsCommandList * id3dGraphicsCommandList, aiNode * node, const aiScene * scene)
{
	for (size_t i = 0; i < node->mNumMeshes; ++i) {
		m_ChildMeshes.push_back(ProcessMesh(id3dDevice, id3dGraphicsCommandList, scene->mMeshes[node->mMeshes[i]], scene)); 
	}

	for (size_t i = 0; i < node->mNumChildren; ++i) {
		ProcessNode(id3dDevice, id3dGraphicsCommandList, node->mChildren[i], scene);
	}
	scene->mRootNode;
}

Mesh Mesh::ProcessMesh(ID3D12Device * id3dDevice, ID3D12GraphicsCommandList * id3dGraphicsCommandList, aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;
	std::vector<Vector3> tangents;
	std::vector<UINT>	materialIndices;
	std::vector<UINT>	indices;
	std::vector<VERTEX_IN_BONE> bonedatas;

	float correctY = 0.0f;
	UINT numBones = mesh->mNumBones;
	UINT numVertices = mesh->mNumVertices;

	for (size_t verticesIndex = 0; verticesIndex < numVertices; ++verticesIndex) {
		aiVector3D vertex;
		aiVector3D texCoord;
		aiVector3D tangent;
		aiVector3D normal;

		if (mesh->mVertices != nullptr) {
			vertex = mesh->mVertices[verticesIndex];
			vertices.emplace_back(vertex.x, vertex.y, vertex.z);
			if (vertex.y < correctY)
				correctY = vertex.y;
		}
		if (mesh->mTextureCoords[0] != nullptr) {
			texCoord = mesh->mTextureCoords[0][verticesIndex];
			texCoords.emplace_back(texCoord.x, texCoord.y);
		}
		if (mesh->mTangents != nullptr) {
			tangent = mesh->mTangents[verticesIndex];
			tangents.emplace_back(tangent.x, tangent.y, tangent.z);
		}
		if (mesh->mNormals != nullptr) {
			normal = mesh->mNormals[verticesIndex];
			normals.emplace_back(normal.x, normal.y, normal.z);
		}
		materialIndices.emplace_back(mesh->mMaterialIndex);
	}

	if (mesh->mBones != nullptr) {
		Matrix4x4 mat;
		bonedatas.resize(numVertices);
		m_Bones.SetInvRootMatrix(Matrix4x4(scene->mRootNode->mTransformation).Inverse());
		m_Bones.InsertBoneData(60.0f, mesh, scene->mRootNode, mat, bonedatas);

		for (size_t boneIndex = 0; boneIndex < numBones; ++boneIndex) {
			aiBone* bone = mesh->mBones[boneIndex];
			UINT numWeight = mesh->mBones[boneIndex]->mNumWeights;

			for (size_t weightIndex = 0; weightIndex < numWeight; ++weightIndex) {
				aiVertexWeight weight = bone->mWeights[weightIndex];
				bonedatas[weight.mVertexId].InputVertexInBoneData(boneIndex, weight.mWeight);
			}
		}
	}

	for (size_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
		aiFace face = mesh->mFaces[faceIndex];

		for (size_t indicesIndex = 0; indicesIndex < face.mNumIndices; ++indicesIndex) {
			indices.push_back(face.mIndices[indicesIndex]);
		}
	}

	return Mesh(id3dDevice, id3dGraphicsCommandList, vertices, normals, tangents, texCoords, indices, materialIndices, bonedatas.data(), correctY);
}


void Mesh::SetPlaneMesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, float width, float depth, UINT matIndex)
{
	std::vector<UINT> boneIndices;
	std::vector<float> weights;

	UINT nVerticesCount = 4;
	UINT nIndicesCount = 6;
	UINT nStartSlot = 0;

	std::vector<UINT> indices{
		0, 2, 1,
		3, 1, 2
	};

	std::vector<Vector3> tangents(nVerticesCount);

	std::vector<Vector3> normals{
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f)
	};

	std::vector<Vector3> vertices{
		Vector3(-width / 2.0f, 0.0f, -depth / 2.0f),
		Vector3(+width / 2.0f, 0.0f, -depth / 2.0f),
	
		Vector3(-width / 2.0f, 0.0f, +depth / 2.0f),
		Vector3(+width / 2.0f, 0.0f, +depth / 2.0f)
	};

	std::vector<UINT> matindices(nVerticesCount, matIndex);

	std::vector<Vector2> texCoords{
		Vector2(0.0f, 0.0f),
		Vector2(1.0f, 0.0f),
		Vector2(0.0f, 1.0f),
		Vector2(1.0f, 1.0f)
	};

	m_ChildMeshes.emplace_back(id3dDevice, id3dGraphicsCommandList, vertices, normals, tangents, texCoords, indices, matindices);
}

void Mesh::SetCubeMesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, float width, float height, float depth, UINT matIndex)
{
	std::vector<UINT> boneIndices;
	std::vector<float> weights;

	UINT nVerticesCount = 24;
	UINT nIndicesCount = 36;
	UINT nStartSlot = 0;

	std::vector<UINT> indices{
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		20, 21, 22,
		20, 22, 23
	};

	std::vector<Vector3> tangents(nVerticesCount);
	std::vector<UINT> matindices(nVerticesCount, matIndex);

	std::vector<Vector3> normals{
		Vector3(0.0f, 0.0f, -1.0f),
		Vector3(0.0f, 0.0f, -1.0f),
		Vector3(0.0f, 0.0f, -1.0f),
		Vector3(0.0f, 0.0f, -1.0f),

		Vector3(0.0f, 0.0f, 1.0f),
		Vector3(0.0f, 0.0f, 1.0f),
		Vector3(0.0f, 0.0f, 1.0f),
		Vector3(0.0f, 0.0f, 1.0f),

		Vector3(+0.0f, +1.0f, 0.0f),
		Vector3(+0.0f, +1.0f, 0.0f),
		Vector3(+0.0f, +1.0f, 0.0f),
		Vector3(+0.0f, +1.0f, 0.0f),

		Vector3(+0.0f, -1.0f, 0.0f),
		Vector3(+0.0f, -1.0f, 0.0f),
		Vector3(+0.0f, -1.0f, 0.0f),
		Vector3(+0.0f, -1.0f, 0.0f),

		Vector3(-1.0f, +0.0f, 0.0f),
		Vector3(-1.0f, +0.0f, 0.0f),
		Vector3(-1.0f, +0.0f, 0.0f),
		Vector3(-1.0f, +0.0f, 0.0f),

		Vector3(+1.0f, +0.0f, 0.0f),
		Vector3(+1.0f, +0.0f, 0.0f),
		Vector3(+1.0f, +0.0f, 0.0f),
		Vector3(+1.0f, +0.0f, 0.0f)
	};

	std::vector<Vector3> vertices{
		Vector3(-width / 2.0f, -height / 2.0f, -depth / 2.0f),
		Vector3(-width / 2.0f, +height / 2.0f, -depth / 2.0f),
		Vector3(+width / 2.0f, +height / 2.0f, -depth / 2.0f),
		Vector3(+width / 2.0f, -height / 2.0f, -depth / 2.0f),

		Vector3(-width / 2.0f, -height / 2.0f, +depth / 2.0f),
		Vector3(+width / 2.0f, -height / 2.0f, +depth / 2.0f),
		Vector3(+width / 2.0f, +height / 2.0f, +depth / 2.0f),
		Vector3(-width / 2.0f, +height / 2.0f, +depth / 2.0f),

		Vector3(-width / 2.0f, +height / 2.0f, -depth / 2.0f),
		Vector3(-width / 2.0f, +height / 2.0f, +depth / 2.0f),
		Vector3(+width / 2.0f, +height / 2.0f, +depth / 2.0f),
		Vector3(+width / 2.0f, +height / 2.0f, -depth / 2.0f),

		Vector3(-width / 2.0f, -height / 2.0f, -depth / 2.0f),
		Vector3(+width / 2.0f, -height / 2.0f, -depth / 2.0f),
		Vector3(+width / 2.0f, -height / 2.0f, +depth / 2.0f),
		Vector3(-width / 2.0f, -height / 2.0f, +depth / 2.0f),

		Vector3(-width / 2.0f, -height / 2.0f, +depth / 2.0f),
		Vector3(-width / 2.0f, +height / 2.0f, +depth / 2.0f),
		Vector3(-width / 2.0f, +height / 2.0f, -depth / 2.0f),
		Vector3(-width / 2.0f, -height / 2.0f, -depth / 2.0f),

		Vector3(+width / 2.0f, -height / 2.0f, -depth / 2.0f),
		Vector3(+width / 2.0f, +height / 2.0f, -depth / 2.0f),
		Vector3(+width / 2.0f, +height / 2.0f, +depth / 2.0f),
		Vector3(+width / 2.0f, -height / 2.0f, +depth / 2.0f),
	};

	std::vector<Vector2> texCoords{
		Vector2(0.0f, 1.0f),
		Vector2(0.0f, 0.0f),
		Vector2(1.0f, 0.0f),
		Vector2(1.0f, 1.0f),

		Vector2(1.0f, 1.0f),
		Vector2(0.0f, 1.0f),
		Vector2(0.0f, 0.0f),
		Vector2(1.0f, 0.0f),

		Vector2(0.0f, 1.0f),
		Vector2(0.0f, 0.0f),
		Vector2(1.0f, 0.0f),
		Vector2(1.0f, 1.0f),

		Vector2(1.0f, 1.0f),
		Vector2(0.0f, 1.0f),
		Vector2(0.0f, 0.0f),
		Vector2(1.0f, 0.0f),

		Vector2(0.0f, 1.0f),
		Vector2(0.0f, 0.0f),
		Vector2(1.0f, 0.0f),
		Vector2(1.0f, 1.0f),

		Vector2(0.0f, 1.0f),
		Vector2(0.0f, 0.0f),
		Vector2(1.0f, 0.0f),
		Vector2(1.0f, 1.0f)
	};

	m_ChildMeshes.emplace_back(id3dDevice, id3dGraphicsCommandList, vertices, normals, tangents, texCoords, indices, matindices);
}
