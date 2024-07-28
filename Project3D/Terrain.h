#pragma once
#include <string>
#include <fstream>
#include <DirectXMath.h>
#include "ExceptionHandle.h"
#include "Graphics.h"
#include "ExceptionHandle.h"
#include "TransformCbuf.h"
#include "DrawableBase.h"

class Terrain : public DrawableBase<Terrain> {
public:
	Terrain(Graphics& gfx, const std::string& heightmapFile);
	void LoadHeightmap(const std::string& heightmapFile);
	void CreateTerrainMesh(Graphics& gfx);
	DirectX::XMMATRIX GetTransformXM() const noexcept;
	void Update(float dt) noexcept;

	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
	};

private:
	std::vector<std::vector<float>> heightmap;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	float spacing = 30.0f;
};