
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

struct VS_TEXTURE_INPUT {
	float3 vertexPosition : POSITION;
	float2 vertexUV : TEXCOORD;
};

struct VS_TEXTURE_OUTPUT {
	float4 worldPosition : SV_POSITION;
	float2 uv : TEXCOORD;
};

sampler DEFAULT_SAMPLER : register(s0);

StructuredBuffer<OBJ_INFO> INST_OBJ_INFO : register(t1);
Texture2D SR_TEXTURE : register(t2);

static float4 defaultVSOut[6] = {
	{ -1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f, -1.0f, 0.0f, 0.0f },
	{ 1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f, 0.0f }
};

VS_TEXTURE_OUTPUT VS(VS_TEXTURE_INPUT vsInput, uint vertexID : SV_VertexID)
{
	VS_TEXTURE_OUTPUT output;

	float4 world = float4(vsInput.vertexPosition, 1.0f);
	
	output.worldPosition = mul(mul(mul(world, objWorld), camView), camProjection);
	output.uv = vsInput.vertexUV;

	return output;
}

float4 PS(VS_TEXTURE_OUTPUT psInput) : SV_TARGET
{
	return SR_TEXTURE.Sample(DEFAULT_SAMPLER, float2(psInput.uv));
}

float4 VSTextureFullScreen(uint vertexID : SV_VertexID) : SV_POSITION
{
	return defaultVSOut[vertexID];
}

float4 PSTextureFullScreen(float4 psInput : SV_POSITION) : SV_TARGET
{
	return SR_TEXTURE[psInput.xy];
}