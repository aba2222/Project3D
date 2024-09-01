#include "Terrain.h"
#define SwapByte(x) (x << 8) | ((x >> 8) & 0xFF)
#define DistFloor(x) (x - x % MAX_LOAD_DIST)

Terrain::Terrain(AppBlock& appBlock, const std::string& heightmapFile, Camera& cam)
    : ManagerBase(appBlock) {
}

void TerrainChunk::CreateMesh(int chunkSize, float spacing) {
    LoadFile((pos->longitude - 121) * 3600, (pos->latitude - 29) * 3600, "Scenery\\N29E121.hgt", MAX_LOAD_DIST / 30);
    int vertexCount = (chunkSize + 1) * (chunkSize + 1);
    for (int i = 0; i <= chunkSize; ++i) {
        for (int j = 0; j <= chunkSize; ++j) {
            float x = j * spacing;
            float z = i * spacing;

            Vertex vertex;
            vertex.pos = DirectX::XMFLOAT3(x, heightMap[i][j], z);
            vertex.normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
            vertices.push_back(vertex);

            vertexIndexMap[{i, j}] = vertices.size() - 1; // 存储顶点的索引
        }
    }

    // Generate indices
    for (int i = 0; i < chunkSize; ++i) {
        for (int j = 0; j < chunkSize; ++j) {
            int topLeft = vertexIndexMap[{i, j}];
            int topRight = vertexIndexMap[{i, j + 1}];
            int bottomLeft = vertexIndexMap[{i + 1, j}];
            int bottomRight = vertexIndexMap[{i + 1, j + 1}];

            // 顶点索引
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);

            DirectX::XMVECTOR v0 = DirectX::XMLoadFloat3(&vertices[topLeft].pos);
            DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&vertices[bottomLeft].pos);
            DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&vertices[topRight].pos);
            DirectX::XMVECTOR normal1 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(v1, v0), DirectX::XMVectorSubtract(v2, v0));

            v0 = DirectX::XMLoadFloat3(&vertices[topRight].pos);
            v1 = DirectX::XMLoadFloat3(&vertices[bottomLeft].pos);
            v2 = DirectX::XMLoadFloat3(&vertices[bottomRight].pos);
            DirectX::XMVECTOR normal2 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(v1, v0), DirectX::XMVectorSubtract(v2, v0));

            normal1 = DirectX::XMVectorAdd(normal1, normal2);

            DirectX::XMFLOAT3 normalFloat3;
            DirectX::XMStoreFloat3(&normalFloat3, normal1);

            vertices[topLeft].normal.x += normalFloat3.x;
            vertices[topLeft].normal.y += normalFloat3.y;
            vertices[topLeft].normal.z += normalFloat3.z;

            vertices[bottomLeft].normal.x += normalFloat3.x;
            vertices[bottomLeft].normal.y += normalFloat3.y;
            vertices[bottomLeft].normal.z += normalFloat3.z;

            vertices[topRight].normal.x += normalFloat3.x;
            vertices[topRight].normal.y += normalFloat3.y;
            vertices[topRight].normal.z += normalFloat3.z;

            vertices[bottomRight].normal.x += normalFloat3.x;
            vertices[bottomRight].normal.y += normalFloat3.y;
            vertices[bottomRight].normal.z += normalFloat3.z;
        }
    }
    for (auto& vertex : vertices) {
        DirectX::XMVECTOR normal = DirectX::XMLoadFloat3(&vertex.normal);
        normal = DirectX::XMVector3Normalize(normal);
        DirectX::XMStoreFloat3(&vertex.normal, normal);
    }

    // Create vertex buffer
    AddBind(std::make_unique<VertexBuffer>(appBlock.gfx, vertices));
    // Create index buffer
    AddIndexBuffer(std::make_unique<IndexBuffer>(appBlock.gfx, indices));

    auto pvs = std::make_unique<VertexShader>(appBlock.gfx, L"PhongVS.cso");
    auto pvsbc = pvs->GetByteCode();
    AddBind(std::move(pvs));
    AddBind(std::make_unique<PixelShader>(appBlock.gfx, L"PhongPS.cso"));

    const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
        {"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
    };
    AddBind(std::make_unique<InputLayout>(appBlock.gfx, ied, pvsbc));
    AddBind(std::make_unique<Topology>(appBlock.gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

    struct PSMaterialConstant {
        DirectX::XMFLOAT3 color;
        float specularIntensity = 0.6f;
        float specularPower = 30.0f;
        float padding[3];
    } pmc;
    pmc.color = { 1.0f,1.0f,1.0f };
    AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(appBlock.gfx, pmc, 1u));

    AddBind(std::make_unique<TransformCbuf>(appBlock.gfx, *this));
}

