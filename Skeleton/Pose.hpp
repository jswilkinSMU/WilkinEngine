#pragma once
#include "Engine/Math/Vec3.h"
#include "Engine/Math/Quat.hpp"
#include <vector>
// -----------------------------------------------------------------------------
struct BoneTransform
{
	BoneTransform() = default;
	BoneTransform(Vec3 const& bonePosition);
	BoneTransform(Vec3 const& bonePosition, Quat const& boneRotation);
	BoneTransform(Vec3 const& bonePosition, Quat const& boneRotation, Vec3 const& boneScale);

	Vec3 m_bonePosition = Vec3::ZERO;
	Quat m_boneRotation = Quat::DEFAULT;
	Vec3 m_boneScale    = Vec3::ZERO;

	Mat44 TransformToMatrix() const;
};
// -----------------------------------------------------------------------------
class Pose
{
public:
	BoneTransform const& GetBoneTransform(int boneIndex) const;
	void SetBoneTransform(int boneIndex, BoneTransform const& boneTransform);
	
	static Pose BlendPoses(Pose const& poseA, Pose const& poseB, float blendFactor);

	int  GetNumBones() const;
	void ResizeBones(int numBones);
	void ClearBones();

public:
	std::vector<BoneTransform> m_localBoneTransforms;
};
// -----------------------------------------------------------------------------