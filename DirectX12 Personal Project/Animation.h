#pragma once
#include "Mesh.h"


Matrix4x4 aiMatrixConverter(aiMatrix4x4& mat) {
	Matrix4x4 result;
	::memcpy(&result, &mat, sizeof(float) * 16);

	return result;
}

class Animation
{
public:
	Animation();
	~Animation();

public:
	void LoadAnimation(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& path, const std::wstring& animationName, const std::wstring& extension);
	void ProcessAnimationNode(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, aiNode* node, const aiScene* scene);

protected:
	std::wstring m_AnimationName;
	Matrix4x4 m_BoneMatrixes[NUM_OF_BONES];
	Matrix4x4 m_InvRootMatrix;
};
