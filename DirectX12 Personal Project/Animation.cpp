#include "Animation.h"



Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::LoadAnimation(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, const std::wstring& path, const std::wstring& animationName, const std::wstring& extension)
{
	Assimp::Importer importer;

	std::string filenameString(path.begin(), path.end());
	std::string extensionString(extension.begin(), extension.end());

	const aiScene* scene = importer.ReadFile(filenameString.c_str(),
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);

	importer.GetImporterIndex(extensionString.c_str());

	ProcessAnimationNode(id3dDevice, id3dGraphicsCommandList, scene->mRootNode, scene);

	m_AnimationName = animationName;
}

void Animation::ProcessAnimationNode(ID3D12Device* id3dDevice, ID3D12GraphicsCommandList* id3dGraphicsCommandList, aiNode* node, const aiScene* scene)
{
	aiNode* rootNode = scene->mRootNode;

	m_InvRootMatrix = aiMatrixConverter(rootNode->mTransformation.Inverse());
	rootNode->FindNode(rootNode->mName.data);

	UINT numAnimation =	scene->mNumAnimations;

	for (UINT i = 0; i < numAnimation; ++i) {
		aiAnimation* nowAnimation = scene->mAnimations[i];

		UINT numChannel = nowAnimation->mNumChannels;

		for (UINT j = 0; j < numChannel; ++j) {
			aiNodeAnim* aniChannel = nowAnimation->mChannels[j];
			
		}
		
	}
}
