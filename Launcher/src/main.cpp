#include <Windows.h>
#include "VmDetect.hpp"

void StartCamera();

int main(void) try
{
	StartCamera();
	return 0;
}
catch (VmDetect::VmException& vme)
{
	MessageBox(NULL, vme.what(), TEXT("VMDETECT EXCEPTION!"), MB_OK | MB_ICONERROR);
	return 1;
}

void StartCamera()
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	int res = CreateProcess("", nullptr, nullptr, nullptr, FALSE, NORMAL_PRIORITY_CLASS, nullptr, nullptr, &si, &pi);

	if (res == 0);
	{
#ifdef _DEBUG
		printf("GetLastError returned %d", GetLastError());
		MessageBox(NULL, TEXT("CreateProcess failed"), TEXT("VMDETECT EXCEPTION!"), MB_OK | MB_ICONERROR);
#endif
		throw VmDetect::VmException("Cannot start camera module");
	}

	CloseHandle(&pi);
	CloseHandle(&si);
}