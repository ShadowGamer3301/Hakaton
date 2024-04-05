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
}

Application::~Application()
{
}

int Application::Run()
{
	if (isVM == 0)
	{
		Window wnd(1280, 720, "DemoApplication");
		while (true)
		{
			if (auto ec = Window::ProcessMessage())
				return *ec;
		}
	}
	else
	{
		throw DemoException("This software doesn't support you current hardware config");
	}
}
