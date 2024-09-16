#include "App.h"
#include <memory>
#include <algorithm>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"


App::App()
	: wnd(800, 600, "3D Box"),
	  light(wnd.Gfx(), 1),
	  terr(appBlock, "Scenery\\N29E121.hgt", cam){
	#ifdef FSP_COUNT
	fsp = 0;
	#endif // FSP_COUNT;
	testObj = std::make_unique<SimObjectBase>(wnd.Gfx(), wnd.kbd, "test", 122.6, 78000);
	cam.SetFocus(testObj.get());
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
	auto dt = timer.Mark() * speed_factor;
	if (wnd.kbd.ReadChar() == 'g') {
		wnd.Gfx().ImguiSwitch();
	}

	wnd.Gfx().BeginFrame(0.275f, 0.510f, 0.706f);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	wnd.Gfx().SetProjection(cam.GetProjectionMatrix((float)wnd.GetWidth() / (float)wnd.GetHeight()));
	light.Bind(wnd.Gfx(), cam.GetMatrix());
	light.Update(0.0f);
	testObj->Update(0.0f);
	terr.Update();

	for (auto& d : updatables) {
		d->Update(wnd.kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
	}

	if (wnd.Gfx().ImguiStatus()) {
		if (ImGui::Begin("Sim speed")) {
			ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 4.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); 
			ImGui::Text("Status: %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause)");
		}
		ImGui::End();

		cam.SpawnControlWindow();
		light.SpawnControlWindow();
		testObj->SpawnControlWindow();
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
