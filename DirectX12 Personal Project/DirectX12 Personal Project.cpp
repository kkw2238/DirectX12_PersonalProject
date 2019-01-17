// DirectX12 Personal Project.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "DirectX12 Personal Project.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{
		Framework frameWork(hInstance);
		if (!frameWork.Initialized())
			return 0;

		return frameWork.Run();
	}
	catch (ExeptionUtility& e)
	{
		MessageBox(nullptr, e.D3DErrorToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}
