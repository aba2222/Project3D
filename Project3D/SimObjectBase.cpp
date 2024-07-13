#include "SimObjectBase.h"

SimObjectBase::SimObjectBase(Graphics& gfx, std::wstring name)
	: name(name) {}

SimObjectBase::~SimObjectBase() {}

void SimObjectBase::Update() {
}
