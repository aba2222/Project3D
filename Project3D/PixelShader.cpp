#include "PixelShader.h"

PixelShader::PixelShader(Graphics& gfx, const std::wstring& path) {
	ComPtr<ID3DBlob> pBlob;
	if (D3DReadFileToBlob(path.c_str(), &pBlob) != S_OK) {
		throw ExceptionHandle(__LINE__, __FILE__);
	}
	if (GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader) != S_OK) {
		throw ExceptionHandle(__LINE__, __FILE__);
	}
}

void PixelShader::Bind(Graphics& gfx) noexcept {
	GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
