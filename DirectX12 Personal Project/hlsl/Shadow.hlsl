#ifndef SHADOW_HLSL
#define SHADOW_HLSL

#include "Sampler.hlsl"
#include "ShaderResource.hlsl"

float CalculateShadowFactor(float4 shadowPosition) {
	float3 shadowPerspective = shadowPosition.xyz / shadowPosition.w;

	uint width, height, numMips;
	SR_TEXTURE[NUM_TEXTURE - 1].GetDimensions(0, width, height, numMips);

	float offset = 1.0f / (float)width;
	float factor = 0.0f;

	const float2 offsetxy[9] = {
		float2(-offset, -offset),	float2(0, -offset), float2(offset, -offset),
		float2(-offset, 0),			float2(0, 0),		float2(offset, 0),
		float2(-offset, offset),	float2(0, offset),	float2(offset, offset)
	};

	for (int i = 0; i < 9; ++i) {
		factor += SR_TEXTURE[NUM_TEXTURE - 1].SampleCmpLevelZero(SHADOWMAP_SAMPLER, shadowPerspective.xy + offsetxy[i], shadowPerspective.z).r;
	}

	return factor / 9.0f;
};


#endif