#pragma once
#include "Source/D3DUtil.h"
#include "MathUtil.h"
#include "Mesh.h"

class Objects
{
public:
	Objects();
	~Objects();

public:
	Vector3			GetObjLightVector();
	Vector3			GetObjUpVector();
	Vector3			GetObjLookVector();
	Vector3			GetObjPosition();

	void UpdateWorldMatrix(ComPtr<ID3D12GraphicsCommandList> id3dCommandList) {};

protected:
	Matrix4x4			m_WorldMatrix; 

	Vector3				m_PositionVector;
};

class GraphicsObjects : public Objects
{
public:
	GraphicsObjects() {};
	~GraphicsObjects() {};

public:
	void Draw(ComPtr<ID3D12GraphicsCommandList> id3dGraphicsCommandList);
	void SetMesh(Mesh* newMesh);

protected:
	Mesh* m_pMesh;
	UINT m_ObjectCount = 1;
};

