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

    this->width = width;
    this->height = height;

    // Create a window and show it
    this->hWnd = CreateWindowEx(
        0,
        WindowClass::GetName(),
        name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        100, 100, // x, y
        width, height, // w, h
        nullptr,
        nullptr,
        WindowClass::GetInstance(),
        this
    );

    ShowWindow(this->hWnd, SW_SHOW);
}

Window::~Window() {
    DestroyWindow(this->hWnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_NCCREATE) {
        /*
        * So when creating a Window Class (Look at the constructor of Window just right above)
        * we're setting the lParam to the Window instance.
        * This piece of code will get that lParam and convert it to the Window Class
        * and then store this window pointer on USERDATA, also it changes
        * the WNDPROC to HandleMsgThunk.
        * This way we can have multiple Windows.
        */
        const CREATESTRUCTW* const pCreate = (CREATESTRUCTW*)lParam;
        Window* const pWnd = (Window*)pCreate->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)(pWnd));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)(&Window::HandleMsgThunk));
        return pWnd->HandleMsg(hWnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Get Window pointer stored in HandleMsgSetup
    Window* const pWnd = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    return pWnd->HandleMsg(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        // Quit window.
        PostQuitMessage(69);
        break;

    case WM_KILLFOCUS:
        kbd.ClearState();
        break;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN: // for keys like Alt
        if ( !(lParam & 0x40000000) || kbd.IsAutorepeatEnabled() )
            kbd.OnKeyPressed(wParam);
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP: // for keys like Alt
        kbd.OnKeyReleased(wParam);
        break;

    case WM_CHAR:
        kbd.OnChar(wParam);
        break;

    case WM_MOUSEMOVE:
    {
        POINTS pt = MAKEPOINTS(lParam);
        mouse.OnMouseMove(pt.x, pt.y);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        POINTS pt = MAKEPOINTS(lParam);
        mouse.OnLeftPressed(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        POINTS pt = MAKEPOINTS(lParam);
        mouse.OnRightPressed(pt.x, pt.y);
        break;
    }
    case WM_LBUTTONUP:
    {
        POINTS pt = MAKEPOINTS(lParam);
        mouse.OnLeftReleased(pt.x, pt.y);
        break;
    }
    case WM_RBUTTONUP:
    {
        POINTS pt = MAKEPOINTS(lParam);
        mouse.OnRightReleased(pt.x, pt.y);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        POINTS pt = MAKEPOINTS(lParam);
        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
            mouse.OnWheelUp(pt.x, pt.y);
        }
        else if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
            mouse.OnWheelDown(pt.x, pt.y);
        }
        break;
    }

    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

