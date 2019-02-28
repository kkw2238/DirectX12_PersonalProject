cbuffer CB_MIX : register(b1)
{
	uint2 cbTexsize : packoffset(c0);
	uint cbDomain : packoffset(c0.z);
	uint cbGroupSize : packoffset(c0.w);
};

Texture2D originTexture(t0);

RWTexture2D<float4> outputTexture(u0);
RWBuffer<float> lumFactor(u1);
RWBuffer<float> testTexture(u2);

static const float4 LUM_FACTOR = float4(0.299f, 0.587f, 0.114f, 0.0f);

groupshared float sharedData[1024];

[numthreads(1024, 1, 1)]
void CalculateLumFirstPass(uint3 GroupID : SV_GroupID, uint3 GroupThreadID : SV_GroupThreadID,  uint3 DispatchThreadID : SV_DispatchThreadID )
{
	testTexture[GroupID.x * 1024 + GroupThreadID.x * 4 + 0] = 1.0f;
	testTexture[GroupID.x * 1024 + GroupThreadID.x * 4 + 1] = 1.0f;
	testTexture[GroupID.x * 1024 + GroupThreadID.x * 4 + 2] = 1.0f;
	testTexture[GroupID.x * 1024 + GroupThreadID.x * 4 + 3] = 1.0f;
}

[numthreads(64, 1, 1)]
void CalculateLumSecondPass(uint3 GroupID : SV_GroupID, uint3 GroupThreadID : SV_GroupThreadID,  uint3 DispatchThreadID : SV_DispatchThreadID)
{
}