#pragma once
#include "Light.h"

class LightManager
{
public:
	LightManager();
	~LightManager();

public:
	static LightManager* Instance();
	
	void CreateLight(ID3D12Device* id3dDevice);
	void UpdateLightInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex);

	std::vector<Light> GetLights();

	Light* GetLight(UINT index);
	Matrix4x4* GetLightMatrix(UINT index);

private:
	ObjectResourceBuffer<CB_LIGHTS> m_LightUploadBuffer;
	std::vector<Light>	m_Lights;
	Vector4	m_Ambient;
};

#define LIGHT_MANAGER LightManager::Instance()