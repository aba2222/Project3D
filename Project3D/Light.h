#pragma once
#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffer.h"

class Light {
public:
	Light(Graphics& gfx, int type, float radius = 0.5f);
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Draw(Graphics& gfx) const;
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept;
private:
	struct PointLightCBuf {
		DirectX::XMFLOAT3 pos;
		float padding0; // 野割
		DirectX::XMFLOAT3 ambient;
		float padding1; // 野割
		DirectX::XMFLOAT3 diffuseColor;
		float padding2; // 野割
		float diffuseIntensity;
		float attConst;  // Only point
		float attLin;  // Only point
		float attQuad;  // Only point
		int lightType;  // Light type: 0 = point 1 = directional
		float padding3[3]; // 野割
	};
	int lightType;

	std::vector<PointLightCBuf> lights;
	//PointLightCBuf lData;
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};