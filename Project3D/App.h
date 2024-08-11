#pragma once
#include <sstream>
#include "Window.h"
#include "ProTimer.h"
#include "Graphics.h"
#include "Drawable.h"
#include "Melon.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "Light.h"
#include "Box.h"
#include "Math3D.h"
#include "SimObjectBase.h"
#include "Terrain.h"
//#define FSP_COUNT

class App {
public:
	App();
	int Go();

private:
	std::unique_ptr<SimObjectBase> testObj;
	float speed_factor = 1.0f;
	void DoFrame();
	ImguiManager imgui;
	Window wnd;
	ProTimer timer;
	Camera cam;
	Light light;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawbles = 180;
	Terrain terr;
	#ifdef FSP_COUNT
		ProTimer fspTimer;
		int fsp;
	#endif // FSP_COUNT
};

