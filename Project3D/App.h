#pragma once
#include <sstream>
#include "Window.h"
#include "ProTimer.h"
#include "Graphics.h"
#include "Box.h"
//#define FSP_COUNT

class App {
public:
	App();
	int Go();

private:
	void DoFrame();
	Window wnd;
	ProTimer timer;
	std::vector<std::unique_ptr<class Box>> boxes;
	#ifdef FSP_COUNT
		ProTimer fspTimer;
		int fsp;
	#endif // FSP_COUNT
};

