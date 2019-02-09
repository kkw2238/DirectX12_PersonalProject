#include "LightManager.h"



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

	m_Ambient = Vector4(1.1f, 1.1f, 1.1f, 1.0f);
	m_Lights.resize(MAX_LIGHT);
	m_Lights = {
		//					�����				  ���� ���� �Ÿ�		  ���� ��ġ		���� �� �Ÿ�				���� ����	 ����Ʈ����Ʈ �Ŀ�
		Light(CB_LIGHT_INFO{Vector3(0.5f, 0.0f, 0.0f), 1.0f, Vector3(1.0f, 1.0f, 1.0f), 60.0f, Vector3(-1.0f, -1.0f, -1.0f), 10.0f})
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
