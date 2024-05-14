#pragma once
#include "NeonWin.h"
#include "NeonException.h"
#include "DxgiInfoManager.h"
#include <vector>
#include "d3d11.h"
#include "wrl.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>
#include "ConditionalNoexcept.h"

namespace Bind
{
	class Bindable;
}


class Graphics
{
	friend class Bind::Bindable;
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
	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
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
	~Graphics() = default;
	void BeginFrame(float r, float g, float b) noexcept;
	void EndFrame();
	void ClearBuffer(float red, float green, float blue);
	void DrawIndexed(UINT count) noxnd;
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	void SetCamera(DirectX::FXMMATRIX cam) noexcept;
	DirectX::XMMATRIX GetCamera() const noexcept;
	void EnableImgui() noexcept;
	void DisableImgui() noexcept;
	bool IsImguiEnabled() const noexcept;
	void SetViewport(float width, float height, float TLX, float TLY) const noexcept;

	void SetRenderTarget();
	void RenderToTexture();
	void ClearTexture(float red, float green, float blue);
	void SetBackBufferRenderTarget();
	void ClearBackBuffer(float red, float green, float blue);
	void RenderViewports();
		
private:
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;
	bool imguiEnabled = true;
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pMainRtv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRV = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTexRTV = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexSRV = nullptr;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizerState = nullptr;
};