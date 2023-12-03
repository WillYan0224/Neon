cbuffer cb
{
    row_major matrix transform;
};

struct VSOut
{
    float4 pos : SV_Position;
    float3 color : COLOR;
};

VSOut main(float2 pos : POSITION, float3 color : COLOR)
{
    VSOut vsOut = (VSOut)0;
    vsOut.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
    vsOut.color = color;
    return vsOut;
}