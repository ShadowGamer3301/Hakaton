#pragma once

#define WIN32_LEAN_AND_MEAN //Wy��cz rzadko u�ywane funkcje biblioteki windowsa
#define MAX_STRING 256 //Zdefiniuj maksymaln� d�ugo�� ci�gu na 256 znak�w

#include <Windows.h>
#include <dxgi1_6.h>
#include <vector>
#include <d3d12.h>

#include "VmExcept.h"

namespace VmDetect
{
	bool DetectByBiosVendor(); //Weryfikacja poprzez sprawdzenie producenta BIOSu
	bool DetectBySystemManufacturer(); //Weryfikacja poprzez sprawdzenie producenta komputera
	bool DetectByGraphics(); //Weryfikacja poprzez sprawdzenie akceleracji DirectX 12
}