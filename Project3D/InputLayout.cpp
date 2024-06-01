#include "InputLayout.h"

InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode) {
	if (GetDevice(gfx)->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&pInputLayout) != S_OK) { //E_INVALIDARG
		throw ExceptionHandle(__LINE__, __FILE__);
	}
}

void InputLayout::Bind(Graphics& gfx) noexcept {
	GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}