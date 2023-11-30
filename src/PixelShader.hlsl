struct VSOut
{
    float4 pos : SV_Position;
    float3 color : COLOR;
};

float4 main(VSOut vsout) : SV_TARGET
{
    return float4(vsout.color, 1.0f);
}