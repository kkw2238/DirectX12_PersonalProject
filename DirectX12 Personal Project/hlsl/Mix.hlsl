cbuffer CB_MIX : register(b1)
{
	uint2 cbTexsize : packoffset(c0);
	uint cbDomain : packoffset(c0.z);
	uint cbGroupSize : packoffset(c0.w);
};

RWTexture2D<float4> outputTexture : register(u0);
RWTexture2D<float4> testTexture : register(u2);
RWBuffer<float> lumFactor : register(u1);

static const float4 LUM_FACTOR = float4(0.299f, 0.587f, 0.114f, 0.0f);

groupshared float sharedData[1024];

[numthreads(1024, 1, 1)]
void CalculateLumFirstPass(uint3 GroupID : SV_GroupID, uint3 GroupThreadID : SV_GroupThreadID,  uint3 DispatchThreadID : SV_DispatchThreadID )
{
	uint index = GroupID.x * 1024 + GroupThreadID.x;
	uint2 xy = uint2(index % 800, index / 800);

	testTexture[xy] = float4(1.0f, 1.0f, 1.0f, 1.0f);

	GroupMemoryBarrierWithGroupSync();
}

[numthreads(64, 1, 1)]
void CalculateLumSecondPass(uint3 GroupID : SV_GroupID, uint3 GroupThreadID : SV_GroupThreadID,  uint3 DispatchThreadID : SV_DispatchThreadID)
{
}