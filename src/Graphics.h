#pragma once
#include "NeonWin.h"
#include "d3d11.h"

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete; // no copy constructor
	Graphics& operator=(const Graphics&) = delete; // no copy constructor
	~Graphics();
	void EndFrame();
private:
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pDeviceContext = nullptr;
};