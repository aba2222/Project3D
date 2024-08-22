#pragma once
#include <string>
#include <fstream>
#include <unordered_map>
#include <DirectXMath.h>
#include <memory>
#include "ExceptionHandle.h"
#include "Graphics.h"
#include "ExceptionHandle.h"
#include "TransformCbuf.h"
#include "DrawableBase.h"
#include "Camera.h"
#define MAX_LOAD_DIST 300
#define REBUILD_THRESHOLD 30
#define CHUNK_SIZE 30

class TerrainChunk : public DrawableBase<TerrainChunk> {
public:
	TerrainChunk(Graphics& gfx, const std::string& heightmapFile, std::unique_ptr<Camera> cam);
	DirectX::XMMATRIX GetTransformXM() const noexcept;
	void Update(float dt) noexcept;
	void CreateMesh(const std::vector<std::vector<float>>& heightmap, int chunkSize, float spacing);

	struct pair_hash {
		template <class T1, class T2>
		std::size_t operator() (const std::pair<T1, T2>& pair) const {
			return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
		}
	};

protected:
	static std::unique_ptr<Camera> cam;
	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
	};
	DirectX::XMFLOAT3 localPos;
	Graphics& gfx;
	std::unordered_map<std::pair<int, int>, int, pair_hash> vertexIndexMap;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
};

class Terrain {
friend class TerrainChunk;
public:
	Terrain(Graphics& gfx, const std::string& heightmapFile, Camera& cam);
	void LoadHeightmap(const std::string& heightmapFile);
	void Update(float dt) noexcept;

private:
	float spacing = 30.0f;
	Graphics& gfx;
	Camera& cam;
	float lastCamLat;
	float lastCamLon;
	float lastCamAtt;
	std::list<std::unique_ptr<TerrainChunk>> activeChunks;
};	