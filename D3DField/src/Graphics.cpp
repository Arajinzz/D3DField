#include "Graphics.h"

#include <string>
#include <sstream>
#include <d3dcompiler.h>

// Set up linking for d3d11
#pragma comment(lib, "d3d11.lib")

// To load shaders
#pragma comment(lib, "D3DCompiler.lib")

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

	HRESULT hr; // Should be declared before calling GFX_CHECK_ERROR macro

	GFX_CHECK_ERROR(
		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			D3D11_CREATE_DEVICE_DEBUG,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&SwapDesc,
			&pSwap,
			&pDevice,
			nullptr,
			&pContext
		)
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

void Graphics::DrawTriangle()
{
	namespace wrl = Microsoft::WRL;

	struct Vertex {
		struct {
			float x;
			float y;
		} pos;

		struct {
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
	};

	const Vertex vertices[] = {
		{ 0.0f,  0.5f, 255,   0,   0, 0},
		{ 0.5f, -0.5f,   0, 255,   0, 0},
		{-0.5f, -0.5f,   0,   0, 255, 0},
	};

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.CPUAccessFlags = 0u;
	vertexBufferDesc.MiscFlags = 0u;
	vertexBufferDesc.ByteWidth = sizeof(vertices);
	vertexBufferDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA vertexSubData = {};
	vertexSubData.pSysMem = vertices;

	HRESULT hr;
	// Create vertex buffer
	GFX_CHECK_ERROR(pDevice->CreateBuffer(&vertexBufferDesc, &vertexSubData, &pVertexBuffer))

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;

	// Bind vertex buffer
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// Create Index buffer
	const unsigned short indices[] = {
		0, 1, 2,
	};

	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA indexSubData = {};
	indexSubData.pSysMem = indices;

	GFX_CHECK_ERROR(pDevice->CreateBuffer(&indexBufferDesc, &indexSubData, &pIndexBuffer));

	// Bind index buffer
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// Create vertex shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	wrl::ComPtr<ID3DBlob> pVertexBlob;

	GFX_CHECK_ERROR(D3DReadFileToBlob(L"VertexShader.cso", &pVertexBlob));
	GFX_CHECK_ERROR(
		pDevice->CreateVertexShader(pVertexBlob->GetBufferPointer(), pVertexBlob->GetBufferSize(), nullptr, &pVertexShader)
	);

	// Bind Vertex Shader
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);

	// Create pixel shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pPixelBlob;

	GFX_CHECK_ERROR(D3DReadFileToBlob(L"PixelShader.cso", &pPixelBlob));
	GFX_CHECK_ERROR(
		pDevice->CreatePixelShader(pPixelBlob->GetBufferPointer(), pPixelBlob->GetBufferSize(), nullptr, &pPixelShader)
	);

	// Bind Pixel Shader
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

	// Input layout obj
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC inputElemDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	GFX_CHECK_ERROR(
		pDevice->CreateInputLayout(
			inputElemDesc,
			std::size(inputElemDesc),
			pVertexBlob->GetBufferPointer(),
			pVertexBlob->GetBufferSize(),
			&pInputLayout
		)
	);

	// Bind input layout
	pContext->IASetInputLayout(pInputLayout.Get());

	// Bind render target
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	// Setup primitive topology to Triangle List (Groups of 3 vertices)
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Setup Viewport
	D3D11_VIEWPORT viewPort;
	viewPort.Width = 800;
	viewPort.Height = 600;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;

	pContext->RSSetViewports(1u, &viewPort);

	pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);
}

Graphics::DXException::DXException(unsigned int line, const char* file, HRESULT hr, std::vector<std::string> infos) :
	Exception(line, file),
	hr(hr)
{

	for (const auto& msg : infos) {
		info += msg;
		info.push_back('\n');
	}

	// Removes final new line
	if (!info.empty()) {
		info.pop_back();
	}

}

const char* Graphics::DXException::what() const
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDesc() << std::endl;

	if (!info.empty()) {
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl;
	}

	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::DXException::GetType() const
{
	return "DirectX Exception";
}

std::string Graphics::DXException::GetErrorDesc() const
{
	WCHAR buffer[1024];
	DXGetErrorDescription(hr, buffer, 1024);
	// Convert to string
	std::wstring wide_string(buffer);
	std::string str(wide_string.begin(), wide_string.end());
	return str;
}

std::string Graphics::DXException::GetErrorString() const
{
	const WCHAR* errStr = DXGetErrorStringW(hr);
	std::wstring wide_string(errStr);
	std::string str(wide_string.begin(), wide_string.end());
	return str;
}

std::string Graphics::DXException::GetErrorInfo() const
{
	return info;
}
