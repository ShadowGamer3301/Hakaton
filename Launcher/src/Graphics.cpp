#include "Graphics.h"
#include "DemoExcept.h"
#include "ini.h"


Graphics::Graphics(Window* pWnd)
{
	//Stwórz DXGI factory
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(pFactory.GetAddressOf()));
	if (FAILED(hr)) throw DemoException("Application halted due to DXGI malfunction");

	//ZnajdŸ odpowiedni adapter graficzny
	FindSuitalbeAdapter();
	if (pAdapter == NULL) throw DemoException("Application halted due to lack of suitable adapters");

	//Stwórz potrzebne interfejsy do dzia³ania DirectX 11
	CreateDevice();
	CreateRasterizer();
	CreateSwapChain(pWnd);
	CreateDepthStencil(pWnd->width, pWnd->height);
	CreateRenderTarget(pWnd->width, pWnd->height);
	CreateSampler();

	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
}

Graphics::~Graphics()
{
	//Uwolnij wszystkie interfejsy
}

void Graphics::StartDraw()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	float color[4] = { 0.0f, 0.4f, 0.6f, 1.0f };
	pContext->ClearRenderTargetView(pRenderTarget.Get(), color);
}

void Graphics::EndDraw()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	pSwapChain->Present(0, 0);
}

void Graphics::DrawInterface()
{
	static char fname[MAX_STRING], lname[MAX_STRING];

	if (ImGui::Begin("Main Menu"))
	{
		ImGui::InputText("First Name", fname, IM_ARRAYSIZE(fname));
		ImGui::InputText("Last Name", lname, IM_ARRAYSIZE(fname));
		if (ImGui::Button("Login"))
		{
			mINI::INIFile f("sendreq.ini");
			mINI::INIStructure ls;
			ls["User"]["FNAME"] = fname;
			ls["User"]["LNAME"] = lname;

			if (!f.write(ls))
			{
				throw VmDetect::VmException("Failed to create requed file");
			}

			system("py sqlsend.py");
		}

		ImGui::End();
	}
}

void Graphics::FindSuitalbeAdapter()
{
	IDXGIAdapter* tempAdapter;
	UINT i = 0;

	//Go through all adapters installed in computer
	while (pFactory->EnumAdapters(i, &tempAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		//Get details 
		DXGI_ADAPTER_DESC dsc;
		tempAdapter->GetDesc(&dsc);

		printf("Adapter found: %ls\n", dsc.Description);
		if (dsc.DedicatedVideoMemory > 2147483648)
		{
			printf("Adapter suitable: %ls\n", dsc.Description);
			pAdapter = tempAdapter;
			return;

		}
		i++;
	}
}

void Graphics::CreateDevice()
{
	//Wymuœ na urz¹dzeniu wsparcie dla funkcji DirectX 11 i DirectX 11.1
	D3D_FEATURE_LEVEL fl[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
#ifdef _DEBUG
	HRESULT hr = D3D11CreateDevice(pAdapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, NULL, D3D11_CREATE_DEVICE_DEBUG, fl,
		_countof(fl), D3D11_SDK_VERSION, pDevice.GetAddressOf(), nullptr, pContext.GetAddressOf());
#else
	HRESULT hr = D3D11CreateDevice(pAdapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, NULL, 0, fl,
		_countof(fl), D3D11_SDK_VERSION, pDevice.GetAddressOf(), nullptr, pContext.GetAddressOf());
#endif

	if (FAILED(hr)) throw DemoException("Application halted due to D3D11 malfunction");
}

void Graphics::CreateSwapChain(Window* pWnd)
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd)); //Ustaw wartoœci domyœlne
	sd.BufferCount = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.Windowed = true;
	sd.BufferDesc.Width = pWnd->width;
	sd.BufferDesc.Height = pWnd->height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //U¿ywaj 32-bitowej palety barw
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.RefreshRate.Numerator = 240; //Wspieraj odœwierzanie monitora od 1 do 240 Hz
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = 0;
	sd.OutputWindow = pWnd->hWnd;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.SampleDesc.Count = 1; //Wy³¹c anty-aliasing
	sd.SampleDesc.Quality = 0;

	HRESULT hr = pFactory->CreateSwapChain(pDevice.Get(), &sd, pSwapChain.GetAddressOf());
	if (FAILED(hr)) throw DemoException("Program halted due to DXGI malfunction");
}

void Graphics::CreateRenderTarget(int width, int height)
{
	ID3D11Texture2D* pBackBuffer;
	HRESULT hr = pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (FAILED(hr)) throw DemoException("Program halted due to DXGI malfunction");

	hr = pDevice->CreateRenderTargetView(pBackBuffer, 0, pRenderTarget.GetAddressOf());
	if (FAILED(hr)) throw DemoException("Program halted due to D3D11 malfunction");

	pContext->OMSetRenderTargets(1, pRenderTarget.GetAddressOf(), pDepthView.Get());
}

void Graphics::CreateDepthStencil(int width, int height)
{
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_R24G8_TYPELESS;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	HRESULT hr = pDevice->CreateTexture2D(&descDepth, nullptr, pDepthBuffer.GetAddressOf());
	if (FAILED(hr)) throw DemoException("Program halted due to D3D11 malfunction");

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	// Parametry testu g³êbokoœci
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Parametry testu Stencil
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Gdy pixel jest przodem
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Gdy pixel jest do ty³u
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	hr = pDevice->CreateDepthStencilState(&dsDesc, pDepthState.GetAddressOf());
	if (FAILED(hr)) throw DemoException("Program halted due to D3D11 malfunction");

	pContext->OMSetDepthStencilState(pDepthState.Get(), 0);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0;
	hr = pDevice->CreateDepthStencilView(pDepthBuffer.Get(), &descDSV, pDepthView.GetAddressOf());
	if (FAILED(hr)) throw DemoException("Program halted due to D3D11 malfunction");
}

void Graphics::CreateSampler()
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = pDevice->CreateSamplerState(&sampDesc, pSampler.GetAddressOf());
	if (FAILED(hr)) throw DemoException("Program halted due to D3D11 malfunction");
}

void Graphics::CreateRasterizer()
{
	D3D11_RASTERIZER_DESC dsc = {};
	dsc.FillMode = D3D11_FILL_SOLID;
	dsc.CullMode = D3D11_CULL_BACK;
	dsc.FrontCounterClockwise = FALSE;
	dsc.DepthBias = 0;
	dsc.DepthBiasClamp = 0.0f;
	dsc.SlopeScaledDepthBias = 0.0f;
	dsc.DepthClipEnable = FALSE;
	dsc.ScissorEnable = FALSE;
	dsc.MultisampleEnable = TRUE;
	dsc.AntialiasedLineEnable = FALSE;

	HRESULT hr = pDevice->CreateRasterizerState(&dsc, pRasterizer.GetAddressOf());
	if (FAILED(hr)) throw DemoException("Program halted due to D3D11 malfunction");

	pContext->RSSetState(pRasterizer.Get());
}
