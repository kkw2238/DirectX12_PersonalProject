float4 VS(uint vertexID : SV_VertexID) : SV_POSITION
{
	if (vertexID == 0)
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	if (vertexID == 1)
		return float4(0.0f, 1.0f, 0.0f, 0.0f);
	if (vertexID == 2)
		return float4(1.0f, 0.0f, 0.0f, 0.0f);
	if (vertexID == 3)
		return float4(1.0f, 0.0f, 0.0f, 0.0f);
	if (vertexID == 4)
		return float4(0.0f, 1.0f, 0.0f, 0.0f);
	if (vertexID == 5)
		return float4(1.0f, 1.0f, 0.0f, 0.0f);

	return float4(0.0f, 0.0f, 0.0f, 0.0f);
}

float4 PS(float4 input : SV_POSITION) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 0.0f);
}