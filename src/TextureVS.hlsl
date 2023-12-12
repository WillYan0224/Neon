cbuffer CBuf 
{
    matrix transform;
};

struct VSOut
{
    float2 tex : TEXCOORD;
    float4 Pos : SV_Position;
};

VSOut main(float3 pos : Position, float2 tex : TEXCOORD)
{
    VSOut vsout;
    vsout.Pos = mul(float4(pos, 1.0f), transform);
    vsout.tex = tex;
    return vsout;
}