TerrainChunk::TerrainChunk(float startPosLon, float stratPosLat, AppBlock& appBlock)
    : appBlock(appBlock) {
    pos = std::make_unique<EarthPos>(startPosLon, stratPosLat, 0, 0, 0, 0);
}

DirectX::XMMATRIX TerrainChunk::GetTransformXM() const noexcept {
    auto localPos = appBlock.cam.GetEarthPos()->ToLocal(pos->longitude, pos->latitude, pos->altitude);
    return DirectX::XMMatrixTranslation(localPos.x, localPos.y, localPos.z);
}

void TerrainChunk::Update(float dt) noexcept {
    Draw(appBlock.gfx);
}

void TerrainChunk::LoadFile(int startX, int startY, const std::string& heightmapFile, int size) {
    infile.open(heightmapFile, std::ios::binary);
    if (!infile) { throw std::runtime_error("Faild in Open: " + heightmapFile); }
    int gridSize = 3601; 

    if (startX < 0 || startY < 0 || size <= 0 || startX + size > gridSize || startY + size > gridSize) {
        throw std::out_of_range("Requested area exceeds grid range");
    }

    heightMap.resize(size);
    for (int y = 0; y < size; ++y) {
        heightMap[y].resize(size);
    }

    const std::streamsize bytesPerRow = gridSize * sizeof(int16_t);
    const std::streamsize bytesToRead = size * sizeof(int16_t);

    for (int y = 0; y < size; ++y) {
        std::streampos offset = static_cast<std::streampos>((startY + y) * gridSize + startX) * sizeof(int16_t);
        infile.seekg(offset, std::ios::beg);
        if (!infile) {
            throw std::runtime_error("Seekg failed，spacing: " + std::to_string(offset));
        }

        infile.read(reinterpret_cast<char*>(heightMap[y].data()), bytesToRead);
        if (infile.gcount() != bytesToRead) {
            throw std::runtime_error("Read data failed，expected number of bytes read: " + std::to_string(bytesToRead));
        }

        for (int x = 0; x < size; ++x) {
            heightMap[y][x] = SwapByte(heightMap[y][x]);
        }
    }
}

void Terrain::Update() {
    EarthPos* nowPos = appBlock.cam.GetEarthPos();
    bool haveChunk = 0;
    for (auto a = classList.begin(); a != classList.end();) {
        DirectX::XMFLOAT3 localPos = nowPos->ToLocal((*a)->GetEarthPos());
        if (localPos.x > MAX_LOAD_DIST  * 10|| localPos.y > MAX_LOAD_DIST * 10 || localPos.z > MAX_LOAD_DIST * 10) {
            a->release();
            a = classList.erase(a);
            continue;
        }
        if ((*a)->GetEarthPos()->longitude == nowPos->longitude - fmod(nowPos->longitude, MAX_LOAD_DIST_LON) &&
            (*a)->GetEarthPos()->latitude == nowPos->latitude - fmod(nowPos->latitude, MAX_LOAD_DIST_LON)) {
            haveChunk = 1;
        }
        (*a)->Draw(appBlock.gfx);

        a++;
    }
    if (haveChunk == 1) return;
    auto newChunk = std::make_unique<TerrainChunk>(nowPos->longitude - fmod(nowPos->longitude, MAX_LOAD_DIST_LON),
                                                   nowPos->latitude - fmod(nowPos->latitude, MAX_LOAD_DIST_LON), appBlock);
    newChunk->CreateMesh(CHUNK_SIZE, spacing);
    classList.insert(classList.begin(), std::move(newChunk));
    return;
}