#ifndef LIGHT_HLSL
#define LIGHT_HLSL

#define MAX_LIGHT 1

struct LIGHT_INFO {
	float3 litColor;
	float litFallstart;
	float3 litPosition;
	float litFallend;
	float3 litDirection;
	float litSpotPower;
};

cbuffer CB_LIGHT_INFO : register(b2)
{
	float4 litAmbient;
	LIGHT_INFO lights[MAX_LIGHT];
};

float3 ComputeDirectionalLight(int lightIndex, float3 normal, float3 toCam, float shadowFactor) {
	LIGHT_INFO light = lights[lightIndex];

	float3 lightVec = -light.litDirection;
	return light.litColor * max(dot(lightVec, normal), 0.0f);
}

float3 CalculateLight(float3 normal, float3 objPos, float3 camPos) {
	float3 result = (float3)0.0f;
	float3 toCam = camPos - objPos;

#if (NUM_DIRECTION > 0)
	for (int i = 0; i < NUM_DIRECTION; ++i) {
		result += ComputeDirectionalLight(i, normal, toCam, 1.0f);
	}
#endif
	return result + litAmbient;
}
#endif