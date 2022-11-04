#pragma once


#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include <wrl/client.h>

#include "dxerr.h"
#include "Exception.h"
#include "DXGIInfoManager.h"


#define GFX_CHECK_ERROR(hrcall) infoManager.Set(); if(FAILED(hr = hrcall)) throw Graphics::DXException(__LINE__, __FILE__, hr, infoManager.GetMessages());

class Graphics
{
public:
	class DXException : public Exception {
	public:
		DXException(unsigned int line, const char* file, HRESULT hr, std::vector<std::string> infos = {});
		const char* what() const override;
		virtual const char* GetType() const override;
		std::string GetErrorDesc() const;
		std::string GetErrorString() const;
		std::string GetErrorInfo() const;
	private:
		HRESULT hr;
		std::string info;
	};
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void EndFrame();
	void ClearBuffer(float r, float g, float b);

	void DrawTriangle();

private:
	DXGIInfoManager infoManager;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
};

