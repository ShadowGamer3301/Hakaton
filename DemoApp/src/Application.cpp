#include "Application.h"

Application::Application()
{
	//Sprawd� czy program nie jest uruchamiany w 
	//maszynie wirtualnej b�d� czy nie ma pod��czonego
	//debuggera
	isVM |= VmDetect::DetectByBiosVendor();
	isVM |= VmDetect::DetectBySystemManufacturer();
	isVM |= VmDetect::DetectByGraphics();

	//Wy��cz sprawdzanie debuggera w konfiguracji _DEBUG
#ifndef _DEBUG
	isVM |= IsDebuggerPresent();
#endif	

	if (isVM == 0)
	{
		pWnd = new Window(1280, 720, "DemoApplication");
		pGfx = new Graphics(pWnd);
	}
	else
	{
		throw DemoException("This software doesn't support you current hardware config");
	}
}

Application::~Application()
{
}

int Application::Run()
{
	while (true)
	{
		if (auto ec = Window::ProcessMessage())
			return *ec;
		pGfx->StartDraw();
		pGfx->DrawInterface();
		pGfx->EndDraw();
	}
	
}
