#ifndef DEFAULT_SHADER_HLSL
#define DEFAULT_SHADER_HLSL

#include "NormalMap.hlsl"
#include "Light.hlsl"
#include "cbuffer.hlsl"
#include "Shadow.hlsl"

struct VS_TEXTURE_INPUT {
	float3 vertexPosition : POSITION;
	float2 vertexUV : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	uint   matIndex : MATINDEX;
	uint4	boneIndices : BONEINDEX;
	float4	weights : WEIGHT;
};

struct VS_TEXTURE_OUTPUT {
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float2 uv : TEXCOORD;
	float3 worldNormal : NORMAL;
	float3 worldTangent : TANGENT;
	float4 shdowPosition : SHADOW_POSITION;
	uint matIndex : MATINDEX;
};

struct PS_FORWARD_OUTPUT {
	float4 color : SV_TARGET0;
	float4 norm : SV_TARGET1;
};

PS_FORWARD_OUTPUT packing(float4 color, float3 normal) {
	const float4 addNorm = float4(1.0f, 1.0f, 1.0f, 1.0f);

	PS_FORWARD_OUTPUT result;
	result.color = color;
	result.norm = (float4(normal, 1.0f) + addNorm) * 0.5f;
	
	return result;
}

VS_TEXTURE_OUTPUT VS(VS_TEXTURE_INPUT vsInput, uint vertexID : SV_VertexID)
{
	VS_TEXTURE_OUTPUT output;
	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float3 posL		= float3(0.0f, 0.0f, 0.0f);
	float3 normalL	= float3(0.0f, 0.0f, 0.0f);
	float3 tanL		= float3(0.0f, 0.0f, 0.0f);

	weights[0] = vsInput.weights.x;
	weights[1] = vsInput.weights.y;
	weights[2] = vsInput.weights.z;
	weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

	for (int i = 0; i < 4; ++i) {
		posL	+= weights[i] * mul(float4(vsInput.vertexPosition, 1.0f), objBoneMat[vsInput.boneIndices[i]]).xyz;
		normalL += weights[i] * mul(vsInput.normal, (float3x3)objBoneMat[vsInput.boneIndices[i]]).xyz;
		tanL	+= weights[i] * mul(vsInput.tangent, (float3x3)objBoneMat[vsInput.boneIndices[i]]).xyz;
	}

	float4 vertexPosition = float4(vsInput.vertexPosition, 1.0f);
	
	output.worldNormal = mul(normalL, (float3x3)objWorld);
	output.worldPosition = mul(float4(posL, 1.0f), objWorld);
	output.position = mul(mul(mul(float4(posL, 1.0f), objWorld), camView), camProjection);
	output.worldTangent = mul(float4(tanL, 1.0f), objWorld).xyz;
	output.uv = vsInput.vertexUV;

	output.matIndex = vsInput.matIndex;
	output.shdowPosition = mul(output.worldPosition, camShadow);

	return output;
}

PS_FORWARD_OUTPUT PS(VS_TEXTURE_OUTPUT psInput)
{
	float3 normal = SR_TEXTURE[NonUniformResourceIndex((psInput.matIndex * NUM_USED_MAP) + 1)].Sample(DEFAULT_SAMPLER, psInput.uv).xyz;
	float4 color = SR_TEXTURE[NonUniformResourceIndex(psInput.matIndex * NUM_USED_MAP)].Sample(DEFAULT_SAMPLER, psInput.uv);
	
	normal = CalculateNormalMap(normal, psInput.worldTangent, psInput.worldNormal);

	return packing(color, normal);
}


VS_TEXTURE_DEBUG_OUT VSTextureDebug(uint vertexID : SV_VertexID)
{
	VS_TEXTURE_DEBUG_OUT vsout;
	vsout.position = defaultVSOut[vertexID];
	vsout.position.y = (vsout.position.y > -0.3f) ? -0.3f : vsout.position.y;
	vsout.uv.xy = defaultVSOutUV[vertexID];
	
	return vsout;
}

float4 PSTextureDebug(VS_TEXTURE_DEBUG_OUT psInput) : SV_TARGET
{
	float index = floor(psInput.uv.x * NUM_TEXTURE);

	float2 uv =	psInput.uv;
	uv.x = saturate((uv.x * NUM_TEXTURE) - index);
	
	float4 color = SR_TEXTURE[NonUniformResourceIndex((uint)index)].Sample(DEFAULT_SAMPLER, uv);
	if ((uint)index > 1) {
		color = color.rrra;
		
	}
	return color;
}

#endif