// dear imgui - standalone example application for DirectX 11
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include "TextEditor.h"
#include <dwmapi.h>
#include "Bridge.h"

#pragma comment(lib, "dwmapi.lib")

void FindWindowA_Bypass() {
	DWORD old;
	VirtualProtect((LPVOID)&FindWindowA, 1, PAGE_EXECUTE_READWRITE, &old);
	*(char*)&FindWindowA = 0x90;
	VirtualProtect((LPVOID)&FindWindowA, 1, old, &old);
}

// Data
static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

HRESULT CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return E_FAIL;

	CreateRenderTarget();

	return S_OK;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

RECT wSizeM;
HWND tWndM;
int rWidthM;
int rHeightM;
int ClientMem = 0;
MARGINS MARGIN;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//tWndM = FindWindowA(NULL, "Roblox");
	//GetWindowRect(tWndM, &wSizeM);
	//MARGIN = { 0, 0, rWidthM , rHeightM };

	switch (msg) {
	case WM_CREATE:
		if (ClientMem == 0) {
			//DwmExtendFrameIntoClientArea(hWnd, &MARGIN);
			ClientMem = 1;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(1);
		break;

	default:
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}

}

char WINNAME[19] = " ";
WNDCLASSEX wndClass;

void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

TextEditor editor;
#include <thread>
bool toggled = true;
HWND hwnd;


RECT wSize;
HWND tWnd;
int rWidth;
int rHeight;


void detectopen() {

}

int settext = 0;

int InitImgui(int, char**)
{
	FindWindowA_Bypass();
	std::thread worker(detectopen);
	int horizontal = 0;
	int vertical = 0;
	GetDesktopResolution(horizontal, vertical);
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbClsExtra = NULL;
	wndClass.cbWndExtra = NULL;
	wndClass.hCursor = LoadCursor(0, IDC_ARROW);
	wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wndClass.hbrBackground = (HBRUSH)RGB(0, 0, 0);
	wndClass.hInstance = GetModuleHandle(NULL);;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = WINNAME;
	wndClass.lpszMenuName = WINNAME;
	wndClass.style = CS_VREDRAW | CS_HREDRAW;

	// Create application window
	//WNDCLASSEX wndClass = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
	//RegisterClassEx(&wc);

	if (!RegisterClassEx(&wndClass))
	{
		exit(1);
	}

	tWnd = FindWindowA(NULL, "Roblox");
	GetWindowRect(tWnd, &wSize);
	rWidth = wSize.right - wSize.left;
	rHeight = wSize.bottom - wSize.top;

	hwnd = CreateWindowEx(WS_EX_TOPMOST /*| WS_EX_TRANSPARENT*/ | WS_EX_LAYERED, WINNAME, WINNAME, WS_POPUP, 1, 1, horizontal, vertical, 0, 0, 0, 0);
	SetLayeredWindowAttributes(hwnd, 0, 0.1f, LWA_ALPHA);
	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, ULW_COLORKEY);
	//MARGINS margins = { -1 };
	//DwmExtendFrameIntoClientArea(hwnd, &margins);
	// Initialize Direct3D
	if (CreateDeviceD3D(hwnd) < 0)
	{
		CleanupDeviceD3D();
		UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
		return 1;
	}

	// Show the window
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);


	// Our state
	ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

	// Main loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{

		if (!FindWindowA(NULL, "Roblox"))
			exit(1);

		if (GetKeyState(0x2E) & 0x8000)
		{
			if (toggled == true) {
				Sleep(300);
				ShowWindow(hwnd, SW_HIDE);
				toggled = false;
			}
			else if (toggled == false) {
				Sleep(300);
				ShowWindow(hwnd, SW_SHOW);
				toggled = true;
			}
		}



		tWnd = FindWindowA(NULL, "Roblox");
		GetWindowRect(tWnd, &wSize);
		rWidth = wSize.right - wSize.left;
		rHeight = wSize.bottom - wSize.top;
		MoveWindow(hwnd, wSize.left, wSize.top, rWidth - 10, rHeight - 10, true);

		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		{

			ImGui::Begin("Vain - Unrestricted Level 6 Lua Wrapper");
			editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
			editor.Render("Script Editor", ImVec2(580, 265));
			if (settext == 0) {
				editor.SetText("--Created by robomat and elkoax");
				settext = 1;
			}
			if (ImGui::Button("Execute", ImVec2(70, 20))) {
				//Wrapper::Execute_Script(editor.GetText());
			}
			ImGui::SameLine(60, 60);
			if (ImGui::Button("Clear", ImVec2(70, 20))) {
				editor.SetText("");
			}
			ImGui::End();

			ImGui::Begin("Settings");
			ImGui::Text("Toggle Key: Delete");
			ImGui::Text("Credits");
			ImGui::Text("Main Dev: RoboMat");
			ImGui::Text("Updater: ElKoax and Squiddy");
			ImGui::End();
		}



		// Rendering
		ImGui::Render();
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		//g_pSwapChain->Present(1, 0); // Present with vsync
		g_pSwapChain->Present(0, 0); // Present without vsync
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	DestroyWindow(hwnd);
	UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);

	return 0;
}