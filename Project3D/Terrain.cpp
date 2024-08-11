#include "Terrain.h"
#define SwapByte(x) (x << 8) | ((x >> 8) & 0xFF)

Terrain::Terrain(Graphics& gfx, const std::string& heightmapFile, Camera& cam) 
    : cam(cam),
      gfx(gfx) {
    LoadHeightmap(heightmapFile);
}

void Terrain::LoadHeightmap(const std::string& heightmapFile) {
    std::ifstream file(heightmapFile, std::ios::binary);
    if (!file.is_open()) { throw ExceptionHandle(__LINE__, __FILE__); }

    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    int dimension;
    if (fileSize == 1201 * 1201 * sizeof(int16_t)) { dimension = 1201; }
    else if (fileSize == 3601 * 3601 * sizeof(int16_t)) { dimension = 3601; }
    else { throw ExceptionHandle(__LINE__, __FILE__); }

    heightmap.resize(dimension, std::vector<float>(dimension));

    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            int16_t height;
            file.read(reinterpret_cast<char*>(&height), sizeof(int16_t));
            if (!file) { throw ExceptionHandle(__LINE__, __FILE__); }
            // Correct the byte order
            height = static_cast<int16_t>(SwapByte(static_cast<uint16_t>(height)));

            if (height == -32768) {
                height = 0; // Handle voids in SRTM data
            }
            heightmap[i][j] = static_cast<float>(height);
        }
    }
    file.close();
}

void Terrain::TerrainChunk::CreateMesh(const std::vector<std::vector<float>>& heightmap, int chunkSize, float spacing) {
    int vertexCount = (chunkSize + 1) * (chunkSize + 1);
    for (int i = 0; i <= chunkSize; ++i) {
        for (int j = 0; j <= chunkSize; ++j) {
            float x = j * spacing;
            float z = i * spacing;

            Vertex vertex;
            vertex.pos = DirectX::XMFLOAT3(x, heightmap[i][j], z);
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
    AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));
    // Create index buffer
    AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

    auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
    auto pvsbc = pvs->GetByteCode();
    AddStaticBind(std::move(pvs));
    AddStaticBind(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));

    const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
        {"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
    };
    AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
    AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

    struct PSMaterialConstant {
        DirectX::XMFLOAT3 color;
        float specularIntensity = 0.6f;
        float specularPower = 30.0f;
        float padding[3];
    } pmc;
    pmc.color = { 1.0f,1.0f,1.0f };
    AddStaticBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, pmc, 1u));

    AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

Terrain::TerrainChunk::TerrainChunk(Graphics& gfx, const std::string& heightmapFile) 
    : gfx(gfx) {
}

DirectX::XMMATRIX Terrain::TerrainChunk::GetTransformXM() const noexcept {
    return DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
}

void Terrain::Update(float dt) noexcept {
    DirectX::XMVECTOR camPosVec = DirectX::XMVectorSet(cam.GetPos().x, cam.GetPos().y, cam.GetPos().z, 0.0f);
    DirectX::XMVECTOR oldCamPosVec = DirectX::XMVectorSet(cameraPos.x, cameraPos.y, cameraPos.z, 0.0f);
    DirectX::XMVECTOR delta = DirectX::XMVectorSubtract(camPosVec, oldCamPosVec);
    float distanceSq = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(delta));

    if (distanceSq > REBUILD_THRESHOLD * REBUILD_THRESHOLD) {
        cameraPos = cam.GetPos();
        DirectX::XMFLOAT3 newChunkPos = { cameraPos.x - ((int)cameraPos.x % CHUNK_SIZE), 0.0f, cameraPos.z - ((int)cameraPos.z % CHUNK_SIZE) };
        bool haveChunk = 0;
        for (auto& a : activeChunks) {
            if (a->chunkPos.x == newChunkPos.x && a->chunkPos.z == newChunkPos.z){
                haveChunk = 1;
            }
            a->Draw(gfx);
        }
        if (haveChunk == 1) return;
        auto newChunk = std::make_unique<TerrainChunk>(gfx, "Scenery\\N29E121.hgt");
        newChunk->chunkPos = newChunkPos;
        newChunk->CreateMesh(heightmap, CHUNK_SIZE, spacing);
        activeChunks.insert(activeChunks.begin(), std::move(newChunk));
        return;
    }
    for (auto a = activeChunks.begin(); a != activeChunks.end(); a++) {
        DirectX::XMVECTOR chunkPosVec = DirectX::XMVectorSet((*a)->chunkPos.x, (*a)->chunkPos.y, (*a)->chunkPos.z, 0.0f);
        DirectX::XMVECTOR delta = DirectX::XMVectorSubtract(chunkPosVec, camPosVec);
        float distanceSq = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(delta));
        if (distanceSq > MAX_LOAD_DIST * MAX_LOAD_DIST) {
            a = activeChunks.erase(a);
            continue;
        }
        (*a)->Draw(gfx);
    }
}