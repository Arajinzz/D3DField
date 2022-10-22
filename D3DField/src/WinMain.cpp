// This is the entry point

#include <Windows.h>
#include <string>
#include <sstream>


LRESULT CALLBACK MainWndProc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        // Quit window.
        PostQuitMessage(69);
        break;

    case WM_KEYDOWN:
    {
        const char character[2] = { wParam, 0 };
        SetWindowText(hWnd, character);
        break;
    }

    case WM_KEYUP:
        SetWindowText(hWnd, "D3DField");
        break;

    case WM_CHAR:
    {
        static std::string title;
        title.push_back(wParam);
        SetWindowText(hWnd, title.c_str());
        break;
    }

    case WM_LBUTTONDOWN:
    {
        POINTS pt = MAKEPOINTS(lParam);
        std::ostringstream oss;
        oss << "(" << pt.x << "," << pt.y << ")";
        SetWindowText(hWnd, oss.str().c_str());
        break;
    }

    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow
)
{

    const auto className = "D3DField";

    // Register a window class
    WNDCLASSEX wndClass = {0};
    wndClass.cbSize = sizeof(wndClass);
    wndClass.style = CS_OWNDC;
    wndClass.lpfnWndProc = MainWndProc;
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
        "D3DField",
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        100, 100, // x, y
        800, 600, // w, h
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    ShowWindow(hWnd, SW_SHOW);

    // Handle messages
    MSG msg = { 0 };
    int result;

    while ( (result = GetMessage(&msg, nullptr, 0, 0)) > 0 ) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // if result is -1 means that an error occured, otherwise exit with PostQuitMessage code
    int returnValue = (result == -1) ? -1 : msg.wParam;

    return returnValue;
}