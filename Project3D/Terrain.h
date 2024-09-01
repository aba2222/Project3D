#pragma once
#include <string>
#include <fstream>
#include <unordered_map>
#include <DirectXMath.h>
#include "ManagerBase.h"
#include <memory>
#include "ExceptionHandle.h"
#include "Graphics.h"
#include "ExceptionHandle.h"
#include "TransformCbuf.h"
#include "DrawableBase.h"
#include "Camera.h"
#define MAX_LOAD_DIST 3000
#define MAX_LOAD_DIST_LON 0.0269f
#define REBUILD_THRESHOLD 30
#define CHUNK_SIZE 100

class TerrainChunk : public DrawableBase<TerrainChunk> {
public:
	TerrainChunk(float startPosLon ,float stratPosLat, AppBlock& appBlock);
	DirectX::XMMATRIX GetTransformXM() const noexcept;
	void Update(float dt) noexcept;
	void LoadFile(int startX, int startY, const std::string& heightmapFile, int size);
	void CreateMesh( int chunkSize, float spacing);

	struct pair_hash {
		template <class T1, class T2>
		std::size_t operator() (const std::pair<T1, T2>& pair) const {
			return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
		}
	};

protected:
	std::ifstream infile;
	std::vector<std::vector<uint16_t>> heightMap;
	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
	};
	DirectX::XMFLOAT3 localPos;
	AppBlock& appBlock;
	std::unordered_map<std::pair<int, int>, int, pair_hash> vertexIndexMap;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
};

class Terrain : public ManagerBase<TerrainChunk> {
friend class TerrainChunk;
public:
	Terrain(AppBlock& appBlock, const std::string& heightmapFile, Camera& cam);
	void Update();

private:
	float spacing = 30.0f;
	float lastCamLat;
	float lastCamLon;
	float lastCamAtt;
};	