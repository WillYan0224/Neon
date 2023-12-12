Texture2D tex;
SamplerState smplr;

float4 main(float2 tc : TEXCOORD ) : SV_Target
{
    return tex.Sample(smplr, tc);
}