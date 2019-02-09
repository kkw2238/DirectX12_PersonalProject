#include "Mesh.h"
#include "Structures.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::wstring& fildname)
{
}

Mesh::~Mesh()
{
}

void Mesh::SetCubeMesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, float width, float height, float depth)
{
	m_nVerticesCount = 24;
	m_nIndicesCount = 36;
	m_nStartSlot = 0;
	
	std::vector<UINT> indexInfo{
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

	std::vector<Vector3> tangents(m_nVerticesCount);

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

	CreateTangentVectors(vertices, normals, texCoords, indexInfo, m_nIndicesCount / 3, tangents);

	std::array<IA_TEXTURE_OBJ, 24> vertexInfo;

	for (UINT i = 0; i < m_nVerticesCount; ++i) {
		vertexInfo[i] = IA_TEXTURE_OBJ(vertices[i], texCoords[i], normals[i], tangents[i]);
	}
	
	m_ID3DVertexBuffer = D3DUtil::CreateDefaultBuffer(id3dDevice, id3dGraphicsCommandList, vertexInfo.data(), sizeof(IA_TEXTURE_OBJ) * m_nVerticesCount, m_ID3DVertexUploadBuffer);
	m_D3DVertexBufferView.BufferLocation = m_ID3DVertexBuffer->GetGPUVirtualAddress();
	m_D3DVertexBufferView.StrideInBytes = sizeof(IA_TEXTURE_OBJ);
	m_D3DVertexBufferView.SizeInBytes = m_D3DVertexBufferView.StrideInBytes * m_nVerticesCount;

	m_ID3DIndexBuffer = D3DUtil::CreateDefaultBuffer(id3dDevice, id3dGraphicsCommandList, indexInfo.data(), sizeof(UINT) * m_nIndicesCount, m_ID3DIndexUploadBuffer);
	m_D3DIndexBufferView.BufferLocation = m_ID3DIndexBuffer->GetGPUVirtualAddress();
	m_D3DIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_D3DIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndicesCount;
}

void Mesh::DrawMesh(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT nObjectCount)
{
	id3dGraphicsCommandList->IASetPrimitiveTopology(m_PrimitiveType);
	id3dGraphicsCommandList->IASetIndexBuffer(&m_D3DIndexBufferView);
	id3dGraphicsCommandList->IASetVertexBuffers(m_nStartSlot, 1, &m_D3DVertexBufferView);

	id3dGraphicsCommandList->DrawIndexedInstanced(m_nIndicesCount, nObjectCount, 0, 0, 0);
}
