#include "VertexShader.h"

VertexShader::VertexShader(Graphics& gfx, const std::wstring& path) {
	if (D3DReadFileToBlob(path.c_str(), &pBytecodeBlob) != S_OK) {
		throw ExceptionHandle(__LINE__, __FILE__);
	}
	if (GetDevice(gfx)->CreateVertexShader(pBytecodeBlob->GetBufferPointer(), pBytecodeBlob->GetBufferSize(), 
										   nullptr, &pVertexSharder) != S_OK) {
		throw ExceptionHandle(__LINE__, __FILE__);
	}
}

void VertexShader::Bind(Graphics& gfx) noexcept {
	GetContext(gfx)->VSSetShader(pVertexSharder.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetByteCode() noexcept {
	return pBytecodeBlob.Get();
}
