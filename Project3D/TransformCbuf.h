#pragma once
#include "Graphics.h"
#include "ConstantBuffer.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformCbuf : public Bindable {
public:
	TransformCbuf(Graphics& gfx, const Drawable& parent,UINT slot = 0u);
	void Bind(Graphics& gfx) noexcept override;
private: 
	struct Transforms {
		DirectX::XMMATRIX modelViewProj;
	    DirectX::XMMATRIX model;
	};

	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
	const Drawable& parent;
};