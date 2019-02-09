
cbuffer CB_CAM_INFO : register(b0)
{
	matrix camProjection;
	matrix camView;
};

cbuffer CB_OBJ_INFO : register(b1)
{
	matrix objWorld;
};

struct LIGHT_INFO {
	float3 litColor;
	float litFallstart;
	float3 litPosition;
	float litFallend;
	float3 litDirection;
	float litSpotPower;
};


#include "NormalMap.hlsl"
#include "Light.hlsl"

struct OBJ_INFO {
	matrix objWorld;
};

struct VS_TEXTURE_INPUT {
	float3 vertexPosition : POSITION;
	float2 vertexUV : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

struct VS_TEXTURE_OUTPUT {
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float2 uv : TEXCOORD;
	float3 worldNormal : NORMAL;
	float3 worldTangent : TANGENT;
};

sampler DEFAULT_SAMPLER : register(s0);

StructuredBuffer<OBJ_INFO> INST_OBJ_INFO : register(t1);
Texture2D SR_TEXTURE : register(t2);

static float4 defaultVSOut[6] = {
	{ -1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f, -1.0f, 0.0f, 0.0f },
	{ 1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f, 0.0f }
};

VS_TEXTURE_OUTPUT VS(VS_TEXTURE_INPUT vsInput, uint vertexID : SV_VertexID)
{
	VS_TEXTURE_OUTPUT output;

	float4 world = float4(vsInput.vertexPosition, 1.0f);
	
	output.position = mul(world, objWorld);
	output.worldPosition = mul(mul(output.position, camView), camProjection);
	output.uv = vsInput.vertexUV;
	output.worldNormal = mul(world, vsInput.normal);
	output.worldTangent = mul(world, vsInput.tangent);

	return output;
}

float4 PS(VS_TEXTURE_OUTPUT psInput) : SV_TARGET
{
	float3 normal = CalculateNormalMap(psInput.worldNormal, psInput.worldTangent, psInput.worldNormal);
	return float4(normal, 1.0f);
	//return SR_TEXTURE.Sample(DEFAULT_SAMPLER, float2(psInput.uv)) + litAmbient;
}

float4 VSTextureFullScreen(uint vertexID : SV_VertexID) : SV_POSITION
{
	return defaultVSOut[vertexID];
}

float4 PSTextureFullScreen(float4 psInput : SV_POSITION) : SV_TARGET
{
	return SR_TEXTURE[psInput.xy];
}