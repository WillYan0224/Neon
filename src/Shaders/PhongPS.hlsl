cbuffer LightCBuf : register(b0)
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer ObjectCBuf
{
    float3 materialColor;
    float  specularIntensity;
    float  specularPower;
};

float4 main(float3 cameraPos : Position, float3 n : Normal) : SV_Target
{
	// fragment to light vector data
    const float3 vToL = lightPos - cameraPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
	// diffuse attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));
    // reflect along normal
    const float3 r = reflect(vToL, n);
    // specular based on view vector and reflected vector
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(cameraPos), normalize(-r))), specularPower);
	// final color
    return float4(saturate((diffuse + ambient + specular) * materialColor), 1.0f);
}