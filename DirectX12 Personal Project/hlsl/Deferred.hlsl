#ifndef DEFERRED_HLSL
#define DEFERRED_HLSL

#include "ShaderResource.hlsl"
#include "Sampler.hlsl"
#include "cbuffer.hlsl"
#include "Shadow.hlsl"
#include "Light.hlsl"

struct UnpackData {
	float4 color;
	float4 worldPosition;
	float3 normal;
};

float ConvertTextureToDepth(float3 uvw) {

	float depth = SR_TEXTURE[2].Load(uvw).r;
	return (camProjection[3][2] / (depth - camProjection[2][2]));
}

float4 ConvertTextureToWorld(float2 uv, float depth) {
	float4 world = (float4)0.0f;

	world.xy = float2(uv.x * (1.0f / camProjection[0][0]), uv.y * (1.0f / camProjection[1][1]));
	world.z = depth;
	world.w = 1.0f;

	return mul(world, camInvProjection);
}

UnpackData Unpack(float2 position, float2 uv) {
	const float3 addNormal = float3(-0.5f, -0.5f, -0.5f);
	float3 uvw = float3(position, 0.0f);
	float depth = 0.0f;
	UnpackData result;

	depth = ConvertTextureToDepth(uvw);

	result.color = SR_TEXTURE[0].Load(uvw);

	result.normal = (SR_TEXTURE[1].Load(uvw).xyz + addNormal) * 2.0f;
	result.worldPosition = ConvertTextureToWorld(uv, depth);

	return result;
}

VS_TEXTURE_DEBUG_OUT VSTextureFullScreen(uint vertexID : SV_VertexID)
{
	VS_TEXTURE_DEBUG_OUT vsOutput;
	vsOutput.position = defaultVSOut[vertexID];
	vsOutput.uv = defaultVSOutUV[vertexID];
	return vsOutput;
}

float4 PSTextureFullScreen(VS_TEXTURE_DEBUG_OUT psInput) : SV_TARGET
{
	float4 color;
	UnpackData data = Unpack(psInput.position.xy, psInput.uv);
	float shadowFactor = CalculateShadowFactor(mul(data.worldPosition, camShadow));
	
	color = data.color;

	color.xyz += CalculateLight(data.normal, data.worldPosition.xyz, camPosition, shadowFactor);

	return float4(data.normal, 1.0f);
	return color;
}

#endif