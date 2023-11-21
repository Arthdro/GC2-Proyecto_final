Texture2D textures;
SamplerState colorSampler : register(s0);

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projMatrix;
	float4 valores;
};

struct VS_Input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	
	//Niebla
    float fogFactor : FOG;
};

PS_Input VS_Main(VS_Input vertex)
{
	//Niebla
    float4 vertexPos;
	
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	//Niebla
    vertexPos = vsOut.pos;
	
	vsOut.pos = mul(vsOut.pos, projMatrix);

	vsOut.tex0 = vertex.tex0;
	
		//Niebla
    float fogStart = -10.0f;
    float fogEnd = 75.0f;
	
    float fogFactor = saturate((fogEnd - vertexPos.z) / (fogEnd - fogStart));
    vsOut.fogFactor = fogFactor;

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
	float4 finalColor = textures.Sample(colorSampler, pix.tex0);
	
		//Niebla
    float4 fogColor = float4(0.412f, 0.443f, 0.612f, 1.0f);
    finalColor = pix.fogFactor * finalColor + (1.0f - pix.fogFactor) * fogColor;

	return finalColor;
}