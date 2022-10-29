#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <dxgidebug.h>
#include <wrl/client.h>
#include <memory>

class DXGIInfoManager
{
public:
	DXGIInfoManager();
	~DXGIInfoManager() = default;

	DXGIInfoManager(const DXGIInfoManager&) = delete;
	DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;

	void Set();
	std::vector<std::string> GetMessages();

private:
	unsigned long long next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
};

