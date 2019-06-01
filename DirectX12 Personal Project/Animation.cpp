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

Matrix4x4 Animation::InterpolationNodeanim(aiNodeAnim* nodeAnim, float aniTime)
{
	return InterpolationTransformPos(nodeAnim, aniTime) * InterpolationRotationQuaternion(nodeAnim, aniTime) * InterpolationScaleSize(nodeAnim, aniTime);
}

Matrix4x4 Animation::InterpolationTransformPos(aiNodeAnim* nodeAnim, float aniTime)
{
	unsigned int positionKeys = nodeAnim->mNumPositionKeys;
	if (positionKeys == 1) {
		Vector3 pos = Vector3(nodeAnim->mPositionKeys[0].mValue);
		return Matrix4x4::GetTransformMatrix(pos);
	}

	aiVectorKey* nextVecKey = std::find_if(&nodeAnim->mPositionKeys[0], &nodeAnim->mPositionKeys[0] + (positionKeys - 1), [&aniTime](aiVectorKey& vecKey) {
		return vecKey.mTime > static_cast<double>(aniTime);
		}
	);

	aiVectorKey* nowVecKey = (nextVecKey == &nodeAnim->mPositionKeys[0]) ? nextVecKey : nextVecKey - 1;

	double delTime = nextVecKey->mTime - nowVecKey->mTime;

	Vector3 offset = Vector3::Lerp(nowVecKey->mValue, nextVecKey->mValue, (nextVecKey->mTime - static_cast<double>(aniTime)) / delTime);
	
	return Matrix4x4::GetTransformMatrix(offset);
}

Matrix4x4 Animation::InterpolationRotationQuaternion(aiNodeAnim* nodeAnim, float aniTime)
{
	unsigned int rotationKeys = nodeAnim->mNumRotationKeys;
	if (rotationKeys == 1) {
		Vector4 quaternion = Vector4(nodeAnim->mRotationKeys[0].mValue.x, nodeAnim->mRotationKeys[0].mValue.y, nodeAnim->mRotationKeys[0].mValue.z, nodeAnim->mRotationKeys[0].mValue.w);
		return Matrix4x4::GetRotationMatrix(quaternion);
	}

	aiQuatKey* nextQuatKey = std::find_if(&nodeAnim->mRotationKeys[0], &nodeAnim->mRotationKeys[0] + (rotationKeys - 1), [&aniTime](aiQuatKey & quatKey) {
		return quatKey.mTime > static_cast<double>(aniTime);
		}
	);

	aiQuatKey* nowQuatKey = (nextQuatKey == &nodeAnim->mRotationKeys[0]) ? nextQuatKey : nextQuatKey - 1;

	double delTime = nextQuatKey->mTime - nowQuatKey->mTime;

	Vector4 quaternion = Vector4::Slerp(nowQuatKey->mValue, nextQuatKey->mValue, (nextQuatKey->mTime - static_cast<double>(aniTime)) / delTime, true);

	return Matrix4x4::GetRotationMatrix(quaternion);
}

Matrix4x4 Animation::InterpolationScaleSize(aiNodeAnim* nodeAnim, float aniTime)
{
	unsigned int scaleKeys = nodeAnim->mNumScalingKeys;
	if (scaleKeys == 1) {
		Vector3 scale = Vector3(nodeAnim->mScalingKeys[0].mValue);
		return Matrix4x4::GetScaleMatrix(scale);
	}

	aiVectorKey* nextVecKey = std::find_if(&nodeAnim->mScalingKeys[0], &nodeAnim->mScalingKeys[0] + (scaleKeys - 1), [&aniTime](aiVectorKey & vecKey) {
		return vecKey.mTime > static_cast<double>(aniTime);
		}
	);

	aiVectorKey * nowVecKey = (nextVecKey == &nodeAnim->mScalingKeys[0]) ? nextVecKey : nextVecKey - 1;

	double delTime = nextVecKey->mTime - nowVecKey->mTime;

	Vector3 sizeOffset = Vector3::Lerp(nowVecKey->mValue, nextVecKey->mValue, (nextVecKey->mTime - static_cast<double>(aniTime)) / delTime);

	return Matrix4x4::GetScaleMatrix(sizeOffset);
}

void Animation::UpdateRealTime(float animationTime, Bones* bones, aiNode* node, Matrix4x4& parentsMat, std::vector<Matrix4x4>& matDatas)
{
	aiNode* nowNode = node;
	if (nowNode == nullptr)
		nowNode = m_Scene.mRootNode;
		
	std::string nodeName = nowNode->mName.data;
	Matrix4x4 nowMat;

	aiNodeAnim* nodeAnim = FindNodeAnimation(&m_Animation, nodeName);

	Matrix4x4 nodeTransform;

	if (nodeAnim != nullptr)
		nodeTransform = InterpolationNodeanim(nodeAnim, animationTime);

	nowMat = parentsMat * nodeTransform;

	int index = bones->findBoneNumber(nodeName);

	if (index != -1) {
		matDatas[index] = bones->InvRootMatrix() * nowMat * bones->OffsetMat(index);
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
		UpdateRealTime(animationTime, bones, nowNode->mChildren[i], nowMat, matDatas);
}


aiNodeAnim* Animation::FindNodeAnimation(aiAnimation* animation, std::string nodeName)
{
	aiNodeAnim** ppAniChannels = animation->mChannels;
	int numAniChannels = animation->mNumChannels;

	aiNodeAnim* result = std::find_if(ppAniChannels[0], ppAniChannels[numAniChannels + 1], [&nodeName](aiNodeAnim& ani) {
		return (nodeName.compare(ani.mNodeName.data) == 0);
		});

	if (result == ppAniChannels[numAniChannels + 1])
		return nullptr;

	return result;
}

void AnimationController::LoadAnimation(const std::wstring& path, const std::wstring& animationName, const std::wstring& extension)
{

	std::string filenameString(path.begin(), path.end());
	std::string extensionString = ".fbx";

	const aiScene* scene = m_Importer.ReadFile(filenameString.c_str(),
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);

	aiScene aiSc = *scene;

	m_Importer.GetImporterIndex(extensionString.c_str());

	m_LoadedAnimations[animationName] = std::make_shared<Animation>(aiSc);
}

void AnimationController::Update(float& nowFlameTime, float elapsedTime)
{
	nowFlameTime += (elapsedTime * m_AniSpeed);
	if (m_Roop)
		nowFlameTime = (nowFlameTime >= m_AniTime) ? 0.0f : m_AniTime;
}

void AnimationController::SetAnimation(std::wstring& aniName)
{
	m_NowAni = aniName;
}

std::vector<Matrix4x4> AnimationController::GetAnimMatrix(float& nowFlameTime, Bones* bones)
{
	if (m_LoadedAnimations.size() < 1) return std::vector<Matrix4x4>();

	Matrix4x4 mat4x4;
	std::vector<Matrix4x4> aniMat;
	aniMat.resize(bones->BoneCount());

	m_LoadedAnimations[m_NowAni]->UpdateRealTime(nowFlameTime, bones, nullptr, mat4x4, aniMat);

	return aniMat;
}

AnimationController* AnimationController::Instance()
{
	static AnimationController animManager;
	return &animManager;
}

