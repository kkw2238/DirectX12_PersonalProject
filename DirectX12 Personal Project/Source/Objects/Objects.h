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
	void			SetObjRightVector(UINT index, const Vector3& vector);
	void			SetObjUpVector(UINT index, const Vector3& vector);
	void			SetObjLookVector(UINT index, const Vector3& vector);
	void			SetObjPosition(UINT index, const Vector3& vector);
	void			SetObjRotateAngle(UINT index, const Vector3& angles);
	void			SetObjLookAt(UINT index, const Vector3& target);
	void			SetObjLookDirection(UINT index, const Vector3& direction);
	
	Vector3			GetObjRightVector(UINT index);
	Vector3			GetObjUpVector(UINT index);
	Vector3			GetObjLookVector(UINT index);
	Vector3			GetObjPosition(UINT index);
	Vector3			GetObjRotateAngle(UINT index);
	
	void Move(UINT index, DWORD direction, float elapsedtime);
	void Move(UINT index, const Vector3& distance);
	void Rotate(UINT index, const Vector3& angles);

	virtual void UpdateInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex) {};
	virtual UINT BuildObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT objectCount) { return objectCount; }

protected:
	std::vector<Matrix4x4>	m_WorldMatrix; 

	std::vector<Matrix4x4>	m_RotateMatrix;
	std::vector<Vector3>	m_RotateAngle;

	float m_MoveSpeed = 0.1f;

};

/* �ܼ��� �ؽ��� ������Ʈ ( �޽� X )*/
class GraphicsTextureObject : public Objects
{
public:
	GraphicsTextureObject() { m_Is2D = true; };
	~GraphicsTextureObject() {};

public:
	void SetMesh(Mesh* newMesh);
	void Draw(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex);

protected:
	std::vector<TextureRootInfo> m_Textures;

	CD3DX12_CPU_DESCRIPTOR_HANDLE m_CBVCPUDescriptorHandle;
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_CBVGPUDescriptorHandle;

	Mesh* m_pMesh = nullptr;

	UINT m_ObjectCount = 1;

	bool m_IsUsedDescriptorArray = false;
	bool m_Is2D;
};

/* �޽��� ������ ������Ʈ */
class GraphicsMeshObject : public GraphicsTextureObject
{
public:
	GraphicsMeshObject() { m_Is2D = false; };
	~GraphicsMeshObject() {};

public:
	void SetObjScale(UINT index, const Vector3& vector);
	void CreateCBV(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, ID3D12DescriptorHeap* id3dDescriptorHeap, UINT offset);

	Vector3	GetObjScale(UINT index);
	UINT BuildObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT objectCountx);
	
	virtual void UpdateInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex);

protected:
	Vector3 m_ScaleSize = Vector3(1.0f, 1.0f, 1.0f);
	ObjectResourceBuffer<CB_OBJ_INFO> m_ObjUploadBuffer;

	bool isoutput = false;
	float m_AniTime = 0.0f;
};

