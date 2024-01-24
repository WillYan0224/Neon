#include "TransformCB.h"

namespace Bind
{
	TransformCB::TransformCB(Graphics& gfx, const Drawable& parent, UINT slot)
		:
		parent(parent)
	{
		if (!pVcbuf)
		{
			pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
		}
	}

	void TransformCB::Bind(Graphics& gfx) noexcept
	{
		const auto modelView = parent.GetTransformXM() * gfx.GetCamera();
		const auto model = parent.GetTransformXM();
		const Transforms tf =
		{
			DirectX::XMMatrixTranspose(modelView),
			DirectX::XMMatrixTranspose(
				modelView * gfx.GetProjection()
			)
		};
		pVcbuf->Update(gfx, tf);
		pVcbuf->Bind(gfx);
	}


	std::unique_ptr<VertexConstantBuffer<TransformCB::Transforms>> TransformCB::pVcbuf;
}