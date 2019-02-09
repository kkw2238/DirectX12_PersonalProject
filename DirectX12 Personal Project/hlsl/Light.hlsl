#ifndef LIGHT_HLSL
#define LIGHT_HLSL

#define MAX_LIGHT 1

cbuffer CB_LIGHT_INFO : register(b2)
{
	float4 litAmbient;
	LIGHT_INFO lights[MAX_LIGHT];
}

float3 CalculateLight(float3 color, float3 normal, float3 position) {
	
}
#endif