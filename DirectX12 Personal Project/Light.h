#pragma once
#include "MathUtil.h"
#include "Camera.h"
#include "Structures.h"

class Light : public Camera
{
public:
	Light();
	Light(ID3D12Device* id3dDevice, const CB_LIGHT_INFO& lightInfo);
	~Light();

public:
	void SetLightInfo(ID3D12Device* id3dDevice, const CB_LIGHT_INFO& lightInfo);
	CB_LIGHT_INFO GetLightInfo() const;

private:
	CB_LIGHT_INFO m_LightInfo;
};