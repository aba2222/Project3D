#pragma once
#include <sstream>
#include "Window.h"
#include "ProTimer.h"
#include "Graphics.h"
#include "Drawable.h"
#include "Sheet.h"
#include "SkinnedBox.h"
#include "Melon.h"
#include "Pyramid.h"
#include "ImguiManager.h"
//#define FSP_COUNT

class App {
public:
	App();
	int Go();

private:
	float speed_factor = 1.0f;
	void DoFrame();
	ImguiManager imgui;
	Window wnd;
	ProTimer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawbles = 180;
	#ifdef FSP_COUNT
		ProTimer fspTimer;
		int fsp;
	#endif // FSP_COUNT
};

