#pragma once

/*
 * Structchers.h 파일에는 현재 프로젝트에서 사용 할 Cbuffer관련 struct를 모아뒀음.
 */

#include "MathUtil.h"
#include <Map>

const unsigned int MAX_LIGHT = 2;
const unsigned int VERTEX_IN_BONE_COUNT = 4;
const unsigned int NUM_OF_BONES = 96;

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
};

struct VERTEX_IN_BONE_DATA {
	UINT boneIndices[VERTEX_IN_BONE_COUNT] = { 0, 0, 0, 0 };
	float weights[VERTEX_IN_BONE_COUNT] = { 0.0f, 0.0f, 0.0f, 0.0f };
};

struct VERTEX_IN_BONE {
	VERTEX_IN_BONE_DATA data;
	UINT linkedBoneCount = 0;
	float totalWeight = 0.0f;

	void InputVertexInBoneData(UINT boneIndex, float weight) {
		if (linkedBoneCount > VERTEX_IN_BONE_COUNT - 1) return;
		data.boneIndices[linkedBoneCount] = boneIndex;
		data.weights[linkedBoneCount] = weight;

		totalWeight += weight;

		++linkedBoneCount;
	}

	VERTEX_IN_BONE_DATA GetData() const { return data; }
};

struct IA_TEXTURE_OBJ {
	IA_TEXTURE_OBJ() {};
	IA_TEXTURE_OBJ(Vector3& vertex, Vector2& texCoord, Vector3& normal, Vector3& tangent, const UINT matindex, const VERTEX_IN_BONE_DATA vboData) :
		vertexPos(vertex), texCoord(texCoord), normal(normal), tangent(tangent), materialIndex(matindex), vertexBoneData(vboData) {}
	IA_TEXTURE_OBJ(Vector3& vertex, Vector2& texCoord, Vector3& normal, Vector3& tangent, const UINT matindex) :
		vertexPos(vertex), texCoord(texCoord), normal(normal), tangent(tangent), materialIndex(matindex) {}

	Vector3 vertexPos;
	Vector2 texCoord;
	Vector3 normal;
	Vector3 tangent;
	UINT	materialIndex;
	VERTEX_IN_BONE_DATA vertexBoneData;
};

std::ostream& operator<<(std::ostream& os, const IA_TEXTURE_OBJ& obj);

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