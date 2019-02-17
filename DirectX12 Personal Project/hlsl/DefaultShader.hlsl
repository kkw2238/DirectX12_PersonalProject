
cbuffer CB_CAM_INFO : register(b0)
{
	matrix camProjection;
	matrix camView;
	float3 camPosition;
};

cbuffer CB_OBJ_INFO : register(b1)
{
	matrix objWorld;
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
	uint   matIndex : MATINDEX;
};

struct VS_TEXTURE_OUTPUT {
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float2 uv : TEXCOORD;
	float3 worldNormal : NORMAL;
	float3 worldTangent : TANGENT;
	uint matIndex : MATINDEX;
};

sampler DEFAULT_SAMPLER : register(s0);

StructuredBuffer<OBJ_INFO> INST_OBJ_INFO : register(t1);

Texture2D SR_TEXTURE[4] : register(t2);

static const uint NUM_USED_MAP = 2;

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
	
	output.worldPosition = mul(world, objWorld);
	output.position = mul(mul(output.worldPosition, camView), camProjection);
	output.uv = vsInput.vertexUV;
	output.worldNormal = mul(float4(vsInput.normal, 1.0f), objWorld).xyz;
	output.worldTangent = mul(float4(vsInput.tangent, 1.0f), objWorld).xyz;
	output.matIndex = vsInput.matIndex;

	return output;
}

float4 PS(VS_TEXTURE_OUTPUT psInput) : SV_TARGET
{
	float3 normal = SR_TEXTURE[NonUniformResourceIndex((psInput.matIndex * NUM_USED_MAP) + 1)].Sample(DEFAULT_SAMPLER, psInput.uv);
	normal = CalculateNormalMap(normal, psInput.worldTangent, psInput.worldNormal);

	float4 color = SR_TEXTURE[NonUniformResourceIndex(psInput.matIndex * NUM_USED_MAP)].Sample(DEFAULT_SAMPLER, psInput.uv);

	color.xyz += CalculateLight(normal, psInput.worldPosition.xyz, camPosition);

	return color;
}

float4 VSTextureFullScreen(uint vertexID : SV_VertexID) : SV_POSITION
{
	return defaultVSOut[vertexID];
}

float4 PSTextureFullScreen(float4 psInput : SV_POSITION) : SV_TARGET
{
	return SR_TEXTURE[0][psInput.xy];
}