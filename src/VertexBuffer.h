#pragma once
#include "Bindable.h"
#include "Macros/GraphicsMacro.h"
#include "Vertex.h"

namespace Bind
{
	class VertexBuffer : public Bindable
	{
	public:
		template<class T>
		VertexBuffer(Graphics& gfx, const std::vector<T>& vertices)
			:
			stride(sizeof(T))
		{
			INFOMAN(gfx);

			D3D11_BUFFER_DESC bd = {};
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0u;
			bd.MiscFlags = 0u;
			bd.ByteWidth = static_cast<UINT>(sizeof(T) * vertices.size());
			bd.StructureByteStride = sizeof(T);

			D3D11_SUBRESOURCE_DATA sd = {};
			sd.pSysMem = vertices.data();
			GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));
		}
		VertexBuffer(Graphics& gfx, const Dvtx::VertexBuffer& vbuf);
		void Bind(Graphics& gfx) noexcept override;
	protected:
		UINT stride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	};
}

