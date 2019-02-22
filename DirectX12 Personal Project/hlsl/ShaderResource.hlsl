#ifndef SHADER_RESOURCE_HLSL
#define SHADER_RESOURCE_HLSL

struct OBJ_INFO {
	matrix objWorld;
};

#define NUM_TEXTURE 5

static const uint NUM_USED_MAP = 2;

StructuredBuffer<OBJ_INFO> INST_OBJ_INFO : register(t1);
Texture2D SR_TEXTURE[NUM_TEXTURE] : register(t2);

#endif