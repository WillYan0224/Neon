#pragma once
#include "NeonWin.h"
#include "NeonExcpetion.h"
#include "DxgiInfoManager.h"
#include <vector>
#include "d3d11.h"

class Graphics
{
public:
	class Exception : public NeonException
	{
		using NeonException::NeonException;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete; // no copy constructor
	Graphics& operator=(const Graphics&) = delete; // no copy constructor
	~Graphics();
	void EndFrame();
	void ClearBuffer(float red, float green, float blue);
private:
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pDeviceContext = nullptr;
	ID3D11RenderTargetView* pMainRtv = nullptr;
};