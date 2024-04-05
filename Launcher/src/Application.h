#pragma once
#include "Window.h"
#include "Graphics.h"

class Application
{
public: 
	Application();
	~Application();

	int Run();

private:
	Window* pWnd;
	Graphics* pGfx;
	int isVM = 0;
};