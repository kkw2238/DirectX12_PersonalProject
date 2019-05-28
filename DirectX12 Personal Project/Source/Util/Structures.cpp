#include "Structures.h"

std::ostream& operator<<(std::ostream& os, const IA_TEXTURE_OBJ& obj)
{
	os << "x=" << obj.vertexPos.x << " y=" << obj.vertexPos.y << " z=" << obj.vertexPos.z
		<< " tu=" << obj.texCoord.x << " tv=" << obj.texCoord.y
		<< " nx=" << obj.normal.x << " ny=" << obj.normal.y << " nz=" << obj.normal.z
		<< " bx=" << obj.vertexBoneData.boneIndices[0] << " by=" << obj.vertexBoneData.boneIndices[1] << " bz=" << obj.vertexBoneData.boneIndices[2]<< " ba="  << obj.vertexBoneData.boneIndices[3]
		<< " wx=" << obj.vertexBoneData.weights[0] << " wy=" << obj.vertexBoneData.weights[1] << " wz=" << obj.vertexBoneData.weights[2] << " wa=" << obj.vertexBoneData.weights[3] << " \n";
		return os;
}
