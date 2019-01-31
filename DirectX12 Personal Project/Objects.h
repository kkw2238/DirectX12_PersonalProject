#pragma once
#include "Source/D3DUtil.h"
#include "Mesh.h"
#include "Sturctures.h"

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

	void Move(UINT index, WORD direction, float elapsedtime);
	void Move(UINT index, const Vector3& distance);

	virtual void UpdateInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex) {};
	virtual void BuildObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT objectCount) {}
protected:
	std::vector<Matrix4x4>	m_WorldMatrix; 
	float m_MoveSpeed = 0.1f;
};

class GraphicsObjects : public Objects
{
public:
	GraphicsObjects() {};
	~GraphicsObjects() {};

public:
	void Draw(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex);
	void SetMesh(Mesh* newMesh);
	void BuildObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT objectCountx);
	
	virtual void UpdateInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex);
protected:
	Mesh* m_pMesh;
	UINT m_ObjectCount = 1;

	ObjectResourceBuffer<CB_OBJ_INFO> m_ObjUploadBuffer;
};

