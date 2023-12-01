#include "Graphics.h"
#include "dxerr.h"
#include <sstream>
#include "DxgiInfoManager.h"
#include <d3dcompiler.h>

#include <cmath>

using namespace Microsoft::WRL;

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

// graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

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
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
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

	ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
	GFX_THROW_INFO(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pMainRtv));
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.0f };
	pDeviceContext->ClearRenderTargetView(pMainRtv.Get(), color);
}

void Graphics::EndFrame()
{
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

void Graphics::DrawTestTriangle()
{
	namespace wrl = Microsoft::WRL;
	HRESULT hr;

	struct Vertex
	{
		struct 
		{
			float x;
			float y;
		}pos;
		struct 
		{
			float r;
			float g;
			float b;
		}color;
	};

	// create vertex buffer
	const Vertex vertices[] =
	{
		{  0.0f,  0.5f, 1.0f, 0.0f, 0.0f },
		{  0.5f, -0.5f, 0.0f, 1.0f, 0.0f },
		{ -0.5f, -0.5f,	0.0f, 0.0f, 1.0f },
		{ -0.3f,  0.3f,	0.0f, 0.0f, 1.0f },
		{  0.3f,  0.3f, 0.0f, 1.0f, 0.0f },
		{  0.0f, -0.8f, 1.0f, 0.0f, 0.0f },
	};

	// create constant buffer
	struct VB_CB_Offset2D
	{
		float x;
		float y;
	};
	VB_CB_Offset2D offset2D[] =
	{
		{ 0.0f, 0.0f },
	};
	
	// create constant buffer (dynamic)
	ComPtr<ID3D11Buffer> pConstantBufferVSOffset;
	D3D11_BUFFER_DESC cbd = {};
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(offset2D) + (16 - sizeof(offset2D) % 16);
	cbd.StructureByteStride = sizeof(VB_CB_Offset2D);
	D3D11_SUBRESOURCE_DATA csd = {};
	ZeroMemory(&csd, sizeof(csd));
	csd.pSysMem = offset2D;
	GFX_THROW_INFO(pDevice->CreateBuffer(&cbd, &csd, &pConstantBufferVSOffset));
	
	offset2D[0].x += 0.5f;
	// apply change
	D3D11_MAPPED_SUBRESOURCE mappedCBOffset;
	pDeviceContext->Map(pConstantBufferVSOffset.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedCBOffset);
	CopyMemory(mappedCBOffset.pData, offset2D, sizeof(VB_CB_Offset2D));
	pDeviceContext->Unmap(pConstantBufferVSOffset.Get(), 0u);


	// create vertex buffer
	ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertices;
	GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));


	// create indices buffer
	const unsigned short indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 1,
		2, 1, 5,
	};
	ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	ZeroMemory(&isd, sizeof(sd));
	isd.pSysMem = indices;
	GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));
	
	// Bind index buffer to pipeline
	pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);


	// Bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pDeviceContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	pDeviceContext->VSSetConstantBuffers(0, 1, pConstantBufferVSOffset.GetAddressOf());

	// set Viewport(s) as RS
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.Height = 540;
	viewport.Width = 960;

	// bind viewport
	pDeviceContext->RSSetViewports(1u, &viewport);


	// create vertex shader
	ComPtr<ID3D11VertexShader> pVertexShader;
	ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(L"src\\VertexShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, pVertexShader.GetAddressOf()));

	// bind vertex shader
	pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	

	// create input layout (vertex shader)
	ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ieDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	GFX_THROW_INFO(pDevice->CreateInputLayout(ieDesc, static_cast<UINT>(std::size(ieDesc)), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));
	
	// bind vertex input
	pDeviceContext->IASetInputLayout(pInputLayout.Get());

	// bind primitive topology
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// create pixel shader
	ComPtr<ID3D11PixelShader> pPixelShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"src\\PixelShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, pPixelShader.GetAddressOf()));
	
	// bind pixel shader
	pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	

	// bind render targets
	pDeviceContext->OMSetRenderTargets(1, pMainRtv.GetAddressOf(), nullptr);


	// GFX_THROW_INFO_ONLY(pDeviceContext->Draw(static_cast<UINT>(std::size(vertices)), 0u));
	GFX_THROW_INFO_ONLY(pDeviceContext->DrawIndexed(static_cast<UINT>(std::size(indices)), 0u, 0u));

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
