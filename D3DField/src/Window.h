#pragma once

#include <Windows.h>
#include <optional>

#include "Keyboard.h"
#include "Mouse.h"


class Window
{

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
	std::optional<int> ProcessMessages();

private:
	HWND hWnd;
	UINT width, height;

public:
	Keyboard kbd;
	Mouse mouse;

private:
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

