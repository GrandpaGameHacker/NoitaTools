#pragma once
#include <Windows.h>
#include <vector>
#include <fstream>
struct GuiState
{
	GuiState() {};
	GuiState(GuiState const&) = delete;
	void operator=(GuiState const&) = delete;
	static float fDpiScale;
	static HANDLE hNoita;
	static int gameSeed;
	static std::vector<int> seedHistory;
	static std::ofstream logFile;
	static void Init();
	static void GuiLoop();
	static void CopyToClipboard(const char* format, ...);
	static void closeLog();
};

typedef GuiState GS;
