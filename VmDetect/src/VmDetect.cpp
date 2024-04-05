#include "VmExcept.h"
#include "VmDetect.hpp"

namespace VmDetect
{
	bool DetectByBiosVendor()
	{
		HKEY key = 0; char buffer[MAX_STRING] = { 0 }; DWORD bufSize = sizeof(buffer); LSTATUS status = 0;
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\HardwareConfig\\Current\\"), NULL, KEY_READ, &key) != ERROR_SUCCESS)
		{
#ifdef _DEBUG
			MessageBox(NULL, TEXT("Cannot open SYSTEM\\HardwareConfig\\Current in RegEdit"), TEXT("ERROR!"), MB_OK | MB_ICONERROR);
#endif
			throw VmException("Program halted due to critical error!");
		}

		status = RegGetValue(key, NULL, TEXT("BIOSVendor"), RRF_RT_REG_SZ, NULL, buffer, &bufSize);
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
	bool DetectBySystemManufacturer()
	{
		HKEY key = 0; char buffer[MAX_STRING] = { 0 }; DWORD bufSize = sizeof(buffer); LSTATUS status = 0;
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\HardwareConfig\\Current\\"), NULL, KEY_READ, &key) != ERROR_SUCCESS)
		{
#ifdef _DEBUG
			MessageBox(NULL, TEXT("Cannot open SYSTEM\\HardwareConfig\\Current in RegEdit"), TEXT("ERROR!"), MB_OK | MB_ICONERROR);
#endif
			throw VmException("Program halted due to critical error!");
		}

		status = RegGetValue(key, NULL, TEXT("SystemManufacturer"), RRF_RT_REG_SZ, NULL, buffer, &bufSize);
		if (status != ERROR_SUCCESS)
		{
#ifdef _DEBUG
			MessageBox(NULL, TEXT("Cannot open SystemManufacturer in RegEdit"), TEXT("ERROR!"), MB_OK | MB_ICONERROR);
#endif
			throw VmException("Program halted due to critical error!");
		}

		if (strcmp(buffer, "Microsoft Corporation") == 0)
			return true;

		return false;
	}

	bool DetectByGraphics()
	{
		//W zwi¹zku i¿ najpopularniejsze narzêdzia do wirtualizacji nie
		//wspieraj¹ akceleracji DirectX 12 u¿yj jej by sprawdziæ czy 
		//aplikacja nie jest uruchamiana w maszynie wirtualnej

		HRESULT hr = S_OK;
		IDXGIFactory* pFactory = nullptr;
		ID3D12Device* pDevice = nullptr;
		IDXGIAdapter* pAdapter = nullptr;

		//Stwórz interfejs DXGI
		hr = CreateDXGIFactory(IID_PPV_ARGS(&pFactory));
		if (FAILED(hr))
		{
#ifdef _DEBUG
			MessageBox(NULL, TEXT("CreateDXGIFactory failed"), TEXT("ERROR!"), MB_OK | MB_ICONERROR);
#endif
			throw VmException("Program halted due to critical error!");
		}

		//Wyszukaj dostêpne adaptery
		UINT i = 0;
		IDXGIAdapter* tempAdapter;
		while (pFactory->EnumAdapters(i, &tempAdapter) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC desc = { 0 };
			tempAdapter->GetDesc(&desc);

			//SprawdŸ czy adapter posiada co najmniej 2GB pamiêci video
			if (desc.DedicatedVideoMemory > 2147483648)
			{
				//Jeœli posiada to zapisz go i zakoñcz przeszukiwanie
				pAdapter = tempAdapter;
				break;
			}

			i++;
		}

		//Stwórz interfejs urz¹dzenia dla DirectX 12
		hr = D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&pDevice));
		if (FAILED(hr))
		{
#ifdef _DEBUG
			MessageBox(NULL, TEXT("D3D12CreateDevice failed"), TEXT("ERROR!"), MB_OK | MB_ICONERROR);
#endif
			//Jeœli funkcja zawiod³a uznaj komputer za maszynê wirtualn¹
			pFactory->Release();
			pAdapter->Release();

			return true;
		}

		//Wyczyœæ pamiêæ z d³u¿ej niepotrzebnych interfejsów
		pDevice->Release();
		pAdapter->Release();
		pFactory->Release();

		return false;
	}
	
}