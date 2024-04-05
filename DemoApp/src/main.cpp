#include "Application.h"

int main(void) try
{
	auto app = new Application();
	int ret = app->Run();
	delete app;

	return ret;
}
catch (VmDetect::VmException& vme)
{
	MessageBox(NULL, vme.what(), "VMDETECT EXCEPTION!", MB_ICONERROR | MB_OK);
	return 1;
}
catch (DemoException& de)
{
	MessageBox(NULL, de.what(), "DEMOAPPEXCEPTION!", MB_ICONERROR | MB_OK);
	return 1;
}