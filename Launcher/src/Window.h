#pragma once
#include "VmDetect.hpp"
#include "DemoExcept.h"
#include <optional>
#include "imgui.h"
#include "imgui_impl_win32.h"

class Window
{
	friend class Graphics;
private:
	class WindowClass
	{
	public:
		static const char* GetName() noexcept { return "DemoClass"; }
		static HINSTANCE GetInstance() noexcept { return wndClass.hInst; }

	private:
		WindowClass();
		~WindowClass() noexcept;

		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:
	Window(UINT width, UINT height, const char* title);
	~Window();

	static std::optional<int> ProcessMessage();

private:
	HWND hWnd;
	UINT width, height;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
};