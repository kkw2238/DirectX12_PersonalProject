#pragma once

/*
 * Structchers.h ���Ͽ��� ���� ������Ʈ���� ��� �� Cbuffer���� struct�� ��Ƶ���.
 */

#include "MathUtil.h"
#define MAX_LIGHT 1

struct CB_CAMERA_INFO {
	Matrix4x4 matProjection;
	Matrix4x4 matView;
	Vector3	  position;
};

struct CB_OBJ_INFO {
	Matrix4x4 matWorld;
};

struct IA_TEXTURE_OBJ {
	IA_TEXTURE_OBJ() {};
	IA_TEXTURE_OBJ(Vector3& vertex, Vector2& texCoord, Vector3& normal, Vector3& tangent) :
		vertexPos(vertex), texCoord(texCoord), normal(normal), tangent(tangent) {}

	Vector3 vertexPos;
	Vector2 texCoord;
	Vector3 normal;
	Vector3 tangent;
};

struct CB_LIGHT_INFO {
	Vector3 color;
	float fallstart;
	Vector3 position;
	float fallend;
	Vector3 direction;
	float spotPower;
};

struct CB_LIGHTS {
	Vector4 ambient = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	CB_LIGHT_INFO lights[MAX_LIGHT];
};