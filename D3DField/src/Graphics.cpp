#include "Graphics.h"

// Set up linking for d3d11
#pragma comment(lib, "d3d11.lib")

namespace wrl = Microsoft::WRL;

Graphics::Graphics(HWND hWnd)
{

	DXGI_SWAP_CHAIN_DESC SwapDesc = {};
	SwapDesc.BufferDesc.Width = 0; // take all window space
	SwapDesc.BufferDesc.Height = 0;
	SwapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // Pixels layout (channels) BGRA
	SwapDesc.BufferDesc.RefreshRate.Numerator = 0; // Refresh rate
	SwapDesc.BufferDesc.RefreshRate.Denominator = 0;
	SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // No scaling required because it will take all the window space
	SwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapDesc.SampleDesc.Count = 1; // NO Anti aliasing
	SwapDesc.SampleDesc.Quality = 0;
	SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // This buffer is the render target
	SwapDesc.BufferCount = 1; // 1 front buffer and 1 back buffer
	SwapDesc.OutputWindow = hWnd;
	SwapDesc.Windowed = TRUE;
	SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // Multi buffering swap
	SwapDesc.Flags = 0;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&SwapDesc,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	);

	// Gain access to the back-buffer
	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), (void**)(&pBackBuffer));
	pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pTarget
	);

}

void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u);
}

void Graphics::ClearBuffer(float r, float g, float b)
{
	const float color[] = { r, g, b, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}
