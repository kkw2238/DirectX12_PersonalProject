static float4 defaultVSOut[6] = {
	{ -1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f, -1.0f, 0.0f, 0.0f },
	{ 1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f, 0.0f }
};

struct BaseVertexInput {
	float3 baseInputVertex : POSITION;
};

struct BaseVertexOutput {
	float4 baseOutVertex : SV_POSITION;
	float4 TestOutVertex : POSITION;
};

BaseVertexOutput VS(BaseVertexInput input, uint vertexID : SV_VertexID)
{
	BaseVertexOutput test;
	test.baseOutVertex = float4(input.baseInputVertex.x, input.baseInputVertex.y, 0.0f, 0.0f);
	test.TestOutVertex = float4(input.baseInputVertex.x, input.baseInputVertex.y, input.baseInputVertex.z, 0.0f);
	return test;
}

float4 PS(BaseVertexOutput input) : SV_TARGET
{
	float4 data = float4(input.TestOutVertex.x / 800.0f, input.TestOutVertex.y / 600.0f, 0.0f, 0.0f);
	return data;
}