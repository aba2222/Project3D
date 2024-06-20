#pragma once
#include "Bindable.h"

class Sampler : public Bindable {
public:
	Sampler(Graphics& gfx);
	void Bind(Graphics& gfx) noexcept override;

protected:
	ComPtr<ID3D11SamplerState> pSampler;
};

