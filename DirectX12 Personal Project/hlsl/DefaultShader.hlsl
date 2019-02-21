#ifndef DEFAULT_SHADER_HLSL
#define DEFAULT_SHADER_HLSL

#define NUM_TEXTURE 5

cbuffer CB_CAM_INFO : register(b0)
{
	matrix camProjection;
	matrix camView;
	matrix camShadow;
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
sampler SHADOWMAP_SAMPLER : register(s1);

StructuredBuffer<OBJ_INFO> INST_OBJ_INFO : register(t1);

Texture2D SR_TEXTURE[NUM_TEXTURE] : register(t2);

static const uint NUM_USED_MAP = 2;

static float4 defaultVSOut[6] = {
	{ -1.0f, -1.0f, 0.0f, 1.0f },
	{ -1.0f, +1.0f, 0.0f, 1.0f },
	{ +1.0f, -1.0f, 0.0f, 1.0f },
	{ +1.0f, -1.0f, 0.0f, 1.0f },
	{ -1.0f, +1.0f, 0.0f, 1.0f },
	{ +1.0f, +1.0f, 0.0f, 1.0f }
};

static float2 defaultVSOutUV[6] = {
	{ 0.0f,  1.0f },
	{ 0.0f,  0.0f },
	{ 1.0f,  1.0f },
	{ 1.0f,  1.0f },
	{ 0.0f,  0.0f },
	{ 1.0f,  0.0f }
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

float4 PSShadow(VS_TEXTURE_OUTPUT psInput) : SV_TARGET
{
	return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

float4 VSTextureFullScreen(uint vertexID : SV_VertexID) : SV_POSITION
{
	return defaultVSOut[vertexID];
}

float4 PSTextureFullScreen(float4 psInput : SV_POSITION) : SV_TARGET
{
	return SR_TEXTURE[0][psInput.xy];
}

struct VS_TEXTURE_DEBUG_OUT {
	float4 position : SV_POSITION;
	float4 pos : POSITION;
	float2 uv : UV;
};

VS_TEXTURE_DEBUG_OUT VSTextureDebug(uint vertexID : SV_VertexID)
{
	VS_TEXTURE_DEBUG_OUT vsout;
	vsout.position = defaultVSOut[vertexID];
	vsout.position.y = (vsout.position.y > -0.3f) ? -0.3f : vsout.position.y;
	vsout.pos = vsout.position;
	vsout.uv.xy = defaultVSOutUV[vertexID];
	
	return vsout;
}

float4 PSTextureDebug(VS_TEXTURE_DEBUG_OUT psInput) : SV_TARGET
{
	float index = floor(psInput.uv.x * NUM_TEXTURE);

	float2 uv =	psInput.uv;
	uv.x = saturate((uv.x * NUM_TEXTURE) - index);
	
	float4 color = SR_TEXTURE[NonUniformResourceIndex((uint)index)].Sample(DEFAULT_SAMPLER, uv);
	if ((uint)index == 1) {
		color = color.rrra;
		
	}
	return color;
}

#endif