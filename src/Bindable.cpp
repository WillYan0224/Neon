#include "Bindable.h"
#include <stdexcept>

ID3D11DeviceContext* Bindable::GetDeviceContext(Graphics& gfx) noexcept
{
	return gfx.pDeviceContext.Get();
}

ID3D11Device* Bindable::GetDevice(const Graphics& gfx) noexcept
{
	return gfx.pDevice.Get();
}

DxgiInfoManager& Bindable::GetInfoManager(const Graphics& gfx) noexcept(IS_DEBUG)
{
#ifndef NDEBUG
	return gfx.infoManager;
#else
	throw std::logic_error("Error! (tried to access gfx.infoManager in Release config)");
#endif
}
