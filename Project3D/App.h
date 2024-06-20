#pragma once
#include <sstream>
#include "Window.h"
#include "ProTimer.h"
#include "Graphics.h"
#include "Drawable.h"
#include "Box.h"
#include "SkinnedBox.h"
#include "Melon.h"
#include "Pyramid.h"
//#define FSP_COUNT

class App {
public:
	App();
	int Go();

private:
	void DoFrame();
	Window wnd;
	ProTimer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawbles = 180;
	#ifdef FSP_COUNT
		ProTimer fspTimer;
		int fsp;
	#endif // FSP_COUNT
};

