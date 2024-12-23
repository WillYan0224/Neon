#include "Graphics.h"
#include "dxerr.h"
#include <sstream>
#include "DxgiInfoManager.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Macros/GraphicsMacro.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"


using namespace Microsoft::WRL;
namespace DX = DirectX;
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

Graphics::Graphics( HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(swapDesc));
	swapDesc.BufferDesc.Width = 0;
	swapDesc.BufferDesc.Height = 0;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapDesc.BufferCount = 1;
	swapDesc.OutputWindow = hWnd;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Flags = 0;

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr;

	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapDesc,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pDeviceContext
	));

	// initializing back buffer
	ComPtr<ID3D11Texture2D> BackBuffer;
	GFX_THROW_INFO(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &BackBuffer));
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(BackBuffer.Get(), nullptr, &pMainRtv));


	D3D11_RASTERIZER_DESC rasDesc = {};
	pDevice->CreateRasterizerState(&rasDesc, &pRasterizerState);
	pDeviceContext->RSSetState(pRasterizerState.Get());

	// Create depth stencil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = true;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));
	// bind depth stencil state 
	pDeviceContext->OMSetDepthStencilState(pDSState.Get(), 1u);
	
	// create depth stencil texture2D
	ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC depthDesc = {};
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.Width = 1280u;
	depthDesc.Height = 760u;
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.ArraySize = 1u;
	depthDesc.MipLevels = 1u;
	depthDesc.SampleDesc.Count = 1u;
	depthDesc.SampleDesc.Quality = 0u;
	GFX_THROW_INFO(pDevice->CreateTexture2D(&depthDesc, nullptr, &pDepthStencil));

	// create depth stencil view desc
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0u;
	GFX_THROW_INFO(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &dsvDesc, &pDSV));

	// bind render targets and depth stencil 
	pDeviceContext->OMSetRenderTargets(1u, pMainRtv.GetAddressOf(), pDSV.Get());
	
	// configure viewport
	// D3D11_VIEWPORT vp;
	// vp.Width = 1280.0f;
	// vp.Height = 760.0f;
	// vp.MinDepth = 0.0f;
	// vp.MaxDepth = 1.0f;
	// vp.TopLeftX = 0.0f;
	// vp.TopLeftY = 0.0f;
	// pDeviceContext->RSSetViewports(1u, &vp);
	

	ImGui_ImplDX11_Init(pDevice.Get(), pDeviceContext.Get());
}

void Graphics::SetViewport(float width, float height, float TLX, float TLY) const noexcept
{
	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = TLX;
	vp.TopLeftY = TLY;
	pDeviceContext->RSSetViewports(1u, &vp);
}

void Graphics::RenderToTexture()
{
	ComPtr<ID3D11Texture2D> pTexture;
	D3D11_TEXTURE2D_DESC texDesc = {};
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = 1280u;
	texDesc.Height = 760u;
	texDesc.MipLevels = 1u;
	texDesc.ArraySize = 1u;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count = 1u;
	texDesc.SampleDesc.Quality = 0u;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0u;
	texDesc.MiscFlags = 0u;
	pDevice->CreateTexture2D(&texDesc, nullptr, &pTexture);

	// create shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC texSRVDesc = {};
	texSRVDesc.Format = texDesc.Format;
	texSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	texSRVDesc.Texture2D.MostDetailedMip = 0u;
	texSRVDesc.Texture2D.MipLevels = 1u;
	pDevice->CreateShaderResourceView(pTexture.Get(), &texSRVDesc, &pTexSRV);

	// create render target view
	D3D11_RENDER_TARGET_VIEW_DESC texRtvDesc = {};
	ZeroMemory(&texRtvDesc, sizeof(texRtvDesc));
	texRtvDesc.Format = texDesc.Format;
	texRtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	texRtvDesc.Texture2D.MipSlice = 0u;
	pDevice->CreateRenderTargetView(pTexture.Get(), &texRtvDesc, &pTexRTV);

}

void Graphics::ClearTexture(float red, float green, float blue)
{
}

void Graphics::SetRenderTarget()
{
	pDeviceContext->OMSetRenderTargets(1u, pTexRTV.GetAddressOf(), pDSV.Get());
}

void Graphics::SetBackBufferRenderTarget()
{
	pDeviceContext->OMSetRenderTargets(1u, pMainRtv.GetAddressOf(), nullptr);
}

void Graphics::ClearBackBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.0f };
	pDeviceContext->ClearRenderTargetView(pMainRtv.Get(), color);
	pDeviceContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::RenderViewports()
{
	ImGui::Begin("Viewport");
	ImGui::Image((void*)pTexSRV.Get(), ImVec2(1280, 760));
	ImGui::End();
}


void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.0f };
	pDeviceContext->ClearRenderTargetView(pTexRTV.Get(), color);
	pDeviceContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawIndexed(UINT count) noxnd
{
	GFX_THROW_INFO_ONLY(pDeviceContext->DrawIndexed(count, 0u, 0u));
}

void Graphics::SetProjection(DirectX::FXMMATRIX proj) noexcept
{
	projection = proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
	return projection;
}

void Graphics::SetCamera(DirectX::FXMMATRIX cam) noexcept
{
	camera = cam;
}

DirectX::XMMATRIX Graphics::GetCamera() const noexcept
{
	return camera;
}


void Graphics::EnableImgui() noexcept
{
	imguiEnabled = true;
}

void Graphics::DisableImgui() noexcept
{
	imguiEnabled = false;
}

bool Graphics::IsImguiEnabled() const noexcept
{
	return imguiEnabled;
}

void Graphics::BeginFrame(float red, float green, float blue) noexcept
{
	// imgui begin frame
	if (imguiEnabled)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	// resize render target view on window resize
	if (pMainRtv.Get() == nullptr)
	{
		ComPtr<ID3D11Texture2D> BackBuffer;
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &BackBuffer);
		pDevice->CreateRenderTargetView(BackBuffer.Get(), nullptr, &pMainRtv);
	}
}

void Graphics::EndFrame()
{
		ImGui::Begin("Viewport");
		ImGui::Image((void*)pSRV.Get(), ImVec2(256,256));
		ImGui::End();
	// imgui frame end
	if (imguiEnabled)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	HRESULT hr;
#ifndef NDEBUG
	infoManager.Set();
#endif

	if (FAILED(hr = pSwapChain->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else
		{
			throw GFX_EXCEPT(hr);
		}
	}
}


// Graphics exception stuff
Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file),
	hr(hr)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorString(hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription(hr, buf, sizeof(buf));
	return buf;
}


std::string Graphics::HrException::GetErrorInfo() const noexcept
{
	return info;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Graphics Exception[Device Removed](DXGI_ERROR_DEVICE_REMOVED)";
}

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Neon Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}
