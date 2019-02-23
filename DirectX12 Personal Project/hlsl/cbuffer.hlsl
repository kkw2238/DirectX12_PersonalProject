#ifndef CBUFFER_HLSL
#define CBUFFER_HLSL

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

struct VS_TEXTURE_DEBUG_OUT {
	float4 position : SV_POSITION;
	float2 uv : UV;
};


cbuffer CB_CAM_INFO : register(b0)
{
	matrix camProjection;
	matrix camView;
	matrix camShadow;
	matrix camInvView;
	float3 camPosition;
};

cbuffer CB_OBJ_INFO : register(b1)
{
	matrix objWorld;
};

#endif