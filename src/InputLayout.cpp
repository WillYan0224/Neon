#include "InputLayout.h"
#include "Macros/GraphicsMacro.h"

InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertextShaderBytecode)
{
	INFOMAN(gfx);

	GFX_THROW_INFO(
		GetDevice(gfx)->CreateInputLayout(layout.data(),
			static_cast<UINT>(layout.size()),
			pVertextShaderBytecode->GetBufferPointer(),
			pVertextShaderBytecode->GetBufferSize(),
			&pInputLayout
		)
	);
}

void InputLayout::Bind(Graphics& gfx) noexcept
{
	GetDeviceContext(gfx)->IASetInputLayout(pInputLayout.Get());
}
