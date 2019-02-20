#include "Light.h"


Light::Light()
{
}

Light::Light(ID3D12Device* id3dDevice, const CB_LIGHT_INFO& lightInfo)
{
	SetLightInfo(id3dDevice, lightInfo);
}

Light::~Light()
{
}

void Light::SetLightInfo(ID3D12Device* id3dDevice, const CB_LIGHT_INFO& lightInfo)
{
	m_CamUploadBuffer.CreateResourceBuffer(id3dDevice, 1);

	m_WorldMatrix.resize(1);

	//SetObjPosition(0, lightInfo.position);
	//SetObjLookVector(0, lightInfo.direction);

	m_LightInfo = lightInfo;
	
	m_Projection = XMMatrixShadow(XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f), m_LightInfo.position.GetXMVector());
}

CB_LIGHT_INFO Light::GetLightInfo() const
{
	return m_LightInfo;
}