#pragma once

/*
 * Structchers.h 파일에는 현재 프로젝트에서 사용 할 Cbuffer관련 struct를 모아뒀음.
 */

#include "MathUtil.h"
#define MAX_LIGHT 2

struct CB_CAMERA_INFO {
	Matrix4x4 matProjection;
	Matrix4x4 matView;
	Matrix4x4 shadowMat;
	Matrix4x4 matInvView;
	Vector3	  position;
};

struct CB_OBJ_INFO {
	Matrix4x4 matWorld;
};

struct IA_TEXTURE_OBJ {
	IA_TEXTURE_OBJ() {};
	IA_TEXTURE_OBJ(Vector3& vertex, Vector2& texCoord, Vector3& normal, Vector3& tangent, UINT matindex) :
		vertexPos(vertex), texCoord(texCoord), normal(normal), tangent(tangent), materialIndex(matindex) {}

	Vector3 vertexPos;
	Vector2 texCoord;
	Vector3 normal;
	Vector3 tangent;
	UINT	materialIndex;
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