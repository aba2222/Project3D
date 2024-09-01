#include "Light.h"
#include "imgui/imgui.h"

Light::Light(Graphics& gfx, int type, float radius)
	: mesh(gfx, radius),
	  cbuf(gfx),
      lightType(type),
	  gfx(gfx) {
	Reset();
}

void Light::SpawnControlWindow() noexcept {
	static int selectedItem = 0;

	if (ImGui::Begin("Light")) {
		ImGui::Text("Position");

		if (ImGui::Combo("Light", &selectedItem,
			[](void* data, int idx, const char** out_text) -> bool {
				auto* items = static_cast<std::vector<PointLightCBuf>*>(data);
				if (idx < 0 || idx >= items->size()) return false;
				*out_text = (*items)[idx].name;
				return true;
			},
			&lights, lights.size())) {
		}

		ImGui::SliderFloat("X", &lights[selectedItem].pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &lights[selectedItem].pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &lights[selectedItem].pos.z, -60.0f, 60.0f, "%.1f"); 
		ImGui::SliderFloat("DI", &lights[selectedItem].diffuseIntensity, 0.0f, 1.0f, "%.1f");
		if (ImGui::Button("Reset")) {
			Reset();
		}
	}
	ImGui::End();
}

void Light::Reset() noexcept {
	lights.clear();
	lights.push_back({
		{ 0.0f,1.0f,0.0f } , 0.0f,
		{ 0.05f,0.05f,0.05f }, 0.0f,
		{ 1.0f,1.0f,1.0f }, 0.0f,
		0.6f,
		1.0f,
		0.045f,
		0.0075f,
		lightType,
		"sun",
		0.0f,
	});
}

void Light::Update(float dt) noexcept {
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