#include "VmExcept.h"
#include "VmDetect.hpp"

namespace VmDetect
{
	bool DetectByBiosVendor()
	{
		HKEY key = 0; char buffer[MAX_STRING] = { 0 }; DWORD bufSize = sizeof(buffer); LSTATUS status = 0;
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\HardwareConfig\\Current"), NULL, NULL, &key) != ERROR_SUCCESS)
		{
#ifdef _DEBUG
			MessageBox(NULL, TEXT("Cannot open SYSTEM\\HardwareConfig\\Current in RegEdit"), TEXT("ERROR!"), MB_OK | MB_ICONERROR);
#endif
			throw VmException("Program halted due to critical error!");
		}

		status = RegGetValue(key, NULL, "BIOSVendor", NULL, NULL, buffer, &bufSize);
		if(status != ERROR_SUCCESS)
		{
#ifdef _DEBUG
			MessageBox(NULL, TEXT("Cannot open BIOSVendor in RegEdit"), TEXT("ERROR!"), MB_OK | MB_ICONERROR);
#endif
			throw VmException("Program halted due to critical error!");
		}

		if (strcmp(buffer, "Microsoft Corporation") == 0)
			return true;

		return false;
	}
}