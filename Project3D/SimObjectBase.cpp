#include "SimObjectBase.h"
#include "EngineControl.h"
#include "ControlSurfaces.h"

SimObjectBase::SimObjectBase(Graphics& gfx, Keyboard& keyBoard, std::string name, float S, int m)
	: name(name),
	 forces(S, m, this),
	 id(0),
     kbd(keyBoard),
	 gfx(gfx) {
	pos = std::make_unique<EarthPos>(DSMTD(121.0f, 27.8f, 0.0f), DSMTD(29.0f, 49.6f, 00.0f), 500.0f, 0.0f, 0.0f, 0.0f);
	model = std::make_unique<AssModel>(gfx, "Models\\320.obj", DirectX::XMFLOAT3{ 1,1,1 },1.0f);
	std::vector<EngineControl::Engine> engVec;
	engVec.push_back({ 564,2,120000 ,0 });
	engVec.push_back({ 564,2,120000 ,0 });
	AddControl(1, std::make_unique<EngineControl>(engVec));
	AddControl(2, std::make_unique<ControlSurfaces>(S, forces));
}

SimObjectBase::~SimObjectBase() {}

void SimObjectBase::Update(float dt) noexcept {
	if (controls[1] != nullptr) {
		if (kbd.KeyIsPressed(VK_F3)) { controls[1]->IncStatus(+1); }
		else if (kbd.KeyIsPressed(VK_F2)) { controls[1]->IncStatus(-1); }
		forces.SetThrust(controls[1]->GetStatus2());
	}
	for (int i = 1; i <= controls.size(); i++) {
		controls[i]->Update();
	}
	forces.Update();
	model->Draw(gfx);
}

void SimObjectBase::SpawnControlWindow() {
	if (ImGui::Begin("SimObj")) {
		ImGui::Text("Name: %s   Id: %d", name.c_str(), id);
		ImGui::Text("V_X:%.2f km/h", forces.GetVkmh());
		ImGui::Text("V_Y:%.2f km/h", forces.GetV_Y() * 3.6);

		for (int i = 1; i <= controls.size(); i++) {
			controls[i]->SpawnControlWindow();
		}
	}
	ImGui::End();
}

std::unique_ptr<AssModel> SimObjectBase::GetModel() {
	return std::move(model);
}

void SimObjectBase::AddControl(int type, std::unique_ptr<ControlBase> control) {
	controls[type] = std::move(control);
}