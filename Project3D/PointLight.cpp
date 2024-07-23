#include "PointLight.h"
#include "imgui/imgui.h"

PointLight::PointLight(Graphics& gfx, float radius)
	: mesh(gfx, radius),
	  cbuf(gfx) {
	Reset();
}

void PointLight::SpawnControlWindow() noexcept {
	if (ImGui::Begin("Light")) {
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &lData.pos.x , -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &lData.pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &lData.pos.z, -60.0f, 60.0f, "%.1f");
		if (ImGui::Button("Reset")) {
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept {
	lData = {
		{ 0.0f,8.0f,0.0f },
		{ 0.05f,0.05f,0.05f },
		{ 1.0f,1.0f,1.0f },
		1.0f,
		1.0f,
		0.045f,
		0.0075f,
	};
}

void PointLight::Draw(Graphics& gfx) const {
	mesh.SetPos(lData.pos);
	mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept {
	auto dataCopy = lData;
	const auto pos = DirectX::XMLoadFloat3(&lData.pos);
	DirectX::XMStoreFloat3(&dataCopy.pos, DirectX::XMVector3Transform(pos, view));
	cbuf.Update(gfx, dataCopy);
	cbuf.Bind(gfx);
}