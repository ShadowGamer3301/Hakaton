#pragma once
#include "VmDetect.hpp"
#include "DemoExcept.h"
#include <optional>

class Window
{
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

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
};