#include "EngineControl.h"

EngineControl::EngineControl(std::vector<Engine> engVec)
	: engVec(engVec),
	  engCount(engVec.size()){
}

void EngineControl::SpawnControlWindow() {
	ImGui::SliderInt("t", &status, 0, 100);
	ImGui::Text("Turst:%.2f",status2);
}

void EngineControl::Update() {
	float nowThurst = 0;
	for (int i = 0; i < engCount; i++) {
		if (engVec[i].status == 2) { nowThurst += engVec[i].maxThurst * (status / 100.0f); }
	}
	status2 = nowThurst;
}