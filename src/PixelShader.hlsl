cbuffer faceColor
{
    float4 color_face[6];
};

float4 main(uint tid : SV_PrimitiveID) : SV_TARGET
{
    return color_face[tid / 2];
}