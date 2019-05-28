#pragma once
#include "Mesh.h"

class Animation
{
public:
	Animation();
	Animation(aiScene scene);
	~Animation();

public:
	void SetAnimation(aiAnimation ani, aiScene scene, float aniTime, float aniSpeed, bool roop);
	std::vector<Matrix4x4> UpdateRealTime(float animationTime, Bones* bones, aiNode* node);

	aiNodeAnim* FindNodeAnimation(aiAnimation* animation, std::string nodeName);

protected:
	aiAnimation m_Animation;
	aiScene m_Scene;

	float m_NowTime = 0.0f;

	bool m_Roop = true;
};

class AnimationController
{
public:
	void LoadAnimation(const std::wstring& path, const std::wstring& animationName, const std::wstring& extension);
	void Update(float elapsedTime);
	void SetAnimation(std::wstring& aniName);

	std::vector<Matrix4x4> GetBoneMatrix(Bones* bones);
public:
	static AnimationController* Instance();

protected:
	std::map<std::wstring, std::shared_ptr<Animation>> m_LoadedAnimations;
	std::wstring m_NowAni;

	bool  m_Roop = true;

	float m_AniTime = 3.0f;
	float m_AniSpeed = 1.0f;
	float m_NowTime = 0.0f;
};