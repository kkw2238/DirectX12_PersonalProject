// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 특정 포함 파일이 들어 있는
// 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <windowsx.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <wrl.h>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cassert>

// DirectX 관련 헤더
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <D3Dcompiler.h>
#include <comdef.h>
#include <dxgi1_4.h>

// 여기서 프로그램에 필요한 추가 헤더를 참조합니다.
#include "Source/d3dx12.h"

// lib 참조
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

// const
#ifndef  DEF_APPINFO
#define  DEF_APPINFO
	static	int		CLIENT_WIDTH = 800;
	static	int		CLIENT_HEIGHT = 600;
		
	static	bool	App4xMsaaState = false;
	static	UINT	App4xMsaaQuality = 0;
	
	static const UINT DIR_RIGHT = 1;
	static const UINT DIR_LEFT = 2;
	static const UINT DIR_UP = 4;
	static const UINT DIR_DOWN = 8;
#endif // ! DEF_APPINFO

#if defined(_DEBUG)
	#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

using namespace DirectX;

