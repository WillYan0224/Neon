#include "TransformCB.h"

TransformCB::TransformCB(Graphics& gfx, const Drawable& parent)
	:
	parent(parent)
{
	if (!pVcbuf)
	{
		pVcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void TransformCB::Bind(Graphics& gfx) noexcept
{
	pVcbuf->Update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * gfx.GetProjection()
		)
	);
	pVcbuf->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCB::pVcbuf;