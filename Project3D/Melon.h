#pragma once
#include "DrawableBase.h"
#include "TransformCbuf.h"
#include "Sphere.h"
#include <random>


class Melon : public DrawableBase<Melon> {
public:
	Melon(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution <float>& adist,
		std::uniform_real_distribution <float>& ddist,
		std::uniform_real_distribution <float>& odist,
		std::uniform_real_distribution <float>& rdist,
		std::uniform_int_distribution<int>& longdist,
		std::uniform_int_distribution<int>& latdist);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	//�ٶ�
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};

