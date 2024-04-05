#pragma once
#include "Window.h"

class Application
{
public: 
	Application();
	~Application();

	int Run();

private:
	Window* pWnd;
	int isVM = 0;
};