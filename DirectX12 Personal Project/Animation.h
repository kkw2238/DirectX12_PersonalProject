#pragma once
#include "Mesh.h"

class Animation
{
public:
	Animation();
	Animation(const aiScene* scene);
	Animation(const std::wstring& path, const std::wstring& animationName, const std::wstring& extension, Assimp::Importer& assImporter);
	~Animation();

public:
	void SetAnimation(aiAnimation ani, aiScene scene, float aniTime, float aniSpeed, bool roop);

	Matrix4x4 InterpolationNodeanim(aiNodeAnim* nodeAnim, float aniTime);
	Matrix4x4 InterpolationTransformPos(aiNodeAnim* nodeAnim, float aniTime);
	Matrix4x4 InterpolationRotationQuaternion(aiNodeAnim* nodeAnim, float aniTime);
	Matrix4x4 InterpolationScaleSize(aiNodeAnim* nodeAnim, float aniTime);

	void UpdateRealTime(float animationTime, Bones* bones, aiNode* node, Matrix4x4& parentsMat, std::vector<Matrix4x4>& matDatas);
	aiNodeAnim* FindNodeAnimation(aiAnimation* animation, std::string nodeName);

	void UpdateTime(float& nowFlameTime, float elapsedTime);

protected:
	aiAnimation* m_Animation;
	const aiScene* m_Scene;

	float m_NowTime = 0.0f;
	float m_DurationTime = 0.0f;
	float m_TickPerSecond = 0.0f;
	float m_AniSpeed = 1.0f;

	bool m_Roop = true;
};

class AnimationController
{
public:
	void LoadAnimation(const std::wstring& path, const std::wstring& animationName, const std::wstring& extension);
	void Update(float& nowFlameTime, float elapsedTime);
	void SetAnimation(std::wstring& aniName);
	void SetAnimation(int aniIndex);

	void OutputMatrixDatas(Bones* bones);
	std::vector<Matrix4x4> GetAnimMatrix(float& nowFlameTime, Bones* bones);
public:
	static AnimationController* Instance();

protected:
	std::map<std::wstring, std::shared_ptr<Animation>> m_LoadedAnimations;
	std::vector<std::wstring> m_LoadedAnimaionNames;
	std::wstring m_NowAni;

	Assimp::Importer m_Importer;
};

#define ANIMATION_CONTROLLER AnimationController::Instance()