#pragma once
#include "Graphics.h"
#include "Drawable.h"
#include "SimObjectBase.h"

class Camera : public Updatable {
public:
	Camera();
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void Update(float dt) noexcept;
	void Reset() noexcept;
	DirectX::XMMATRIX GetProjectionMatrix(float aspectRatio) const noexcept;
	void SetFocus(SimObjectBase* newF);

private:
	SimObjectBase* focus;
};