// This is the entry point

#include "Window.h"


int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow
)
{

    Window window(800, 600, "D3DField");

    // Handle messages
    MSG msg = { 0 };
    int result;

    while ( (result = GetMessage(&msg, nullptr, 0, 0)) > 0 ) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (window.kbd.IsKeyPressed(VK_MENU)) {
            MessageBox(nullptr, "Alt Key Pressed!!!", "Testing The ALT KEY", 0);
        }

    }

    // if result is -1 means that an error occured, otherwise exit with PostQuitMessage code
    int returnValue = (result == -1) ? -1 : msg.wParam;

    return returnValue;
}