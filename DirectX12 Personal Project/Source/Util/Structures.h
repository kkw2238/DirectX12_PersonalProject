#pragma once

/*
 * Structchers.h ÆÄÀÏ¿¡´Â ÇöÀç ÇÁ·ÎÁ§Æ®¿¡¼­ »ç¿ë ÇÒ Cbuffer°ü·Ã struct¸¦ ¸ğ¾Æµ×À½.
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
	Matrix4x4 matBonesMatrix[NUM_OF_BONES];
	Matrix4x4 matBoneInvMatrix;
};

<<<<<<< HEAD
struct VERTEX_IN_BONE_DATA {
	UINT boneIndices[VERTEX_IN_BONE_COUNT] = { 0, 0, 0, 0 };
	float weights[VERTEX_IN_BONE_COUNT] = { 0.0f, 0.0f, 0.0f, 0.0f };
};

struct VERTEX_IN_BONE {
	VERTEX_IN_BONE_DATA data;
	UINT linkedBoneCount = 0;

	void InputVertexInBoneData(UINT boneIndex, float weight) {
		data.boneIndices[linkedBoneCount] = boneIndex;
		data.weights[linkedBoneCount] = weight;

		++linkedBoneCount;
	}

	VERTEX_IN_BONE_DATA GetData() const { return data; }
};

=======
>>>>>>> parent of ee99c41... ì• ë‹ˆë©”ì´ì…˜ ì‘ì—…ì¤‘
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