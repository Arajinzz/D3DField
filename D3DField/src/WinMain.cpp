// This is the entry point

#include <Windows.h>

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow
)
{

    const LPCWSTR className = L"D3DField";

    // Register a window class
    WNDCLASSEX wndClass = {0};
    wndClass.cbSize = sizeof(wndClass);
    wndClass.style = CS_OWNDC;
    wndClass.lpfnWndProc = DefWindowProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = nullptr;
    wndClass.hIconSm = nullptr;
    wndClass.hCursor = nullptr;
    wndClass.hbrBackground = nullptr;
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = className;
    RegisterClassEx(&wndClass);

    // Create a window instance
    HWND hWnd = CreateWindowEx(
        0,
        className,
        L"D3DField",
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        100,
        100,
        800,
        600,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    // Test window display, To be removed later
    ShowWindow(hWnd, SW_SHOW);
    while (true);

    return 0;
}