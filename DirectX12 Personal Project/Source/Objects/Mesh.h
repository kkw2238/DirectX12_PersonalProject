#pragma once
#include "MathUtil.h"

class Mesh
{
public:
	Mesh();
	Mesh(std::wstring& fildname);
	~Mesh();

public:
	void SetCubeMesh(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dCommandList, float width, float height, float depth);
	void DrawMesh(ID3D12GraphicsCommandList* id3dCommandList, UINT nObjectCount);

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
};

