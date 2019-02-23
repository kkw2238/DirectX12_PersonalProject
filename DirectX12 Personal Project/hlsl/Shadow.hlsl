#ifndef SHADOW_HLSL
#define SHADOW_HLSL

#include "Sampler.hlsl"
#include "ShaderResource.hlsl"

float CalculateShadowFactor(float4 shadowPosition) {
	float3 shadowPerspective = shadowPosition.xyz / shadowPosition.w;

	uint width, height, numMips;
	SR_TEXTURE[TEX_SHADOW].GetDimensions(0, width, height, numMips);

	float offset = 1.0f / (float)width;
	float factor = 0.0f;

	const float2 offsetxy[25] = {
		float2(-offset * 2, -offset * 2),	float2(-offset, -offset * 2),	float2(0, -offset * 2), float2(offset, -offset * 2),	float2(offset * 2, -offset * 2),
		float2(-offset * 2, -offset),		float2(-offset, -offset),		float2(0, -offset),		float2(offset, -offset),		float2(offset * 2, -offset),
		float2(-offset * 2, 0),				float2(-offset, 0.0f),			float2(0, 0),			float2(offset, 0),				float2(offset * 2, 0),
		float2(-offset * 2, offset),		float2(-offset, offset),		float2(0, offset),		float2(offset, offset),			float2(offset * 2, offset),
		float2(-offset * 2, offset * 2),	float2(-offset, offset * 2),	float2(0, offset * 2),	float2(offset, offset * 2),		float2(offset * 2, offset * 2)
	};

	for (int i = 0; i < 25; ++i) {
		factor += SR_TEXTURE[TEX_SHADOW].SampleCmpLevelZero(SHADOWMAP_SAMPLER, shadowPerspective.xy + offsetxy[i], shadowPerspective.z).r;
	}

	return factor / 25.0f;
};


#endif