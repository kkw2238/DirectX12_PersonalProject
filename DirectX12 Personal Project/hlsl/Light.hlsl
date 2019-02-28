#ifndef LIGHT_HLSL
#define LIGHT_HLSL

#define MAX_LIGHT 2

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

float ComputePongFactor(float3 lightPos, float3 objPos, float3 normal) {
	float3 toLight = lightPos - objPos;
	toLight /= length(toLight);

	return saturate(dot(normal, toLight));
}

float3 ComputePointLight(int lightIndex, float3 objPos, float3 normal, float shadowFactor) {
	LIGHT_INFO light = lights[lightIndex];
	float3 toLight = light.litPosition - objPos;
	float dist = length(toLight);
	float pongFactor = ComputePongFactor(light.litPosition, objPos, normal);
	
	if (dist < light.litFallstart)
		return light.litColor * pongFactor;

	else if (dist < light.litFallend)
		return lerp(light.litColor, float3(0.0f, 0.0f, 0.0f), min(1.0f, (dist - light.litFallstart) / (light.litFallend - light.litFallstart))) * pongFactor;

	return float3(0.0f, 0.0f, 0.0f);
}


float3 CalculateLight(float3 normal, float3 objPos, float3 camPos, float shadowFactor) {
	float3 result = (float3)0.0f;
	float3 toCam = camPos - objPos;

#if (NUM_DIRECTION > 0)
	for (int i = 0; i < NUM_DIRECTION; ++i) {
		result += ComputeDirectionalLight(i, normal, toCam, shadowFactor);
	}
#endif

#if (NUM_POINT > 0)
	for (int i = NUM_DIRECTION; i < MAX_LIGHT; ++i) {
		result += ComputePointLight(i, objPos, normal, shadowFactor);
	}
#endif
	return result + litAmbient;
}
#endif