#pragma once

/*
 * Structchers.h ���Ͽ��� ���� ������Ʈ���� ��� �� Cbuffer���� struct�� ��Ƶ���.
 */

#include "MathUtil.h"

struct CB_CAMERA_INFO {
	Matrix4x4 matProjection;
	Matrix4x4 matView;
};

struct CB_OBJ_INFO {
	Matrix4x4 matWorld;
};

struct IA_TEXTURE_OBJ {
	Vector3 vertexPos;
	Vector2 texCoord;
};