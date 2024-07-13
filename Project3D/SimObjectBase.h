#pragma once
#include<string>
#include <wtypes.h>
#include "Graphics.h"

class SimObjectBase {
public:
	SimObjectBase(Graphics& gfx, std::wstring name);
	~SimObjectBase();
	SimObjectBase(const SimObjectBase&) = delete;
	SimObjectBase& operator=(const SimObjectBase&) = delete;

	void Update();

private:
	std::wstring name;
	UINT id;
};