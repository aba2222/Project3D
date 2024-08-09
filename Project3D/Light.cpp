#include "Light.h"
#include "imgui/imgui.h"

Light::Light(Graphics& gfx, int type, float radius)
	: mesh(gfx, radius),
	  cbuf(gfx),
      lightType(type) {
	Reset();
}

void Light::SpawnControlWindow() noexcept {
	if (ImGui::Begin("Light")) {
		ImGui::Text("Position");
		/*ImGui::SliderFloat("X", &lData.pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &lData.pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &lData.pos.z, -60.0f, 60.0f, "%.1f");
		if (ImGui::Button("Reset")) {
			Reset();
		}*/
	}
	ImGui::End();
}

void Light::Reset() noexcept {
	lights.push_back({
		{ 0.0f,8.0f,0.0f } , 0.0f,
		{ 0.05f,0.05f,0.05f }, 0.0f,
		{ 1.0f,1.0f,1.0f }, 0.0f,
		1.0f,
		1.0f,
		0.045f,
		0.0075f,
		lightType,
		{0.0f,0.0f,0.0f},
	});
}

void Light::Draw(Graphics& gfx) const {
	if (lightType == 0) {
		//mesh.SetPos(lData.pos);
		mesh.Draw(gfx);
	}
}

void Light::Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept {
	/*auto dataCopy = lData;
	if (lightType == 0) {
		const auto pos = DirectX::XMLoadFloat3(&lData.pos);
		DirectX::XMStoreFloat3(&dataCopy.pos, DirectX::XMVector3Transform(pos, view));
	}*/
	for (const auto& light : lights) {
		cbuf.Update(gfx, light);  // 逐个更新光源数据
	}
	cbuf.Bind(gfx);
}