#include <Windows.h>
#include <sstream>
#include "Window.h"
#include "ExceptionHandle.h"
#include "App.h"

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {
	try {
		return App{}.Go();
	}
	catch (const ExceptionHandle& e) {
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e) {
		MessageBox(nullptr, e.what(), "Std Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) {
		MessageBox(nullptr, "Unknown", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}