#ifndef NORMAL_HLSL
#define NORMAL_HLSL

float3 CalculateNormalMap(float3 normalMapTexure, float3 worldTangent, float3 worldNormal) {
	float3 N = normalize(worldNormal);
	float3 T = normalize(worldTangent - dot(worldTangent, N) * N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	float3 normal = normalMapTexure * 2.0f - 1.0f;

	return mul(normal, TBN);
}
#endif