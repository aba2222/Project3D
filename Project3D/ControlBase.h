#pragma once
#include "imgui\imgui.h"

class ControlBase {
public:
	ControlBase();
	virtual void Update() {};
	void SetStatus(int sStatus);
	void IncStatus(int inc);
	int GetStatus();
	float GetStatus2();
	virtual void SpawnControlWindow() {};
protected:
	int status = 0;
	float status2 = 0;
};