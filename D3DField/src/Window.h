#pragma once

#include <Windows.h>
#include <optional>
#include <memory>

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Exception.h"

// Macros
#define W_EXCEPT(hr) Window::WException(__LINE__, __FILE__, hr)

class Window
{
public:
	class WException : public Exception {
	public:
		WException(unsigned int line, const char* file, HRESULT hr);
		const char* what() const override;
		virtual const char* GetType() const override;
		static std::string TranslateErrorCode(HRESULT hr);
		HRESULT GetErrorCode() const;
		std::string GetErrorString() const;
	private:
		HRESULT hr;
	};
private:
	class WindowClass {
	public:
		static const char* GetName() { return wndClassName; };
		static HINSTANCE GetInstance() { return wndClass.hInst; };

	private:
		WindowClass();
		~WindowClass();
		WindowClass(const WindowClass&) = delete; // delete the copy constructor
		WindowClass& operator=(const WindowClass&) = delete; // can't be assigned
		static constexpr const char* wndClassName = "D3D Field Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:
	Window(UINT width, UINT height, const char* name);
	~Window();
	void InitGfx();
	std::optional<int> ProcessMessages();

private:
	HWND hWnd;
	UINT width, height;

public:
	Keyboard kbd;
	Mouse mouse;
	std::unique_ptr<Graphics> pGfx;

private:
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

