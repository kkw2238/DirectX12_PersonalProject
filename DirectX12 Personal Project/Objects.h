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
	Vector3			GetObjRightVector(UINT index);
	Vector3			GetObjUpVector(UINT index);
	Vector3			GetObjLookVector(UINT index);
	Vector3			GetObjPosition(UINT index);

	virtual void UpdateInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex) {};
	virtual void BuildObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT objectCount) {}

protected:
	std::vector<Matrix4x4>	m_WorldMatrix; 
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

