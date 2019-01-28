#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::wstring& fildname)
{
}

Mesh::~Mesh()
{
}

void Mesh::SetCubeMesh(ComPtr<ID3D12Device> id3dDevice, ComPtr<ID3D12GraphicsCommandList> id3dCommandList, float width, float height, float depth)
{
	m_nVerticesCount = 8;
	m_nIndicesCount = 36;
	m_nStartSlot = 0;

	std::array<Vector3, 8> vertexInfo{
		Vector3(-width / 2.0f, -height / 2.0f, -depth / 2.0f),
		Vector3(-width / 2.0f, +height / 2.0f, -depth / 2.0f),
		Vector3(+width / 2.0f, +height / 2.0f, -depth / 2.0f),
		Vector3(+width / 2.0f, -height / 2.0f, -depth / 2.0f),
		Vector3(-width / 2.0f, -height / 2.0f, +depth / 2.0f),
		Vector3(-width / 2.0f, +height / 2.0f, +depth / 2.0f),
		Vector3(+width / 2.0f, +height / 2.0f, +depth / 2.0f),
		Vector3(+width / 2.0f, -height / 2.0f, +depth / 2.0f)
	};

	std::array<UINT, 36> indexInfo{
		0, 1, 2,
		0, 2, 3,

		4, 6, 5,
		4, 7, 6,

		4, 5, 1,
		4, 1, 0,

		3, 2, 6,
		3, 6, 7,

		1, 5, 6,
		1, 6, 2,

		4, 0, 3,
		4, 3, 7
	};
	
	m_ID3DVertexBuffer = D3DUtil::CreateDefaultBuffer(id3dDevice.Get(), id3dCommandList.Get(), vertexInfo.data(), sizeof(Vector3) * m_nVerticesCount, m_ID3DVertexUploadBuffer);
	m_D3DVertexBufferView.BufferLocation = m_ID3DVertexBuffer->GetGPUVirtualAddress();
	m_D3DVertexBufferView.StrideInBytes = sizeof(Vector3);
	m_D3DVertexBufferView.SizeInBytes = m_D3DVertexBufferView.StrideInBytes * m_nVerticesCount;

	m_ID3DIndexBuffer = D3DUtil::CreateDefaultBuffer(id3dDevice.Get(), id3dCommandList.Get(), indexInfo.data(), sizeof(UINT) * m_nIndicesCount, m_ID3DIndexUploadBuffer);
	m_D3DIndexBufferView.BufferLocation = m_ID3DIndexBuffer->GetGPUVirtualAddress();
	m_D3DIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_D3DIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndicesCount;
}

void Mesh::DrawMesh(ComPtr<ID3D12GraphicsCommandList> id3dCommandList, UINT nObjectCount)
{
	id3dCommandList->IASetPrimitiveTopology(m_PrimitiveType);
	id3dCommandList->IASetIndexBuffer(&m_D3DIndexBufferView);
	id3dCommandList->IASetVertexBuffers(m_nStartSlot, 1, &m_D3DVertexBufferView);

	id3dCommandList->DrawIndexedInstanced(m_nIndicesCount, nObjectCount, 0, 0, 0);
}
