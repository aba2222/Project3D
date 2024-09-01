#pragma once
#include "EarthPos.h"
#include <memory>

class Updatable {
public:
	Updatable() = default;
	Updatable(const Updatable&) = delete;
	virtual void Update(float dt) noexcept = 0;
	EarthPos* GetEarthPos() noexcept { return pos.get(); }

protected:
	std::unique_ptr<EarthPos> pos;
};