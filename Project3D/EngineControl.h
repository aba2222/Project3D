#pragma once
#include "ControlBase.h"
#include <vector>

class EngineControl : public ControlBase {
public:
	struct Engine {
		//565=CFM56-5B4
		int engCode;
		//0=cut 1=starting 2=running
		unsigned short status;
		//unit:N 120000(TO/GA)
		int maxThurst;
		float N1;
	};
	EngineControl(std::vector<Engine> engVec);
	void SpawnControlWindow() override;
	void Update() override;

private:
	int engCount;
	std::vector<Engine> engVec;
};