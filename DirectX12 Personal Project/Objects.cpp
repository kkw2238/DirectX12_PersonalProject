#include "Objects.h"

Objects::Objects()
{
}

Objects::~Objects()
{
}

Vector3 Objects::GetObjLightVector()
{
	return m_WorldMatrix._xyz1;
}

Vector3 Objects::GetObjUpVector()
{
	return m_WorldMatrix._xyz2;
}

Vector3 Objects::GetObjLookVector()
{
	return m_WorldMatrix._xyz3;
}

Vector3 Objects::GetObjPosition()
{
	return m_PositionVector;
}

void GraphicsObjects::Draw(ComPtr<ID3D12GraphicsCommandList> id3dGraphicsCommandList)
{
	m_pMesh->DrawMesh(id3dGraphicsCommandList, m_ObjectCount);
}

void GraphicsObjects::SetMesh(Mesh* newMesh)
{
	m_pMesh = newMesh;
}
