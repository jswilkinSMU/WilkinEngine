#pragma once
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/Quat.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include <string>
#include <vector>
// -----------------------------------------------------------------------------
enum class CONSTRAINT_TYPE
{
	INVALID = -1,
	FREE,
	LIMITED,
	LOCKED,
	NUM_CONSTRAINT_TYPES
};
// -----------------------------------------------------------------------------
struct BoneConstraint
{
	CONSTRAINT_TYPE m_rotationConstraints[3] = { CONSTRAINT_TYPE::FREE, CONSTRAINT_TYPE::FREE, CONSTRAINT_TYPE::FREE };
	CONSTRAINT_TYPE m_translationConstraints[3] = { CONSTRAINT_TYPE::FREE, CONSTRAINT_TYPE::FREE, CONSTRAINT_TYPE::FREE };

	EulerAngles m_minRotationDegrees = EulerAngles(-180.f, -180.f, -180.f);
	EulerAngles m_maxRotationDegrees = EulerAngles(180.f, 180.f, 180.f);

	Vec3 m_minTranslation = Vec3::ZERO;
	Vec3 m_maxTranslation = Vec3::ZERO;

	Quat ApplyRotationConstraint(Quat const& constrainedRotation);
	Vec3 ApplyTranslationConstraint(Vec3 const& constrainedTranslation);

	bool IsAxisLocked(int axisIndex, bool isRotation) const;
	bool HasRotationLimitOnAxis(int axisIndex) const;
};
// -----------------------------------------------------------------------------
class Bone
{
public:
	void UpdateWorldBoneTransform(std::vector<Bone> const& bones);

	// Bone mutators
	void SetLocalBoneTransform(Vec3 const& position, Quat const& rotation, Vec3 const& scale);
	void SetLocalBonePosition(Vec3 const& position);
	void SetLocalBoneRotation(Quat const& rotation);
	void SetLocalBoneScale(Vec3 const& scale);

	// Bone accessors
	bool  IsRootBone() const;
	Vec2  GetLocalBonePosition2D() const;
	Vec2  GetWorldBonePosition2D() const;
	Vec3  GetLocalBonePosition3D() const;
	Vec3  GetWorldBonePosition3D() const;
	Mat44 GetLocalBoneTransformMatrix() const;
	float GetDistanceToBone(Bone const& nextBone) const;

public:
	std::string m_boneName = "";
	int m_parentBoneIndex = -1;
	std::vector<unsigned int> m_childBoneIndices;
	bool m_isRenderable = true;

	// Local transform data
	Vec3 m_localPosition = Vec3::ZERO;
	Quat m_localRotation = Quat::DEFAULT;
	Vec3 m_localScale	 = Vec3::ONE;
	
	// Combined transform data
	Mat44 m_worldBoneTransform = Mat44();

	// Constraint for joint behavior and limits
	BoneConstraint m_boneConstraint;
};