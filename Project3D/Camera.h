#pragma once
#include "Graphics.h"

class Camera {
public:
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
private:
	float x = 0.0f;
	float y = 0.0f;
	float z = -30.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
};