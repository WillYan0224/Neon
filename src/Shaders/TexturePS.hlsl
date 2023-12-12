Texture2D tex : register(t0);
SamplerState smplr : register(s0);

float4 main(float2 tc : TEXCOORD ) : SV_Target
{
    return tex.Sample(smplr, tc);
}