cbuffer CBuf : register(b0)
{
    matrix modelView;
    matrix modelViewProj;  
};

float4 main(float3 pos : POSITION) : SV_Position
{
    return mul(float4(pos, 1.0f), modelViewProj);
}