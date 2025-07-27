#include "WindowsPlatform.h"

#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include <Windows.h>

#include "../../Gui.h"

HWND WindowsPlatform::window = nullptr;
WNDCLASSEX WindowsPlatform::windowClass = { };
POINTS WindowsPlatform::position = { };

// DirectX state vars
PDIRECT3D9 WindowsPlatform::d3d = nullptr;
LPDIRECT3DDEVICE9 WindowsPlatform::device = nullptr;
D3DPRESENT_PARAMETERS WindowsPlatform::presentParameters = { };

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND window,
    UINT message,
    WPARAM wideParameter,
    LPARAM longParameter
);

long __stdcall WindowProcess(
    HWND window,
    UINT message,
    WPARAM wideParameter,
    LPARAM longParameter)
{
    if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
        return true;

    switch (message)
    {
    case WM_SIZE: {
            if (WindowsPlatform::device && wideParameter != SIZE_MINIMIZED)
            {
                WindowsPlatform::presentParameters.BackBufferWidth = LOWORD(longParameter);
                WindowsPlatform::presentParameters.BackBufferHeight = HIWORD(longParameter);
                WindowsPlatform::Get()->ResetDevice();
            }
    }return 0;

    case WM_SYSCOMMAND: {
            if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
    }break;

    case WM_DESTROY: {
            PostQuitMessage(0);
    }return 0;

    case WM_LBUTTONDOWN: {
            WindowsPlatform::position = MAKEPOINTS(longParameter); // set click points
    }return 0;

    case WM_MOUSEMOVE: {
            if (wideParameter == MK_LBUTTON)
            {
                const auto points = MAKEPOINTS(longParameter);
                auto rect = ::RECT{ };

                GetWindowRect(WindowsPlatform::window, &rect);

                rect.left += points.x - WindowsPlatform::position.x;
                rect.top += points.y - WindowsPlatform::position.y;

                if (WindowsPlatform::position.x >= 0 &&
                    WindowsPlatform::position.x <= Gui::WIDTH &&
                    WindowsPlatform::position.y >= 0 && WindowsPlatform::position.y <= 19)
                    SetWindowPos(
                        WindowsPlatform::window,
                        HWND_TOPMOST,
                        rect.left,
                        rect.top,
                        0, 0,
                        SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
                    );
            }
    }return 0;

    case WM_CLIPBOARDUPDATE: {
            if (WindowsPlatform::Get() && WindowsPlatform::Get()->onCopy)
            {
                WindowsPlatform::Get()->onCopy();
            }
    }return 0;
    }

    return long(DefWindowProc(window, message, wideParameter, longParameter));
}

void SetRoundedWindow(HWND hWnd, int width, int height, int radius) {
    HRGN hRegion = CreateRoundRectRgn(0, 0, width + 1, height + 1, radius, radius);
    SetWindowRgn(hWnd, hRegion, TRUE);
}

void WindowsPlatform::CreateHWindow(const char* inWindowName) noexcept
{
    Gui::windowName = inWindowName;
    wchar_t* wString = new wchar_t[strlen(Gui::windowName)];
    MultiByteToWideChar(CP_ACP, 0, Gui::windowName, -1, wString, 4096);

    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_CLASSDC;
    windowClass.lpfnWndProc = WNDPROC(WindowProcess);
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = GetModuleHandleA(0);
    windowClass.hIcon = 0;
    windowClass.hCursor = 0;
    windowClass.hbrBackground = 0;
    windowClass.lpszMenuName = 0;
    windowClass.lpszClassName = wString;
    windowClass.hIconSm = 0;

    RegisterClassEx(&windowClass);
	
    window = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_NOACTIVATE | WS_EX_TOOLWINDOW,
        wString,
        wString,
        WS_POPUP | WS_VISIBLE,
        100,
        100,
        Gui::WIDTH,
        Gui::HEIGHT,
        nullptr,
        nullptr,
        windowClass.hInstance,
        nullptr
    );

    SetLayeredWindowAttributes(window, 0, 0, LWA_ALPHA);
    SetLayeredWindowAttributes(window, 0, RGB(0, 0, 0), LWA_COLORKEY);

    SetRoundedWindow(window, Gui::WIDTH, Gui::HEIGHT, 20);

    ShowWindow(window, SW_SHOWNORMAL);
    UpdateWindow(window);

    // Word Counter
    AddClipboardFormatListener(window);
}

