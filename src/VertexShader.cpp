#include "VertexShader.h"
#include "Macros/GraphicsMacro.h"

namespace Bind
{
	VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
	{
		INFOMAN(gfx);

		GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pByteCodeBlob));
		GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(
			pByteCodeBlob->GetBufferPointer(),
			pByteCodeBlob->GetBufferSize(),
			nullptr,
			&pVertexShader
		)
		);
	}

	void VertexShader::Bind(Graphics& gfx) noexcept
	{
		GetDeviceContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	}

	ID3DBlob* VertexShader::GetByteCode() const noexcept
	{
		return pByteCodeBlob.Get();
	}
}