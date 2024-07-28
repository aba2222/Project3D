#include "Terrain.h"
#define SwapByte(x) (x << 8) | (x >> 8)

Terrain::Terrain(Graphics& gfx, const std::string& heightmapFile) {
    LoadHeightmap(heightmapFile);
    CreateTerrainMesh(gfx);
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

void Terrain::CreateTerrainMesh(Graphics& gfx) {
    int rows = heightmap.size();
    int cols = heightmap[0].size();

    // Generate vertices
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            Vertex vertex;
            vertex.pos = DirectX::XMFLOAT3(j * spacing, heightmap[i][j], i * spacing);
            vertex.normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
            vertices.push_back(vertex);
        }
    }

    // Generate indices
    for (int i = 0; i < rows - 1; ++i) {
        for (int j = 0; j < cols - 1; ++j) {
            int topLeft = i * cols + j;
            int topRight = i * cols + j + 1;
            int bottomLeft = (i + 1) * cols + j;
            int bottomRight = (i + 1) * cols + j + 1;

            // ¶¥µãË÷Òý
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
            DirectX::XMVECTOR v0 = DirectX::XMLoadFloat3(&vertices[topLeft].pos);
            DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&vertices[bottomLeft].pos);
            DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&vertices[topRight].pos); 
            DirectX::XMVECTOR normal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(v1, v0), 
                                                                   DirectX::XMVectorSubtract(v2, v0)));

            DirectX::XMFLOAT3 normalFloat3;
            DirectX::XMStoreFloat3(&normalFloat3, normal);

            vertices[topLeft].normal.x += normalFloat3.x;
            vertices[topLeft].normal.y += normalFloat3.y;
            vertices[topLeft].normal.z += normalFloat3.z;

            vertices[bottomLeft].normal.x += normalFloat3.x;
            vertices[bottomLeft].normal.y += normalFloat3.y;
            vertices[bottomLeft].normal.z += normalFloat3.z;

            vertices[topRight].normal.x += normalFloat3.x;
            vertices[topRight].normal.y += normalFloat3.y;
            vertices[topRight].normal.z += normalFloat3.z;

            v0 = DirectX::XMLoadFloat3(&vertices[topRight].pos);
            v1 = DirectX::XMLoadFloat3(&vertices[bottomLeft].pos);
            v2 = DirectX::XMLoadFloat3(&vertices[bottomRight].pos); 
            normal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(v1, v0), 
                                                 DirectX::XMVectorSubtract(v2, v0)));

            DirectX::XMStoreFloat3(&normalFloat3, normal);

            vertices[topRight].normal.x += normalFloat3.x;
            vertices[topRight].normal.y += normalFloat3.y;
            vertices[topRight].normal.z += normalFloat3.z;

            vertices[bottomLeft].normal.x += normalFloat3.x;
            vertices[bottomLeft].normal.y += normalFloat3.y;
            vertices[bottomLeft].normal.z += normalFloat3.z;

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

DirectX::XMMATRIX Terrain::GetTransformXM() const noexcept {
    return DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
}

void Terrain::Update(float dt) noexcept {
}