void WindowsPlatform::ResizeWindow(int width, int height) noexcept
{
    // Update window size and region
    SetWindowPos(window, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
    // Update rounded corners
    SetRoundedWindow(window, width, height, 20);
    
    // Update DirectX presentation parameters
    if (device)
    {
        presentParameters.BackBufferWidth = width;
        presentParameters.BackBufferHeight = height;
        ResetDevice();
    }
    
    // Update ImGui display size
    if (ImGui::GetCurrentContext())
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
    }

}

void WindowsPlatform::DestroyHWindow() noexcept
{
    DestroyWindow(window);
    UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool WindowsPlatform::CreateDevice() noexcept
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    if (!d3d)
        return false;

    ZeroMemory(&presentParameters, sizeof(presentParameters));

    presentParameters.Windowed = TRUE;
    presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    presentParameters.EnableAutoDepthStencil = TRUE;
    presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
    presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (d3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        window,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &presentParameters,
        &device) < 0)
        return false;

    return true;
}

void WindowsPlatform::ResetDevice() noexcept
{
    ImGui_ImplDX9_InvalidateDeviceObjects();

    const auto result = device->Reset(&presentParameters);

    if (result == D3DERR_INVALIDCALL)
        IM_ASSERT(0);

    ImGui_ImplDX9_CreateDeviceObjects();
}

void WindowsPlatform::DestroyDevice() noexcept
{
    if (device)
    {
        device->Release();
        device = nullptr;
    }

    if (d3d)
    {
        d3d->Release();
        d3d = nullptr;
    }
}

void WindowsPlatform::SimulateCtrlC()
{
    keybd_event(VK_CONTROL, 0, 0, 0);
    keybd_event('C', 0, 0, 0);
    keybd_event('C', 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
    Sleep(100);
}

std::string WindowsPlatform::GetClipboardText()
{
    std::string text;
    if (OpenClipboard(NULL)) {
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData) {
            char* pszText = static_cast<char*>(GlobalLock(hData));
            if (pszText) {
                text = pszText;
                GlobalUnlock(hData);
            }
        }
        CloseClipboard();
    }
    return text;
}

bool WindowsPlatform::CreatePlatformWindow(const std::string& windowName)
{
    CreateHWindow(windowName.c_str());
    CreateDevice();
    return true;
}

void WindowsPlatform::InitImguiImpl()
{
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(device);
}

void WindowsPlatform::ShutdownImguiImpl()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
}

bool WindowsPlatform::DestroyPlatformWindow()
{
    DestroyDevice();
    DestroyHWindow();
    return true;
}

void WindowsPlatform::BeginRender()
{
    MSG message;
    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);

        if (message.message == WM_QUIT)
        {
            Gui::isRunning = !Gui::isRunning;
            return;
        }
    }
    
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
}

void WindowsPlatform::EndRender()
{
    device->SetRenderState(D3DRS_ZENABLE, FALSE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

    device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
	
    if (device->BeginScene() >= 0)
    {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        device->EndScene();
    }

    const auto result = device->Present(0, 0, 0, 0);

    // Handle loss of D3D9 device
    if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
        ResetDevice();
}

std::string WindowsPlatform::GetAssetPath(const std::string& relativePath)
{
    return ("Assets/" + relativePath);
}

void WindowsPlatform::LaunchURL(const std::string& URL)
{
    ShellExecute(nullptr, L"open", std::wstring(URL.begin(), URL.end()).c_str(), nullptr, nullptr, SW_SHOW);
}
