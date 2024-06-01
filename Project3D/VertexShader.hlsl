/*struct VSOut
{
    float3 color : Color;
    float4 pos : SV_Position;
};

cbuffer CBuf
{
    row_major matrix transfrom;
};

VSOut main(float2 pos : Position, float3 color : Color)
{
    VSOut vso;
    vso.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transfrom);
    vso.color = color;
    return vso;
}*/
cbuffer CBuf {
    matrix transform;
};

float4 main(float3 pos : Position) : SV_Position {
    return mul(float4(pos, 1.0f), transform);
}