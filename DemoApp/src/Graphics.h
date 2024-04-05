#pragma once
#include "Window.h"
#include <d3d11.h>
#include <wrl.h>
#include "imgui_impl_dx11.h"

class Graphics
{
public:
	Graphics(Window* pWnd);
	~Graphics();

	void StartDraw();
	void EndDraw();
	void DrawInterface();

private:
	void FindSuitalbeAdapter();
	void CreateDevice();
	void CreateSwapChain(Window* pWnd);
	void CreateRenderTarget(int width, int height);
	void CreateDepthStencil(int width, int height);
	void CreateSampler();
	void CreateRasterizer();

private:
	//Pozosta³e dane
	static const constexpr UINT mNumBuffers = 2; //Liczba buforów video

	//Interfejsy DXGI 
	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;
	Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;

	//Interfejsy D3D11
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTarget;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthView;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthBuffer;
};