#include "Engine/Animation/Animation.hpp"
#include "Engine/Skeleton/Skeleton.hpp"
#include "Engine/Math/MathUtils.h"

Animation::Animation(std::string const& animName, float duration, bool isAnimLooping, std::function<void(Skeleton&, float)> animCallback)
	:m_animName(animName),
	 m_animDuration(duration),
	 m_animIsLooping(isAnimLooping),
	 m_animCallback(animCallback)
{
}

void Animation::ApplyAnimation(Skeleton& skeleton, float time) const
{
	if (m_animCallback)
	{
		m_animCallback(skeleton, time);
	}
}

Pose Animation::GetPoseAtTime(Skeleton const& skeleton, float time) const
{
	Skeleton tempSkeleton = skeleton;
	tempSkeleton.ResetSkeletonPose();

	m_animCallback(tempSkeleton, time);

	Pose pose;
	pose.ResizeBones(static_cast<int>(tempSkeleton.m_bones.size()));

	for (int boneIndex = 0; boneIndex < static_cast<int>(tempSkeleton.m_bones.size()); ++boneIndex)
	{
		Bone const& bone = tempSkeleton.m_bones[boneIndex];
		BoneTransform boneTransform = BoneTransform(bone.m_localPosition, bone.m_localRotation, bone.m_localScale);
		pose.SetBoneTransform(boneIndex, boneTransform);
	}

	return pose;
}
