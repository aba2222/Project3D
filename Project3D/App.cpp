#include "App.h"
#include <memory>
#include <algorithm>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


App::App() 
	: wnd(800, 600, "The Fart Box"),
	  light(wnd.Gfx()) {
	Assimp::Importer imp;
	auto model = imp.ReadFile("Models\\suzanne.obj", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	class Factory {
	public:
		Factory(Graphics& gfx): gfx(gfx) {}
		std::unique_ptr<Drawable> operator()() {
			const DirectX::XMFLOAT3 mat = { cdist(rng),cdist(rng),cdist(rng) };
			switch (typedist(rng)) {
			case 0:
				return std::make_unique<Pyramid>(gfx, rng, adist, ddist, odist, rdist, tdist);
			case 1:
				return std::make_unique<Box>(gfx, rng, adist, ddist, odist, rdist, bdist, mat);
			case 2:
				return std::make_unique<SkinnedBox>(gfx, rng, adist, ddist, odist, rdist);
			case 3:
				return std::make_unique<Melon>(gfx, rng, adist, ddist, odist, rdist, longdist, latdist);
			case 4:
				return std::make_unique<Sheet>(gfx, rng, adist, ddist, odist, rdist);
			default:
				assert(false && "你这有问题啊");
				return {};
			}
		}

	private:
		Graphics& gfx;
		std::mt19937 rng{std::random_device{}()};
		std::uniform_real_distribution<float> adist{ 0.0f, PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f, PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f, PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f, 20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f, 3.0f };
		std::uniform_int_distribution<int> latdist{ 5, 20 };
		std::uniform_int_distribution<int> longdist{ 10, 40 };
		std::uniform_real_distribution<float> cdist{ 0.0f,1.0f };
		std::uniform_int_distribution<int> typedist{ 0, 2 };
		std::uniform_int_distribution<int> tdist{ 3, 30 };
	};

	#ifdef FSP_COUNT
	fsp = 0;
	#endif // FSP_COUNT;

	Factory f(wnd.Gfx());
	drawables.reserve(nDrawbles);
	std::generate_n(std::back_inserter(drawables), nDrawbles, f);

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
	auto dt = timer.Mark() * speed_factor;

	if (wnd.kbd.ReadChar() == 'g') {
		wnd.Gfx().ImguiSwitch();
	}

	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx(), cam.GetMatrix());

	for (auto& d : drawables) {
		d->Update(wnd.kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
		d->Draw(wnd.Gfx());
	}
	light.Draw(wnd.Gfx());

	if (wnd.Gfx().ImguiStatus()) {
		if (ImGui::Begin("Sim speed")) {
			ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 4.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); 
			ImGui::Text("Status: %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause)");
		}
		ImGui::End();

		cam.SpawnControlWindow();
		light.SpawnControlWindow();
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
