#include "Engine/Skeleton/Pose.hpp"
#include "Engine/Math/MathUtils.h"
// -----------------------------------------------------------------------------
BoneTransform::BoneTransform(Vec3 const& bonePosition)
	:m_bonePosition(bonePosition)
{
	m_boneRotation = Quat::DEFAULT;
	m_boneScale    = Vec3::ONE;
}

BoneTransform::BoneTransform(Vec3 const& bonePosition, Quat const& boneRotation)
	:m_bonePosition(bonePosition),
	 m_boneRotation(boneRotation)
{
	m_boneScale = Vec3::ONE;
}

BoneTransform::BoneTransform(Vec3 const& bonePosition, Quat const& boneRotation, Vec3 const& boneScale)
	:m_bonePosition(bonePosition),
	 m_boneRotation(boneRotation),
	 m_boneScale(boneScale)
{
}

Mat44 BoneTransform::TransformToMatrix() const
{
	return Mat44::MakeTransform(m_bonePosition, m_boneRotation, m_boneScale);
}
// -----------------------------------------------------------------------------
BoneTransform const& Pose::GetBoneTransform(int boneIndex) const
{
	return m_localBoneTransforms[boneIndex];
}

void Pose::SetBoneTransform(int boneIndex, BoneTransform const& boneTransform)
{
	m_localBoneTransforms[boneIndex] = boneTransform;
}

Pose Pose::BlendPoses(Pose const& poseA, Pose const& poseB, float blendFactor)
{
	Pose resultingPose;
	int  numBones = static_cast<int>(poseA.m_localBoneTransforms.size());
	resultingPose.ResizeBones(numBones);

	for (int boneIndex = 0; boneIndex < numBones; ++boneIndex)
	{
		BoneTransform const& poseATransform = poseA.m_localBoneTransforms[boneIndex];
		BoneTransform const& poseBTransform = poseB.m_localBoneTransforms[boneIndex];

		Vec3 blendedPosition = Interpolate(poseATransform.m_bonePosition, poseBTransform.m_bonePosition, blendFactor);
		Quat blendedRotation = SLerp(poseATransform.m_boneRotation, poseBTransform.m_boneRotation, blendFactor);
		Vec3 blendedScale	 = Interpolate(poseATransform.m_bonePosition, poseBTransform.m_bonePosition, blendFactor);

		resultingPose.SetBoneTransform(boneIndex, BoneTransform(blendedPosition, blendedRotation, blendedScale));
	}

	return resultingPose;
}

int Pose::GetNumBones() const
{
	return static_cast<int>(m_localBoneTransforms.size());
}

void Pose::ResizeBones(int numBones)
{
	m_localBoneTransforms.resize(static_cast<int>(numBones));
}

void Pose::ClearBones()
{
	m_localBoneTransforms.clear();
}
// -----------------------------------------------------------------------------