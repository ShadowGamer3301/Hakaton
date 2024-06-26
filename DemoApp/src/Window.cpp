#include "Window.h"

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass()
	: hInst(GetModuleHandle(nullptr))
{
	//Zdefiniuj klas� okna
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.hInstance = GetInstance();
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = GetName();

	if (!RegisterClassEx(&wcex))
	{
#ifdef _DEBUG 
		MessageBox(NULL, TEXT("Failed to register window class"), TEXT("DEMOEXCEPTION"), MB_OK | MB_ICONERROR);
#endif
		throw DemoException("Program halted due to WINAPI malfunction!");
	}
}

Window::WindowClass::~WindowClass() noexcept
{
	UnregisterClass(GetName(), GetInstance());
}

Window::Window(UINT width, UINT height, const char* title)
	: width(width), height(height)
{
	hWnd = CreateWindow(WindowClass::GetName(), title, WS_OVERLAPPEDWINDOW, 0, 0, width, height, nullptr, nullptr, WindowClass::GetInstance(), nullptr);
	if (hWnd == NULL)
	{
#ifdef _DEBUG 
		MessageBox(NULL, TEXT("Failed to create HWND"), TEXT("DEMOEXCEPTION"), MB_OK | MB_ICONERROR);
#endif
		throw DemoException("Program halted due to WINAPI malfunction!");
	}

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; //Umo�liw sterowanie klawiatur�

	ImGui_ImplWin32_Init(hWnd);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

std::optional<int> Window::ProcessMessage()
{
	MSG msg = { 0 };

	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return msg.wParam;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

LRESULT Window::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wp, lp))
		return true;

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		return 0;

	default:
		return DefWindowProc(hWnd, msg, wp, lp);
		break;
	}

}
