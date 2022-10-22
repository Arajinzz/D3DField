#include "Window.h"

Window::WindowClass Window::WindowClass::wndClass; // Singleton declaration

Window::WindowClass::WindowClass() : hInst( GetModuleHandle(nullptr) )
{

    // Register a window class
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = nullptr;
    wc.hIconSm = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    RegisterClassEx(&wc);
	
}

Window::WindowClass::~WindowClass()
{

    UnregisterClass(GetName(), GetInstance());

}


Window::Window(UINT width, UINT height, const char* name) {

}

Window::~Window() {

}

LRESULT Window::HandleMsgSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}

LRESULT Window::HandleMsgThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}

LRESULT Window::HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}

