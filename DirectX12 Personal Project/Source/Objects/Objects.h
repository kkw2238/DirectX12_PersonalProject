#pragma once
#include "D3DUtil.h"
#include "MeshManager.h"
#include "Structures.h"
#include "TextureManager.h"
#include "D3DDescriptorFactory.h"


class Objects
{
public:
	Objects();
	~Objects();

public:
	void			SetObjRightVector(UINT index, Vector3& vector);
	void			SetObjUpVector(UINT index, Vector3& vector);
	void			SetObjLookVector(UINT index, Vector3& vector);
	void			SetObjPosition(UINT index, Vector3& vector);

	Vector3			GetObjRightVector(UINT index);
	Vector3			GetObjUpVector(UINT index);
	Vector3			GetObjLookVector(UINT index);
	Vector3			GetObjPosition(UINT index);

	void Move(UINT index, DWORD direction, float elapsedtime);
	void Move(UINT index, const Vector3& distance);

	virtual void UpdateInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex) {};
	virtual UINT BuildObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT objectCount) { return objectCount; }

protected:
	std::vector<Matrix4x4>	m_WorldMatrix; 
	float m_MoveSpeed = 0.1f;
};

/* 단순한 텍스쳐 오브젝트 ( 메쉬 X )*/
class GraphicsTextureObject : public Objects
{
public:
	GraphicsTextureObject() {};
	~GraphicsTextureObject() {};

public:
	void SetMesh(Mesh* newMesh);
	void SetTextures(ID3D12Device* id3dDevice, std::vector<TextureRootInfo>& newTextures, ID3D12DescriptorHeap* id3dDescriptorHeap, UINT offset);
	void SetTexture(ID3D12Device* id3dDevice, TextureRootInfo& newTexture, ID3D12DescriptorHeap* id3dDescriptorHeap, UINT offset);
	void CreateSRV(ID3D12Device* id3dDevice, ID3D12DescriptorHeap* id3dDescriptorHeap, UINT offset);
	void AddTexture(TextureRootInfo& newTexture);
	void Draw(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex);
	void UpdateTextureInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList);

protected:
	std::vector<TextureRootInfo> m_Textures;

	CD3DX12_CPU_DESCRIPTOR_HANDLE m_CBVCPUDescriptorHandle;
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_CBVGPUDescriptorHandle;

	Mesh* m_pMesh = nullptr;

	UINT m_ObjectCount = 1;
};

/* 메쉬를 소유한 오브젝트 */
class GraphicsMeshObject : public GraphicsTextureObject
{
public:
	GraphicsMeshObject() {};
	~GraphicsMeshObject() {};

public:
	void CreateCBV(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, ID3D12DescriptorHeap* id3dDescriptorHeap, UINT offset);

	UINT BuildObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT objectCountx);
	
	virtual void UpdateInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex);

protected:
	
	ObjectResourceBuffer<CB_OBJ_INFO> m_ObjUploadBuffer;
};

