#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

namespace Bind
{
	class TransformCB : public Bindable
	{
	private:
		struct Transforms
		{
			DirectX::XMMATRIX modelViewProj;
			DirectX::XMMATRIX model;
		};
	public:
		TransformCB(Graphics& gfx, const Drawable& parent, UINT slot = 0u);
		void Bind(Graphics& gfx) noexcept override;
	private:
		static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
		const Drawable& parent;
	};
}