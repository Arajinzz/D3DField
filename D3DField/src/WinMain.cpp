// This is the entry point

#include "Window.h"
#include "Exception.h"

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    Window window(800, 600, "D3DField");
    try {
        // Because Graphics constructor throws exceptions it should be created in try catch bloc
        window.InitGfx();
        // Handle messages
        MSG msg = { 0 };
        std::optional<int> result;

        while (true) {
        
            result = window.ProcessMessages();

            if (result) { // Means it have a value so we received a quit message 
                break;
            }
            window.pGfx->ClearBuffer(0, 0.5, 0.5);
            window.pGfx->DrawTriangle();
            window.pGfx->EndFrame();
            if (window.kbd.IsKeyPressed(VK_MENU)) {
                MessageBox(nullptr, "Alt Key Pressed!!!", "Testing The ALT KEY", 0);
            }

        }

        // if result is -1 means that an error occured, otherwise exit with PostQuitMessage code
        int returnValue = (*result == -1) ? -1 : msg.wParam;
        return returnValue;
    }
    catch (const Exception& e) {
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONERROR);
    }
    catch (const std::exception& e) {
        MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONERROR);
    }
    catch (...) {
        MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONERROR);
    }

    return -1;

}