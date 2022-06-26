#define WIN32_LEAN_AND_MEAN

#pragma warning(disable : 4311)
#pragma warning(disable : 4302)
#pragma warning(disable : 4267)

#include <windows.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <fcntl.h>
#include <io.h>
#include <cstdint>
#include <cstdlib>
#include <dwmapi.h>
#include <comdef.h> 
#include <winternl.h>

#include <dwmapi.h>
#include <comdef.h> 
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dwmapi.lib")

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx9.h"
#include "Imgui/imgui_impl_win32.h"

#include "xor.h"

int Width = GetSystemMetrics(SM_CXSCREEN);
int Height = GetSystemMetrics(SM_CYSCREEN);
const MARGINS Margin = { -1 };
DWORD ScreenCenterX;
DWORD ScreenCenterY;

DWORD_PTR Uworld;
DWORD_PTR LocalPawn;
DWORD_PTR LocalPlayers;
DWORD_PTR Localplayer;
DWORD_PTR Gameinstance;
DWORD_PTR Rootcomp;
DWORD_PTR PlayerController;
DWORD_PTR Ulevel;
DWORD_PTR TeamIndex;
int localplayerID;
#define BOT_ACTORID 16797950
D3DXVECTOR4 Rect;
bool isaimbotting;
DWORD_PTR entityx;
float AimFOV = 40;
DWORD64 ActorCount;
DWORD_PTR AActors;

struct Entity
{
	int id;
	std::string name;
	DWORD_PTR addr;
	DWORD_PTR rootcomponent;
	DWORD_PTR bone;
	DWORD_PTR type;
};
int EntityCount;
Entity EntityList[1000];

#ifdef _DEBUG
#pragma comment( lib, xorstr_("kelib/debug/kelib.lib"))
#else
#pragma comment( lib, "kelib/release/kelib.lib" )
#endif

#pragma comment( linker, "\"/manifestdependency:type='win32' \
	name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
	processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"" )

DWORD64 UWorld = 0x705EB70;//fn
DWORD64 CameraLocation = 0x7038AC0;//fn
DWORD64 GInstance = 0x170;//fn
DWORD64 UlevelX = 0x30;//fn
DWORD64 actorArray = 0x98;//fn
DWORD64 actorCount = 0xA0;//fn
DWORD64 playerController = 0x30;//fn
DWORD64 ObjectID = 0x18;//fn
DWORD64 UlocalPlayer = 0x38;//fn
DWORD64 rootComp = 0x130;//fn
DWORD64 rootPos = 0x11C;//fn
DWORD64 localp4wn = 0x298;//fn
DWORD64 Mesh = 0x278;//fn
DWORD64 PlayerStateX = 0x238;//fn
DWORD64 TeamID = 0xE38;//fn
DWORD64 arraybone = 0x410;//fn
DWORD64 CompotoWorld = 0x1C0;//fn
DWORD64 CameraX = 0x12C;//fn
DWORD64 CameraY = 0x678;//fn
DWORD64 Pitch = 0xb0;//fn
DWORD64 ZoomBase = 0x500;//fn
DWORD64 Yaw = 0x12C;

#include "vector.h"
#include "driver.h"
#include "Math.h"
#include "process.h"
#include "colors.h"
#include "overlay.h"
#include "function.h"

void create_console();

HRESULT DirectXInit(HWND hWnd);
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
void SetupWindow();
WPARAM MainLoop();
void CleanuoD3D();

int isTopwin();
void SetWindowToTarget();

#define TopWindowGame 11
#define TopWindowMvoe 22


IDirect3D9Ex* p_Object = NULL;
IDirect3DDevice9Ex* p_Device = NULL;
D3DPRESENT_PARAMETERS p_Params = { NULL };

#define M_Name xorstr_(L" ")
HWND MyWnd = NULL;
HWND GameWnd = NULL;
RECT GameRect = { NULL };
MSG Message = { NULL };

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);