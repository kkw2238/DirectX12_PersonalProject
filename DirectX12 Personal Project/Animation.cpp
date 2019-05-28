#include "Animation.h"



Animation::Animation()
{
}

Animation::Animation(aiScene scene) :
	m_Animation(*scene.mAnimations[0]), m_Scene(scene)
{
}

Animation::~Animation()
{
}

void Animation::SetAnimation(aiAnimation ani, aiScene scene, float aniTime, float aniSpeed, bool roop)
{

}

std::vector<Matrix4x4> Animation::UpdateRealTime(float animationTime, Bones* bones, aiNode* node)
{

	return std::vector<Matrix4x4>();
}


aiNodeAnim* Animation::FindNodeAnimation(aiAnimation* animation, std::string nodeName)
{
	aiNodeAnim** ppAniChannels = animation->mChannels;
	int numAniChannels = animation->mNumChannels;

	aiNodeAnim* result = std::find(ppAniChannels[0], ppAniChannels[numAniChannels + 1], [nodeName](aiNodeAnim & ani) {
		return nodeName.compare(ani.mNodeName.data) == 0;
		});

	if (result == ppAniChannels[numAniChannels + 1])
		return nullptr;

	return result;
}

void AnimationController::LoadAnimation(const std::wstring& path, const std::wstring& animationName, const std::wstring& extension)
{
	Assimp::Importer importer;

	std::string filenameString(path.begin(), path.end());
	std::string extensionString(extension.begin(), extension.end());

	const aiScene* scene = importer.ReadFile(filenameString.c_str(),
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);

	importer.GetImporterIndex(extensionString.c_str());

	m_LoadedAnimations[animationName] = std::make_shared<Animation>(*scene);
}

void AnimationController::Update(float elapsedTime)
{
	m_NowTime += (elapsedTime * m_AniSpeed);
	if (m_Roop)
		m_NowTime = (m_NowTime >= m_AniTime) ? 0.0f : m_AniTime;
}

void AnimationController::SetAnimation(std::wstring& aniName)
{
	m_NowAni = aniName;
	m_NowTime = 0.0f;
}

std::vector<Matrix4x4> AnimationController::GetBoneMatrix(Bones* bones)
{
	return m_LoadedAnimations[m_NowAni]->UpdateRealTime(m_NowTime, bones, nullptr);
}


