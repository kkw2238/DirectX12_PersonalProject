#pragma once
/*
 *	LightManager 클래스에는 게임에 사용할 조명 객체의 정보가 담겨 있음,
 *  추가로 CBuffer를 업데이트 해주는 역할도 병행합니다.
 */
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

	std::vector<D3D_SHADER_MACRO> GetShaderDefined();
private:
	std::vector<Light>	m_Lights;

	Vector4	m_Ambient;

	ObjectResourceBuffer<CB_LIGHTS> m_LightUploadBuffer;
};

#define LIGHT_MANAGER LightManager::Instance()