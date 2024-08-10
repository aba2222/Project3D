#pragma once
#include <string>
#include <fstream>
#include <unordered_map>
#include <DirectXMath.h>
#include "ExceptionHandle.h"
#include "Graphics.h"
#include "ExceptionHandle.h"
#include "TransformCbuf.h"
#include "DrawableBase.h"
#include "Camera.h"
#define MAX_LOAD_DIST 300

class Terrain : public DrawableBase<Terrain> {
public:
	Terrain(Graphics& gfx, const std::string& heightmapFile, Camera& cam);
	void LoadHeightmap(const std::string& heightmapFile);
	void CreateTerrainMesh();
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
	Camera& cam;

	struct pair_hash {
		template <class T1, class T2>
		std::size_t operator() (const std::pair<T1, T2>& pair) const {
			return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
		}
	};
	std::unordered_map<std::pair<int, int>, int, pair_hash> vertexIndexMap;
	DirectX::XMFLOAT3 cameraPos;
	Graphics& gfx;
};