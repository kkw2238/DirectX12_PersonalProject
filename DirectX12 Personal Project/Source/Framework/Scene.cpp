#include "Scene.h"
#include "LightManager.h"

void Scene::BuildObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	m_TestShaderObject.BuildPipelineObject(id3dDevice, id3dGraphicsCommandList);
	m_DeferredShaderObject.BuildPipelineObject(id3dDevice, id3dGraphicsCommandList);
	m_DebugResourceObject.BuildPipelineObject(id3dDevice, id3dGraphicsCommandList);
	m_TestComputeShaderObject.BuildComputeObjects(id3dDevice, id3dGraphicsCommandList);

	m_Camera = std::make_unique<Camera>();
	m_Camera->BuildObjects(id3dDevice, id3dGraphicsCommandList, 1);
	m_Camera->SetProjectionMatrix(60.0f, (float)CLIENT_WIDTH / (float)CLIENT_HEIGHT, 1.0f, 1000.0f);
}

void Scene::CreateShadowMap(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	Light* light = LIGHT_MANAGER->GetLight(0);
	m_TestShaderObject.CreateShadow(id3dDevice, id3dGraphicsCommandList, L"Shadow", L"Shadow", light);
}

void Scene::RenderObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	m_Camera->SetShadowMatrix(LIGHT_MANAGER->GetLight(0)->GetShadowMatrix());
	m_TestShaderObject.ExecutePipeline(id3dDevice, id3dGraphicsCommandList, L"Obj", L"Obj", m_Camera.get());
}

void Scene::RenderDeferredObjects(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList)
{
	m_Camera->SetShadowMatrix(LIGHT_MANAGER->GetLight(0)->GetShadowMatrix());
	m_DeferredShaderObject.ExecutePipeline(id3dDevice, id3dGraphicsCommandList, L"Deferred", L"Deferred", m_Camera.get());
	m_TestComputeShaderObject.ExecutePipeline(id3dDevice, id3dGraphicsCommandList, L"CalLumFirstPass", L"CalLumFirstPass");

	if (m_EnableDebug) {
		m_DebugResourceObject.ExecutePipeline(id3dDevice, id3dGraphicsCommandList, L"DebugSR", L"Deferred", m_Camera.get());
	}
}

void Scene::UpdateScissorRectViewport()
{
	if (m_Camera != nullptr) {
		m_Camera->CreateScissorRect();
		m_Camera->CreateViewport();
	}
}

void Scene::ProcessKeyboard(float elapsedTime)
{
	DWORD keyDown = 0;

	if (D3DUtil::IsKeyDown('P')) m_EnableDebug = !m_EnableDebug;
	if (D3DUtil::IsKeyDown('W')) keyDown |= DIR_UP;
	if (D3DUtil::IsKeyDown('S')) keyDown |= DIR_DOWN;
	if (D3DUtil::IsKeyDown('D')) keyDown |= DIR_RIGHT;
	if (D3DUtil::IsKeyDown('A')) keyDown |= DIR_LEFT;
	if (D3DUtil::IsKeyDown(VK_UP)) keyDown |= DIR_FORWARD;
	if (D3DUtil::IsKeyDown(VK_DOWN)) keyDown |= DIR_BACK;
	if (D3DUtil::IsKeyDown(VK_ADD)) ProcessMouseWheel(1, elapsedTime);
	if (D3DUtil::IsKeyDown(VK_SUBTRACT)) ProcessMouseWheel(-1, elapsedTime);

	m_Camera.get()->Move(0, keyDown, elapsedTime);
}

void Scene::ProcessMouseWheel(SHORT wheelRotatedir, float elapsedTime)
{
	Vector3 addScaleSize = Vector3(0.01f, 0.01f, 0.01f);
	GraphicsMeshObject* sceneObj = m_TestShaderObject.Objects(0);

	if (sceneObj != nullptr) {
		if (wheelRotatedir > 0) {
			if (sceneObj != nullptr)
				sceneObj->SetObjScale(0, sceneObj->GetObjScale(0) + addScaleSize);
		}
		else 
			sceneObj->SetObjScale(0, sceneObj->GetObjScale(0) - addScaleSize);
		
	}
}

void Scene::ProcesMouseMove(int xpos, int ypos, float elapsedTime)
{
	if (m_IsCapture) {
		POINT mouseDisplacement;
		POINT mousePos;

		GetCursorPos(&mousePos);

		mouseDisplacement.x = mousePos.x - m_MousePos.x;
		mouseDisplacement.y = m_MousePos.y - mousePos.y;

		GraphicsMeshObject* sceneObj = m_TestShaderObject.Objects(0);
		sceneObj->Rotate(0, Vector3(static_cast<float>(mouseDisplacement.y), static_cast<float>(mouseDisplacement.x), 0.0f) / m_MousSeensitivity);

		SetCursorPos(m_MousePos.x, m_MousePos.y);
	}
}

void Scene::ProcesMouseDown(HWND hWnd, UINT msg, int xpos, int ypos, float elapsedTime)
{
	switch (msg) {
	case WM_LBUTTONDOWN:
		GetCursorPos(&m_MousePos);
		ShowCursor(false);
		SetCapture(hWnd);
		m_IsCapture = true;
		break;
	}
}

void Scene::ProcesMouseUp(HWND hWnd, UINT msg, int xpos, int ypos, float elapsedTime)
{
	switch (msg) {
	case WM_LBUTTONUP:
		ReleaseCapture();
		ShowCursor(true);
		SetCursorPos(m_MousePos.x, m_MousePos.y);
		m_IsCapture = false;
		break;
	}
}
