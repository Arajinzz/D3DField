#pragma once


#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>

#include "dxerr.h"
#include "Exception.h"

class Graphics
{
public:
	class DXException : public Exception {
	public:
		DXException(unsigned int line, const char* file, HRESULT hr);
		const char* what() const override;
		virtual const char* GetType() const override;
		std::string GetErrorDesc() const;
		std::string GetErrorString() const;
	private:
		HRESULT hr;
	};
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void EndFrame();
	void ClearBuffer(float r, float g, float b);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
};

