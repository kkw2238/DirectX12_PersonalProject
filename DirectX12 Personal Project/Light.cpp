#include "Light.h"


Light::Light()
{
}

Light::Light(ID3D12Device* id3dDevice, const CB_LIGHT_INFO& lightInfo)
{
	SetLightInfo(id3dDevice, lightInfo);
}

Light::Light(ID3D12Device* id3dDevice, const CB_LIGHT_INFO& lightInfo, float l, float r, float t, float b, float n, float f)
{
	SetLightInfo(id3dDevice, lightInfo);
	SetLightProjection(l, r, t, b, n, f);
}

Light::~Light()
{
}

void Light::SetLightInfo(ID3D12Device* id3dDevice, const CB_LIGHT_INFO& lightInfo)
{
	m_CamUploadBuffer.CreateResourceBuffer(id3dDevice, 1);

	m_WorldMatrix.resize(1);
	m_RotateAngle.resize(1);
	m_RotateMatrix.resize(1);

	SetObjPosition(0, lightInfo.position);
	SetObjLookDirection(0, lightInfo.direction);
	CreateScissorRect();
	CreateViewport();

	m_LightInfo = lightInfo;
}

void Light::SetLightProjection(float l, float r, float t, float b, float n, float f)
{
	XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	m_Projection = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);
	m_ShadowMatrix = m_ViewMatrix.Inverse() * m_Projection * T;
}

CB_LIGHT_INFO Light::GetLightInfo() const
{
	return m_LightInfo;
}