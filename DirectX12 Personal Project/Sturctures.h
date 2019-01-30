#pragma once

/*
 * Structchers.h 파일에는 현재 프로젝트에서 사용 할 Cbuffer관련 struct를 모아뒀음.
 */

#include "MathUtil.h"

struct CB_CAMERA_INFO {
	Matrix4x4 matProjection;
	Matrix4x4 matView;
};

struct CB_OBJ_INFO {
	Matrix4x4 matWorld;
};