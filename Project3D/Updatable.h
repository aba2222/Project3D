#pragma once
#include "EarthPos.h"
#include <memory>

class Updatable {
public:
	Updatable() = default;
	Updatable(const Updatable&) = delete;
	virtual void Update(float dt) noexcept = 0;
	std::unique_ptr<EarthPos> GetEarthPos() noexcept { return std::move(pos); }

protected:
	std::unique_ptr<EarthPos> pos;
};