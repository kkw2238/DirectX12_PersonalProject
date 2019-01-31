
cbuffer CB_CAM_INFO : register(b0)
{
	matrix camProjection;
	matrix camView;
};

cbuffer CB_OBJ_INFO : register(b1)
{
	matrix objWorld;
};

struct OBJ_INFO {
	matrix objWorld;
};

StructuredBuffer<OBJ_INFO> INST_OBJ_INFO : register(t1);

static float4 defaultVSOut[6] = {
	{ -1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f, -1.0f, 0.0f, 0.0f },
	{ 1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f, 0.0f }
};

float4 VS(float3 vsInput : POSITION, uint vertexID : SV_VertexID) : SV_POSITION
{
	float4 world = float4(vsInput.xyz, 1.0f);
	
	return mul(mul(mul(world, objWorld), camView), camProjection);
}

float4 PS(float4 psInput : SV_POSITION) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}