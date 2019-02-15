#include "Mesh.h"
#include "Structures.h"
#include <fstream>

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
		aiProcess_ConvertToLeftHanded);

	importer.GetImporterIndex(extensionString.c_str());

	ProcessNode(id3dDevice, id3dGraphicsCommandList, scene->mRootNode, scene);

	m_MeshName = meshName;
}

Mesh::Mesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, std::vector<Vector3>& vertices, std::vector<Vector3>& normals, std::vector<Vector3>& tangents, std::vector<Vector2>& texCoords, std::vector<UINT>& indices)
{
	CreateMesh(id3dDevice, id3dGraphicsCommandList, vertices, normals, tangents, texCoords, indices);
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

void Mesh::CreateMesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, std::vector<Vector3>& vertices, std::vector<Vector3>&normals, std::vector<Vector3>& tangents, std::vector<Vector2>& texCoords, std::vector<UINT>& indices)
{
	m_nVerticesCount = vertices.size();
	m_nIndicesCount = indices.size();
	m_nStartSlot = 0;

	std::vector<IA_TEXTURE_OBJ> vertexInfo(m_nVerticesCount);
	
	if (normals.size() == 0)
		CreateNormalVectors(vertices, normals);

	if (tangents.size() == 0)
		CreateTangentVectors(vertices, normals, texCoords, indices, m_nVerticesCount / 3, tangents);

	for (UINT i = 0; i < m_nVerticesCount; ++i) {
		vertexInfo[i] = IA_TEXTURE_OBJ(vertices[i], texCoords[i], normals[i], tangents[i]);
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
	std::vector<UINT> indices;

	std::fstream vertexfStream		= std::fstream("Vertex.txt");
	std::fstream normalfStream		= std::fstream("Normal.txt");
	std::fstream UVfStream			= std::fstream("UV.txt");
	std::fstream indexfStream		= std::fstream("Indices.txt");

	for (size_t verticesIndex = 0; verticesIndex < mesh->mNumVertices; ++verticesIndex) {
		aiVector3D vertex;
		aiVector3D texCoord;
		aiVector3D tangent;
		aiVector3D normal;

		if (mesh->mVertices != nullptr) {
			vertex = mesh->mVertices[verticesIndex];
			vertices.emplace_back(vertex.x, vertex.y, vertex.z);
			vertexfStream << verticesIndex << " : \t" << Vector3(vertex.x, vertex.y, vertex.z) << std::endl;
		}
		if (mesh->mTextureCoords[0] != nullptr) {
			texCoord = mesh->mTextureCoords[0][verticesIndex];
			texCoords.emplace_back(texCoord.x, texCoord.y);
			UVfStream << verticesIndex << " : \t" << Vector2(texCoord.x, texCoord.y) << std::endl;
		}
		if (mesh->mTangents != nullptr) {
			tangent = mesh->mTangents[verticesIndex];
			tangents.emplace_back(tangent.x, tangent.y, tangent.z);
		}
		if (mesh->mNormals != nullptr) {
			normal = mesh->mNormals[verticesIndex];
			normals.emplace_back(normal.x, normal.y, normal.z);
			normalfStream << verticesIndex << " : \t" << Vector3(normal.x, normal.y, normal.z) << std::endl;
		}
	}

	for (size_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
		aiFace face = mesh->mFaces[faceIndex];

		for (size_t indicesIndex = 0; indicesIndex < face.mNumIndices; ++indicesIndex) {
			indices.push_back(face.mIndices[indicesIndex]);
			indexfStream << indicesIndex << " : \t" << face.mIndices[indicesIndex] << std::endl;
		}
	}

	vertexfStream.close();
	normalfStream.close();
	UVfStream.close();
	indexfStream.close();

	return Mesh(id3dDevice, id3dGraphicsCommandList, vertices, normals, tangents, texCoords, indices);
}


void Mesh::SetPlaneMesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, float width, float depth)
{
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

	std::vector<Vector2> texCoords{
		Vector2(0.0f, 0.0f),
		Vector2(1.0f, 0.0f),
		Vector2(0.0f, 1.0f),
		Vector2(1.0f, 1.0f)
	};

	m_ChildMeshes.emplace_back(id3dDevice, id3dGraphicsCommandList, vertices, normals, tangents, texCoords, indices);
}

void Mesh::SetCubeMesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, float width, float height, float depth)
{
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

	m_ChildMeshes.emplace_back(id3dDevice, id3dGraphicsCommandList, vertices, normals, tangents, texCoords, indices);
}
