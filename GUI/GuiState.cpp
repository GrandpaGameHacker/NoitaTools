#include "GuiState.h"
#include "Interfaces/IWindow.h"
#include "..\NoitaTools.h"
#include "MainWindow.h"
#include "../W32/Memory.h"

int GS::gameSeed = 0;
HANDLE GS::hNoita = NULL;
std::ofstream GS::logFile;
std::vector<int> GS::seedHistory;
float GS::fDpiScale;
static bool bCreated = false;

void GuiState::Init()
{
    if(!bCreated){
        RECT rect;
        GetClientRect(NoitaTools::DXApp.m_hwnd, &rect);
        ImGui::GetIO().DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
        ImGui_ImplWin32_EnableDpiAwareness();
        fDpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(NoitaTools::DXApp.m_hwnd);
        float SCALE = 2.0f;
        ImFontConfig cfg;
        cfg.SizePixels = 13 * SCALE;
        ImGui::GetIO().Fonts->AddFontDefault(&cfg);
        hNoita = GetNoitaHandle();
        static MainWindow mainWin = MainWindow();
        logFile.open("seed_log.txt", std::ios::app);
        atexit(GuiState::closeLog);
        mainWin.Enable();
    }
}

void GuiState::GuiLoop()
{
    bool done = false;
    while (!done)
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;
        NoitaTools::DXApp.CreateFrame();
        for (IWindow* window : IWindow::WindowList) {
            window->Tick();
        }
        NoitaTools::DXApp.RenderFrame();
    }

}

void GuiState::CopyToClipboard(const char* format, ...)
{
    char buffer[1024] = { 0 };
    va_list v1;
    va_start(v1, format);
    vsnprintf(buffer, 1024, format, v1);
    va_end(v1);
    ImGui::SetClipboardText(buffer);
}

void GuiState::closeLog()
{
    logFile.close();
}
