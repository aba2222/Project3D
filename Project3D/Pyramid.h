#pragma once
#include "TransformCbuf.h"
#include "BaseDrawObject.h"
#include "DrawableBase.h"
#include<random>

class Pyramid : public BaseDrawObject<Pyramid> {
public:
	Pyramid(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution <float>& adist,
		std::uniform_real_distribution <float>& ddist,
		std::uniform_real_distribution <float>& odist,
		std::uniform_real_distribution <float>& rdist,
		std::uniform_int_distribution <int>& tdist);
};