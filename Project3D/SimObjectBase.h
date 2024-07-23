#pragma once
#include<string>
#include <wtypes.h>
#include <map>
#include "DrawableBase.h"
#include "ControlBase.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "ObjForces.h"
#include "AssModel.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

class SimObjectBase {
public:
	SimObjectBase(Graphics& gfx, Keyboard& keyBoard, std::string name, float S, int m);
	~SimObjectBase();
	SimObjectBase(const SimObjectBase&) = delete;
	SimObjectBase& operator=(const SimObjectBase&) = delete;

	void Update();
	void SpawnControlWindow();
	std::unique_ptr<AssModel> GetModel();
	void AddControl(int type, std::unique_ptr<ControlBase> control);
	//1=engine,2=rudder,3=autopilot
	std::map<int, std::unique_ptr<ControlBase>> controls;

private:
	struct Pos {
		DirectX::XMFLOAT3 xyz;
		float roll;
		float pitch;
		float yaw;
	};
	std::string name;
	UINT id;
	ObjForces forces;
	std::unique_ptr<AssModel> model;
	Pos pos;
	Keyboard& kbd;
};