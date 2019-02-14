#include "LightManager.h"

#define NUM_DIR_LIGHT "1"
#define NUM_POINT_LIGHT "1"

LightManager::LightManager()
{
}


LightManager::~LightManager()
{
}

LightManager* LightManager::Instance()
{
	static LightManager instance;
	return &instance;
}

void LightManager::CreateLight(ID3D12Device* id3dDevice)
{
	CB_LIGHTS cb_lightsinfo;

	m_Ambient = Vector4(0.1f, 0.1f, 0.1f, 1.0f);
	m_Lights.resize(MAX_LIGHT);
	m_Lights = {
		//					조명색				  감쇠 사작 거리		  조명 위치		감쇠 끝 거리				조명 방향	 스포트라이트 파워
		Light(CB_LIGHT_INFO{Vector3(0.5f, 0.5f, 0.5f), 1.0f, Vector3(-1.0f, 1.0f, -1.0f), 60.0f, Vector3(1.0f, -1.0f, 1.0f), 10.0f}),
		Light(CB_LIGHT_INFO{Vector3(0.05f, 0.05f, 0.05f), 20.0f, Vector3(-5.0f, 5.0f, -5.0f), 30.0f, Vector3(1.0f, -1.0f, 1.0f), 10.0f})
	};

	cb_lightsinfo.ambient = m_Ambient;
	for (int i = 0; i < MAX_LIGHT; ++i)
		cb_lightsinfo.lights[i] = m_Lights[i].GetLightInfo();
	
	m_LightUploadBuffer.CreateResourceBuffer(id3dDevice, MAX_LIGHT);
	m_LightUploadBuffer.CopyData(0, cb_lightsinfo);
}


void LightManager::UpdateLightInfo(ID3D12GraphicsCommandList* id3dGraphicsCommandList, UINT rootParameterIndex)
{
	id3dGraphicsCommandList->SetGraphicsRootConstantBufferView(rootParameterIndex, m_LightUploadBuffer.GPUVirtualAddress());
}

std::vector<Light> LightManager::GetLights()
{
	return m_Lights;
}

Light* LightManager::GetLight(UINT index)
{
	if (m_Lights.size() > index)
		return &m_Lights[index];

	return nullptr;
}

Matrix4x4* LightManager::GetLightMatrix(UINT index)
{
	if (m_Lights.size() > index)
		return &m_Lights[index].GetShadowMatrix();

	return nullptr;
}

std::vector<D3D_SHADER_MACRO> LightManager::GetShaderDefined()
{
	std::vector<D3D_SHADER_MACRO> macros;

	macros.push_back(D3D_SHADER_MACRO{ "NUM_DIRECTION", NUM_DIR_LIGHT });
	macros.push_back(D3D_SHADER_MACRO{ "NUM_POINT", NUM_POINT_LIGHT });
	macros.push_back(D3D_SHADER_MACRO{ NULL, NULL });
	
	return macros;
}
