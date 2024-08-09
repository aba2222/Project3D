#pragma once
#include "Bindable.h"

template<typename C>
class ConstantBuffer : public Bindable {
public:
	ConstantBuffer(Graphics& gfx, const C& consts, UINT slot = 0u)
		: slot(slot) {
		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;
		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		if (GetDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer) != S_OK) {
			throw ExceptionHandle(__LINE__, __FILE__);
		}
	}


	ConstantBuffer(Graphics& gfx, UINT slot = 0u)
		: slot(slot) {
		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;
		if (GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer) != S_OK) {
			throw ExceptionHandle(__LINE__, __FILE__);
		}
	}

	void Update(Graphics& gfx, const C& consts) {
		D3D11_MAPPED_SUBRESOURCE msr;
		if (GetContext(gfx)->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr) != S_OK) {
			throw ExceptionHandle(__LINE__, __FILE__);
		}
		int a= sizeof(consts);
		memcpy(msr.pData, &consts, sizeof(consts));
		GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
	}

	void Update(Graphics& gfx, const C& consts, int size) {
		D3D11_MAPPED_SUBRESOURCE msr;
		if (GetContext(gfx)->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr) != S_OK) {
			throw ExceptionHandle(__LINE__, __FILE__);
		}
		memcpy(msr.pData, &consts, size);
		GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
	}

protected:
	ComPtr<ID3D11Buffer> pConstantBuffer;
	UINT slot;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C> {
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx) noexcept override { GetContext(gfx)->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf()); };
}; 

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C> {
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx) noexcept override { GetContext(gfx)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf()); };
};