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

class Terrain {
public:
	Terrain(Graphics& gfx, const std::string& heightmapFile, Camera& cam);
	void LoadHeightmap(const std::string& heightmapFile);
	void Update(float dt) noexcept;

	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
	};

	struct pair_hash {
		template <class T1, class T2>
		std::size_t operator() (const std::pair<T1, T2>& pair) const {
			return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
		}
	};
	class TerrainChunk : public DrawableBase<TerrainChunk> {
		friend class Terrain;
	public:
		TerrainChunk(Graphics& gfx, const std::string& heightmapFile);
		DirectX::XMMATRIX GetTransformXM() const noexcept;
		void Update(float dt) noexcept {};
		void CreateMesh(const std::vector<std::vector<float>>& heightmap, int chunkSize, float spacing);

	protected:
		Graphics& gfx;
		std::unordered_map<std::pair<int, int>, int, pair_hash> vertexIndexMap;
		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;
		DirectX::XMFLOAT3 chunkPos;
	};

private:
	std::vector<std::vector<float>> heightmap;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	float spacing = 30.0f;
	Camera& cam;
	Graphics& gfx;
	DirectX::XMFLOAT3 cameraPos;
	std::list<std::unique_ptr<TerrainChunk>> activeChunks;
};