#include "IndexBuffer.h"
#include "Macros/GraphicsMacro.h"

namespace Bind
{
	IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices)
		:
		count(static_cast<UINT>(indices.size()))
	{
		INFOMAN(gfx);
		D3D11_BUFFER_DESC ibd = {};
		ZeroMemory(&ibd, sizeof(ibd));
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = static_cast<UINT>(count * sizeof(unsigned short));
		ibd.StructureByteStride = sizeof(unsigned short);

		D3D11_SUBRESOURCE_DATA isd = {};
		ZeroMemory(&isd, sizeof(isd));
		isd.pSysMem = indices.data();
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&ibd, &isd, &pIndexBuffer));
	}

	void IndexBuffer::Bind(Graphics& gfx) noexcept
	{
		GetDeviceContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	}

	UINT IndexBuffer::GetCount() const noexcept
	{
		return count;
	}
}