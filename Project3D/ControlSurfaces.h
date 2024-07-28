#pragma once
#include <memory>
#include "ObjForces.h"
#include "ControlBase.h"

class ControlSurfaces : public ControlBase {
public:
	ControlSurfaces(float S, ObjForces& forces);
	void SpawnControlWindow() override;
	void Update() override;
	//1=roll 2=pitch

private:
	float S;
	ObjForces& forces;
};