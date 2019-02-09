#include "Light.h"


Light::Light()
{
}

Light::Light(const CB_LIGHT_INFO& lightInfo)
{
	SetLightInfo(lightInfo);
}


Light::~Light()
{
}

void Light::SetLightInfo(const CB_LIGHT_INFO& lightInfo)
{
	m_LightInfo = lightInfo;
	m_ShadowMatrix = XMMatrixShadow(m_LightInfo.direction.GetXMVector(), m_LightInfo.direction.GetXMVector());
}

CB_LIGHT_INFO Light::GetLightInfo() const
{
	return m_LightInfo;
}

Matrix4x4 Light::GetShadowMatrix() const
{
	return m_ShadowMatrix;
}
