#include "Engine/Skeleton/Bone.hpp"
#include "Engine/Math/MathUtils.h"
// -----------------------------------------------------------------------------
void Bone::UpdateWorldBoneTransform(std::vector<Bone> const& bones)
{
	Mat44 localTransform = Mat44::MakeTransform(m_localPosition, m_localRotation, m_localScale);

	if (m_parentBoneIndex >= 0)
	{
		m_worldBoneTransform = bones[m_parentBoneIndex].m_worldBoneTransform * localTransform;
	}
	else
	{
		m_worldBoneTransform = localTransform;
	}
}

void Bone::SetLocalBoneTransform(Vec3 const& position, Quat const& rotation, Vec3 const& scale)
{
	m_localPosition = position;
	m_localRotation = rotation;
	m_localScale	= scale;
}

void Bone::SetLocalBonePosition(Vec3 const& position)
{
	m_localPosition = position;
}

void Bone::SetLocalBoneRotation(Quat const& rotation)
{
	m_localRotation = rotation;
}

void Bone::SetLocalBoneScale(Vec3 const& scale)
{
	m_localScale = scale;
}

bool Bone::IsRootBone() const
{
	return m_parentBoneIndex == -1;
}

Vec2 Bone::GetLocalBonePosition2D() const
{
	return m_localPosition.GetXY();
}

Vec2 Bone::GetWorldBonePosition2D() const
{
	return m_worldBoneTransform.GetTranslation2D();
}

Vec3 Bone::GetLocalBonePosition3D() const
{
	return m_localPosition;
}

Vec3 Bone::GetWorldBonePosition3D() const
{
	return m_worldBoneTransform.GetTranslation3D();
}

Mat44 Bone::GetLocalBoneTransformMatrix() const
{
	return Mat44::MakeTransform(m_localPosition, m_localRotation, m_localScale);
}

float Bone::GetDistanceToBone(Bone const& nextBone) const
{
	Vec3 boneDistance = GetWorldBonePosition3D() - nextBone.GetWorldBonePosition3D();
	float boneLength = boneDistance.GetLength();
	return boneLength;
}
// -----------------------------------------------------------------------------
Quat BoneConstraint::ApplyRotationConstraint(Quat const& constrainedRotation)
{
	EulerAngles eulerAngles = constrainedRotation.GetAsEulerAngles();

	// Yaw
	if (m_rotationConstraints[0] == CONSTRAINT_TYPE::LOCKED)
	{
		eulerAngles.m_yawDegrees = 0.f;
	}
	else if (m_rotationConstraints[0] == CONSTRAINT_TYPE::LIMITED)
	{
		eulerAngles.m_yawDegrees = GetClamped(eulerAngles.m_yawDegrees, m_minRotationDegrees.m_yawDegrees, m_maxRotationDegrees.m_yawDegrees);
	}

	// Pitch
	if (m_rotationConstraints[1] == CONSTRAINT_TYPE::LOCKED)
	{
		eulerAngles.m_pitchDegrees = 0.f;
	}
	else if (m_rotationConstraints[1] == CONSTRAINT_TYPE::LIMITED)
	{
		eulerAngles.m_pitchDegrees = GetClamped(eulerAngles.m_pitchDegrees, m_minRotationDegrees.m_pitchDegrees, m_maxRotationDegrees.m_pitchDegrees);
	}

	// Roll
	if (m_rotationConstraints[2] == CONSTRAINT_TYPE::LOCKED)
	{
		eulerAngles.m_rollDegrees = 0.f;
	}
	else if (m_rotationConstraints[2] == CONSTRAINT_TYPE::LIMITED)
	{
		eulerAngles.m_rollDegrees = GetClamped(eulerAngles.m_rollDegrees, m_minRotationDegrees.m_rollDegrees, m_maxRotationDegrees.m_rollDegrees);
	}

	return Quat::MakeFromEulerAngles(eulerAngles);
}

Vec3 BoneConstraint::ApplyTranslationConstraint(Vec3 const& constrainedTranslation)
{
	Vec3 chosenConstraint = constrainedTranslation;

	// X
	if (m_translationConstraints[0] == CONSTRAINT_TYPE::LOCKED)
	{
		chosenConstraint.x = 0.f;
	}
	else if (m_translationConstraints[0] == CONSTRAINT_TYPE::LIMITED)
	{
		chosenConstraint.x = GetClamped(chosenConstraint.x, m_minTranslation.x, m_maxTranslation.x);
	}

	// Y
	if (m_translationConstraints[1] == CONSTRAINT_TYPE::LOCKED)
	{
		chosenConstraint.y = 0.f;
	}
	else if (m_translationConstraints[1] == CONSTRAINT_TYPE::LIMITED)
	{
		chosenConstraint.y = GetClamped(chosenConstraint.y, m_minTranslation.y, m_maxTranslation.y);
	}

	// Z
	if (m_translationConstraints[2] == CONSTRAINT_TYPE::LOCKED)
	{
		chosenConstraint.z = 0.f;
	}
	else if (m_translationConstraints[2] == CONSTRAINT_TYPE::LIMITED)
	{
		chosenConstraint.z = GetClamped(chosenConstraint.z, m_minTranslation.z, m_maxTranslation.z);
	}

	return chosenConstraint;
}

bool BoneConstraint::IsAxisLocked(int axisIndex, bool isRotation) const
{
	if (isRotation)
	{
		return m_rotationConstraints[axisIndex] == CONSTRAINT_TYPE::LOCKED;
	}
	else
	{
		return m_translationConstraints[axisIndex] == CONSTRAINT_TYPE::LOCKED;
	}
}

bool BoneConstraint::HasRotationLimitOnAxis(int axisIndex) const
{
	return m_rotationConstraints[axisIndex] == CONSTRAINT_TYPE::LIMITED;
}
