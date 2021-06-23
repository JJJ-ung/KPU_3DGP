#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <chrono>

#include <wrl.h>
#include <math.h>
#include <shellapi.h>

#include <list>
#include <set>
#include <map>
#include <array>
#include <vector>
#include <algorithm>
#include <functional>

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <mmsystem.h>
#include <dinput.h>

#include "d3dx12.h"

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif  // _DEBUG  

#include <dxgidebug.h>

#endif

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

using namespace std;
using namespace chrono;

using namespace DirectX;
using namespace DirectX::PackedVector;

using Microsoft::WRL::ComPtr;

///////////////////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

///////////////////////////////////////////////////////////////////////////////////////

#include "Enum.h"
#include "Struct.h"
#include "Function.h"
#include "Typedef.h"

const unsigned short	g_nWinCX = 1280;
const unsigned short	g_nWinCY = 720;

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

#define  NOEVENT 0
#define  OBJDEAD 1 

extern UINT BufferSlotSize[RootParam::END];

extern UINT g_iTexIndex;
#define TEXCNT 100

extern BOOL g_bColliderRender;