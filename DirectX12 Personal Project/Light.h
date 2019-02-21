#pragma once
#include "MathUtil.h"
#include "Camera.h"
#include "Structures.h"

class Light : public Camera
{
public:
	Light();
	Light(ID3D12Device* id3dDevice, const CB_LIGHT_INFO& lightInfo);
	Light(ID3D12Device* id3dDevice, const CB_LIGHT_INFO& lightInfo, float l, float r, float t, float b, float n, float f);
	~Light();

public:
	void SetLightInfo(ID3D12Device* id3dDevice, const CB_LIGHT_INFO& lightInfo);
	void SetLightProjection(float l, float r, float t, float b, float n, float f);
	CB_LIGHT_INFO GetLightInfo() const;

private:
	CB_LIGHT_INFO m_LightInfo;
};