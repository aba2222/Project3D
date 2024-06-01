#include "App.h"
#include "Box.h"

App::App() 
	: wnd(800, 600, "The Fart Box") {
	#ifdef FSP_COUNT
	fsp = 0;
	#endif // FSP_COUNT;

	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 80; i++) {
		boxes.push_back(std::make_unique<Box>(wnd.Gfx(), rng, adist, ddist, odist, rdist));
	}
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

int App::Go() {
	while (1) {
		if (const auto ecode = Window::ProcessMessages()) {
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame() {
	/*const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time is:" << t << 's';
	wnd.SetTitle(oss.str());*/
	auto dt = timer.Mark();
	wnd.Gfx().ClearBuffer(0.3f, 0.3f, 0.8f);
	for (auto& b : boxes) {
		b->Update(dt);
		b->Draw(wnd.Gfx());
	}
	wnd.Gfx().EndFrame();

	#ifdef FSP_COUNT
	fsp++;
	if (fspTimer.Peek() >= 0.1) {
		std::ostringstream oss;
		oss << "fps" << fsp * 10;
		wnd.SetTitle(oss.str());
		fsp = 0;
		fspTimer.Mark();
	}
	#endif // FSP_COUNT;
}
