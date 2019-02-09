#pragma once
#include "MathUtil.h"
#include "Structures.h"

class Light
{
public:
	Light();
	Light(const CB_LIGHT_INFO& lightInfo);
	~Light();

public:
	void SetLightInfo(const CB_LIGHT_INFO& lightInfo);
	CB_LIGHT_INFO GetLightInfo() const;
	Matrix4x4 GetShadowMatrix() const;

private:
	Matrix4x4	m_ShadowMatrix;
	CB_LIGHT_INFO m_LightInfo;
};