#pragma once
#include "Bindable.h"

class VertexShader : public Bindable {
public:
	VertexShader(Graphics& gfx, const std::wstring& path);
	void Bind(Graphics& gfx) noexcept override;
	ID3DBlob* GetByteCode() noexcept;

protected:
	ComPtr<ID3DBlob> pBytecodeBlob;
	ComPtr<ID3D11VertexShader> pVertexSharder;
};
