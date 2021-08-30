#pragma once
#include <Windows.h>
#include "GUI/Helpers/ImGuiApp.h"
#include "GUI/GuiState.h"


class NoitaTools
{
public:
	NoitaTools() {};
	NoitaTools(NoitaTools const&) = delete;
	void operator=(NoitaTools const&) = delete;
	void Initialize();
	static NoitaTools* Get();

	static HWND DXWindow;
	static WNDCLASSEX WindowClass;
	static ImGuiApp DXApp;

private:
	static NoitaTools* noitaTools;
};