#include "NoitaTools.h"
HWND NoitaTools::DXWindow;
WNDCLASSEX NoitaTools::WindowClass;
ImGuiApp NoitaTools::DXApp;
NoitaTools* NoitaTools::noitaTools;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	NoitaTools::Get();
};

void NoitaTools::Initialize()
{
	WindowClass = { sizeof(WNDCLASSEX), CS_CLASSDC, DXApp.WndProc, 0L, 0L, 0L, NULL, NULL, NULL, NULL, "NoitaTools", NULL };
	RegisterClassEx(&WindowClass);

	DXWindow = CreateWindow(
		WindowClass.lpszClassName,
		"Noita Tools", WS_OVERLAPPEDWINDOW,
		100, 100, 415, 430, NULL, NULL,
		WindowClass.hInstance, NULL
	);
	if (!DXApp.CreateDeviceD3D(DXWindow)) {
		DXApp.CleanupDeviceD3D();
		DestroyWindow(DXWindow);
		UnregisterClass(WindowClass.lpszClassName, WindowClass.hInstance);
		exit(-1);
	}
	ImGui_ImplWin32_GetDpiScaleForHwnd(DXWindow);
	SetWindowLong(DXWindow, GWL_STYLE, 0);
	ShowWindow(DXWindow, SW_SHOW);
	UpdateWindow(DXWindow);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = NULL;
	ImGui::StyleColorsDark();
	DXApp.SetupBackend();
	GS::Init();
	GS::GuiLoop();
}

NoitaTools* NoitaTools::Get()
{
	if (!noitaTools) {
		noitaTools = new NoitaTools();
		noitaTools->Initialize();
	}
	return noitaTools;
};
