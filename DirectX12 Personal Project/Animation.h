#pragma once
#include "Mesh.h"

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
	
};
