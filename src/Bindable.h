#pragma once
#include "Graphics.h"

class Bindable
{
public:
	virtual void Bind(Graphics& gfx) noexcept = 0;
	virtual ~Bindable() = default;
protected:
	static ID3D11DeviceContext* GetDeviceContext(Graphics& gfx) noexcept;
	static ID3D11Device* GetDevice(const Graphics& gfx) noexcept;
	static DxgiInfoManager& GetInfoManager(const Graphics& gfx) noexcept(!IS_DEBUG);
};

