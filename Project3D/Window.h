#pragma once
#include <Windows.h>
#include "resource.h"
#include "ExceptionHandle.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <optional>
#include <memory>

class Window {
public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);
	UINT GetWidth() const noexcept;
	UINT GetHeight() const noexcept;
	static std::optional<int> ProcessMessages();
	Graphics& Gfx();
	Keyboard kbd;
	Mouse mouse;

private:
	class WindowClass {
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "Direct3D Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

	static LRESULT CALLBACK HandleMsgSteup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; 

	UINT width;
	UINT height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;
};

