#include "Drawable.h"

void Drawable::Draw(Graphics& gfx) {
	for (auto& b : binds) {
		b->Bind(gfx);
	}
	for (auto& b : GetStaticBinds()) {
		b->Bind(gfx);
	}
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) {
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer) noexcept {
	assert("你这有问题啊" && pIndexBuffer == nullptr);
	pIndexBuffer = indexBuffer.get();
	binds.push_back(std::move(indexBuffer));
}