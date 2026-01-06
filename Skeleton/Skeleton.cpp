#include "Engine/Skeleton/Skeleton.hpp"
#include "Engine/Core/VertexUtils.h"
#include "Engine/Math/MathUtils.h"

/* SKELETON POSE */
// -----------------------------------------------------------------------------
void Skeleton::UpdateSkeletonPose()
{
	for (int boneIndex = 0; boneIndex < static_cast<int>(m_bones.size()); ++boneIndex)
	{
		m_bones[boneIndex].UpdateWorldBoneTransform(m_bones);

		if (m_bones[boneIndex].IsRootBone())
		{
			m_bones[boneIndex].m_worldBoneTransform = m_skeletonModelTransform * m_bones[boneIndex].m_worldBoneTransform;
		}
	}
}

void Skeleton::ResetSkeletonPose()
{
	for (int boneIndex = 0; boneIndex < static_cast<int>(m_bones.size()); ++boneIndex)
	{
		Bone& bone = m_bones[boneIndex];
		bone.SetLocalBoneTransform(Vec3::ZERO, Quat::DEFAULT, Vec3::ONE);
	}

	UpdateSkeletonPose();
}
// -----------------------------------------------------------------------------

/* HELPER UTILITY METHODS */
// -----------------------------------------------------------------------------
int Skeleton::FindBoneIndexByName(std::string const& boneName) const
{
	for (int boneIndex = 0; boneIndex < static_cast<int>(m_bones.size()); ++boneIndex)
	{
		if (m_bones[boneIndex].m_boneName == boneName)
		{
			return boneIndex;
		}
	}
	return -1;
}

Bone* Skeleton::GetBoneByName(std::string const& boneName)
{
	int boneIndex = FindBoneIndexByName(boneName);

	if (boneIndex >= 0)
	{
		return &m_bones[boneIndex];
	}
	else
	{
		return nullptr;
	}
}

Bone* Skeleton::GetBoneByIndex(int boneIndex)
{
	if (boneIndex >= 0 && boneIndex < static_cast<int>(m_bones.size()))
	{
		return &m_bones[boneIndex];
	}
	return nullptr;
}

void Skeleton::GetBoneHierarchy(std::vector<Vertex_PCU>& textVerts, int parentIndex, float& textYPos, float indentStep, BitmapFont& font, int depth)
{
	for (int boneIndex = 0; boneIndex < static_cast<int>(m_bones.size()); ++boneIndex)
	{
		Bone const& bone = m_bones[boneIndex];

		if (bone.m_parentBoneIndex == parentIndex)
		{
			std::string indentation = std::string(depth, '\t');
			std::string boneText = indentation + bone.m_boneName;

			font.AddVertsForText2D(textVerts, Vec2(10.f, textYPos), 15.f, boneText, Rgba8::WHITE);
			textYPos -= 20.f;
			GetBoneHierarchy(textVerts, boneIndex, textYPos, indentStep, font, depth + 1);
		}
	}
}

float Skeleton::AngleBetweenBones(float a, float b, float c)
{
	// Using law of cosines to get angle between bones
	return acosf((a * a + b * b - c * c) / (2 * a * b));
}
// -----------------------------------------------------------------------------

/* INVERSE KINEMATICS */
// -----------------------------------------------------------------------------
void Skeleton::ApplyIKToBone(Bone& bone, float angle, Vec3 const& axisOfRotation)
{
	Quat deltaRotation = Quat::MakeFromAxisAngle(axisOfRotation, angle);
	Quat currentRotation = bone.m_localRotation;
	Quat newRotation = (deltaRotation * currentRotation).GetNormalized();
	bone.SetLocalBoneRotation(newRotation);
}

void Skeleton::SolveTwoBoneIK(int shoulderIndex, int elbowIndex, int handIndex, Vec3 targetPos)
{
	Bone* shoulder = GetBoneByIndex(shoulderIndex);
	Bone* elbow = GetBoneByIndex(elbowIndex);
	Bone* hand = GetBoneByIndex(handIndex);

	Vec3 bonePosA = shoulder->GetWorldBonePosition3D();
	Vec3 bonePosB = elbow->GetWorldBonePosition3D();
	Vec3 bonePosC = hand->GetWorldBonePosition3D();

	float lenAB = (bonePosB - bonePosA).GetLength();
	float lenBC = (bonePosC - bonePosB).GetLength();
	float maxReach = lenAB + lenBC;

	Vec3 target = targetPos;
	float distToTarget = (target - bonePosA).GetLength();

	// Unreachable case check, stop rotations
	if (distToTarget >= maxReach)
	{
		return;
	}

	if (distToTarget > maxReach)
	{
		target = bonePosA + (target - bonePosA).GetNormalized() * maxReach;
	}

	float adjacent = (lenAB * lenAB + distToTarget * distToTarget - lenBC * lenBC) / (2.f * lenAB);
	float height = sqrtf(GetMax(0.f, lenAB * lenAB - adjacent * adjacent));

	Vec3 shoulderToTarget = (target - bonePosA).GetNormalized();
	Vec3 arbitrary = Vec3::YAXE;
	if (fabsf(DotProduct3D(shoulderToTarget, arbitrary)) > 0.999f)
	{
		arbitrary = Vec3::ZAXE;
	}

	Vec3 perp = CrossProduct3D(shoulderToTarget, arbitrary).GetNormalized();
	Vec3 elbowNewPos = bonePosA + shoulderToTarget * adjacent + perp * height;

	// Shoulder rotation
	Vec3 AB_current = (bonePosB - bonePosA).GetNormalized();
	Vec3 AB_goal = (elbowNewPos - bonePosA).GetNormalized();
	Quat shoulderRot = Quat::MakeRotationFromTwoVectors(AB_current, AB_goal);
	Quat newShoulderRotation = shoulderRot * shoulder->m_localRotation;
	shoulder->SetLocalBoneRotation(newShoulderRotation);

	UpdateSkeletonPose();

	bonePosB = elbow->GetWorldBonePosition3D();
	bonePosC = hand->GetWorldBonePosition3D();

	// Elbow rotation
	Vec3 BC_current = (bonePosC - bonePosB).GetNormalized();
	Vec3 BC_goal = (target - bonePosB).GetNormalized();
	Quat elbowRot = Quat::MakeRotationFromTwoVectors(BC_current, BC_goal);
	Quat newElbowRotation = elbowRot * elbow->m_localRotation;
	elbow->SetLocalBoneRotation(newElbowRotation);

	UpdateSkeletonPose();
}

void Skeleton::SolveCCDIK(std::vector<int> const& chainIndices, Vec3 const& targetPosition, int maxIterations, float threshold)
{
	// Check if there are enough bones for a chain
	if (chainIndices.size() < 2)
	{
		return;
	}

	float totalChainLength = 0.f;
	
	for (int chainIndex = 0; chainIndex < static_cast<int>(chainIndices.size() - 1); ++chainIndex)
	{
		int chainIndexA = chainIndices[chainIndex];
		int chainIndexB = chainIndices[chainIndex + 1];
		Vec3 chainIndexAPosition = m_bones[chainIndexA].GetWorldBonePosition3D();
		Vec3 chainIndexBPosition = m_bones[chainIndexB].GetWorldBonePosition3D();
		totalChainLength += (chainIndexBPosition - chainIndexAPosition).GetLength();
	}

	// Clamp if target is unreachable
	Vec3  rootPosition = m_bones[chainIndices[0]].GetWorldBonePosition3D();
	float distToTarget = (targetPosition - rootPosition).GetLength();

	Vec3 clampedTargetPos = targetPosition;
	bool targetUnreachable = false;
	if (distToTarget > totalChainLength)
	{
		targetUnreachable = true;
		Vec3 direction = (targetPosition - rootPosition).GetNormalized();
		clampedTargetPos = rootPosition + direction * totalChainLength;
	}

	distToTarget = (clampedTargetPos - rootPosition).GetLength();

	if (targetUnreachable)
	{
		Vec3 direction = (targetPosition - rootPosition).GetNormalized();

		// Straightening the chain in the target direction
		for (int chainIndex = 0; chainIndex < static_cast<int>(chainIndices.size()) - 1; ++chainIndex)
		{
			int jointIndex = chainIndices[chainIndex];
			int nextIndex = chainIndices[chainIndex + 1];

			Vec3 jointPos = m_bones[jointIndex].GetWorldBonePosition3D();
			Vec3 nextPos = m_bones[nextIndex].GetWorldBonePosition3D();
			Vec3 toNext = (nextPos - jointPos).GetNormalized();

			float dot = DotProduct3D(toNext, direction);
			dot = GetClamped(dot, -1.f, 1.f);
			float angle = acosf(dot);

			if (angle > 0.0001f)
			{
				Vec3 axis = CrossProduct3D(toNext, direction);
				if (axis.GetLengthSquared() > 0.00001f)
				{
					axis.Normalize();
					Quat rotation = Quat::MakeFromAxisAngle(axis, angle);
					m_bones[jointIndex].SetLocalBoneRotation(rotation * m_bones[jointIndex].m_localRotation);
				}
			}
		}

		UpdateSkeletonPose();
		return;
	}

	for (int iterationIndex = 0; iterationIndex < maxIterations; ++iterationIndex)
	{
		bool breakLoop = false;

		for (int chainIndex = static_cast<int>(chainIndices.size()) - 2; chainIndex >= 0; --chainIndex)
		{
			int jointIndex = chainIndices[chainIndex];
			int endEffectorIndex = chainIndices.back();

			Vec3 jointPos = m_bones[jointIndex].GetWorldBonePosition3D();
			Vec3 endEffectorPos = m_bones[endEffectorIndex].GetWorldBonePosition3D();

			Vec3 toEndEffector = endEffectorPos - jointPos;
			Vec3 toTarget = clampedTargetPos - jointPos;

			if (toEndEffector.GetLengthSquared() < 0.00001f || toTarget.GetLengthSquared() < 0.00001f)
			{
				continue;
			}

			toEndEffector.Normalize();
			toTarget.Normalize();

			// Compute rotation to align end effector to target
			float effectorTargetDot = DotProduct3D(toEndEffector, toTarget);
			effectorTargetDot = GetClamped(effectorTargetDot, -1.f, 1.f);
			float angle = acosf(effectorTargetDot);

			// Stop if already fully extended and aligned
			if (effectorTargetDot > 0.9999f && distToTarget > totalChainLength - 0.0001f)
			{
				breakLoop = true;
				break;
			}
			// Rotate to target if there is an angle greater than our threshold
			else if (angle > 0.001f)
			{
				Vec3 rotationAxis = CrossProduct3D(toEndEffector, toTarget);
				
				if (rotationAxis.GetLengthSquared() > 0.00001f)
				{
					Quat rotationQuat = Quat::MakeFromAxisAngle(rotationAxis, angle);
					Quat currentLocalRotation = m_bones[jointIndex].m_localRotation;
					m_bones[jointIndex].SetLocalBoneRotation(rotationQuat * currentLocalRotation);

					UpdateSkeletonPose();
				}
			}
		}

		// Breaking out of outer loop
		if (breakLoop)
		{
			break;
		}

		// Convergence check
		Vec3 newEffectorPos = m_bones[chainIndices.back()].GetWorldBonePosition3D();
		if ((newEffectorPos - clampedTargetPos).GetLength() <= threshold)
		{
			break;
		}
	}
}

void Skeleton::SolveCCDIKConstrained(std::vector<int> const& chainIndices, Vec3 const& targetPosition, int maxIterations, float threshold)
{
	// Check if there are enough bones for a chain
	if (chainIndices.size() < 2)
	{
		return;
	}

	float totalChainLength = 0.f;

	for (int chainIndex = 0; chainIndex < static_cast<int>(chainIndices.size() - 1); ++chainIndex)
	{
		int chainIndexA = chainIndices[chainIndex];
		int chainIndexB = chainIndices[chainIndex + 1];
		Vec3 chainIndexAPosition = m_bones[chainIndexA].GetWorldBonePosition3D();
		Vec3 chainIndexBPosition = m_bones[chainIndexB].GetWorldBonePosition3D();
		totalChainLength += (chainIndexBPosition - chainIndexAPosition).GetLength();
	}

	// Clamp if target is unreachable
	Vec3  rootPosition = m_bones[chainIndices[0]].GetWorldBonePosition3D();
	float distToTarget = (targetPosition - rootPosition).GetLength();

	Vec3 clampedTargetPos = targetPosition;
	bool targetUnreachable = false;
	if (distToTarget > totalChainLength)
	{
		targetUnreachable = true;
		Vec3 direction = (targetPosition - rootPosition).GetNormalized();
		clampedTargetPos = rootPosition + direction * totalChainLength;
	}

	distToTarget = (clampedTargetPos - rootPosition).GetLength();

	if (targetUnreachable)
	{
		Vec3 direction = (targetPosition - rootPosition).GetNormalized();

		// Straightening the chain in the target direction
		for (int chainIndex = 0; chainIndex < static_cast<int>(chainIndices.size()) - 1; ++chainIndex)
		{
			int jointIndex = chainIndices[chainIndex];
			int nextIndex = chainIndices[chainIndex + 1];

			Vec3 jointPos = m_bones[jointIndex].GetWorldBonePosition3D();
			Vec3 nextPos = m_bones[nextIndex].GetWorldBonePosition3D();
			Vec3 toNext = (nextPos - jointPos).GetNormalized();

			float dot = DotProduct3D(toNext, direction);
			dot = GetClamped(dot, -1.f, 1.f);
			float angle = acosf(dot);

			if (angle > 0.0001f)
			{
				Vec3 axis = CrossProduct3D(toNext, direction);
				if (axis.GetLengthSquared() > 0.00001f)
				{
					Quat rotation = Quat::MakeFromAxisAngle(axis, angle);
					Quat currentLocalRotation = m_bones[jointIndex].m_localRotation;
					Quat newRotation = rotation * currentLocalRotation;

					newRotation = m_bones[jointIndex].m_boneConstraint.ApplyRotationConstraint(newRotation);
					m_bones[jointIndex].SetLocalBoneRotation(newRotation);
				}
			}
		}

		UpdateSkeletonPose();
		return;
	}

	// Dead zone check
	if (distToTarget < 2.1f)
	{
		return;
	}

	for (int iterationIndex = 0; iterationIndex < maxIterations; ++iterationIndex)
	{
		bool breakLoop = false;

		for (int chainIndex = static_cast<int>(chainIndices.size()) - 2; chainIndex >= 0; --chainIndex)
		{
			int jointIndex = chainIndices[chainIndex];
			int endEffectorIndex = chainIndices.back();

			Vec3 jointPos = m_bones[jointIndex].GetWorldBonePosition3D();
			Vec3 endEffectorPos = m_bones[endEffectorIndex].GetWorldBonePosition3D();

			Vec3 toEndEffector = endEffectorPos - jointPos;
			Vec3 toTarget = clampedTargetPos - jointPos;

			if (toEndEffector.GetLengthSquared() < 0.00001f || toTarget.GetLengthSquared() < 0.00001f)
			{
				continue;
			}

			toEndEffector.Normalize();
			toTarget.Normalize();

			// Compute rotation to align end effector to target
			float effectorTargetDot = DotProduct3D(toEndEffector, toTarget);
			effectorTargetDot = GetClamped(effectorTargetDot, -1.f, 1.f);
			float angle = acosf(effectorTargetDot);

			// Stop if already fully extended and aligned
			if (effectorTargetDot > 0.9999f && distToTarget > totalChainLength - 0.0001f)
			{
				breakLoop = true;
				break;
			}
			// Rotate to target if there is an angle greater than our threshold
			else if (angle > 0.001f)
			{
				Vec3 rotationAxis = CrossProduct3D(toEndEffector, toTarget);

				if (rotationAxis.GetLengthSquared() > 0.00001f)
				{
					Quat rotationQuat = Quat::MakeFromAxisAngle(rotationAxis, angle);
					Quat currentLocalRotation = m_bones[jointIndex].m_localRotation;
					Quat newRotation = rotationQuat * currentLocalRotation;

					newRotation = m_bones[jointIndex].m_boneConstraint.ApplyRotationConstraint(newRotation);
					m_bones[jointIndex].SetLocalBoneRotation(newRotation);

					UpdateSkeletonPose();
				}
			}
		}

		// Breaking out of outer loop
		if (breakLoop)
		{
			break;
		}

		// Convergence check
		Vec3 newEffectorPos = m_bones[chainIndices.back()].GetWorldBonePosition3D();
		if ((newEffectorPos - clampedTargetPos).GetLength() <= threshold)
		{
			break;
		}
	}
}

void Skeleton::SolveFABRIK(std::vector<int> const& chainIndices, Vec3 const& targetPosition, int maxIterations, float threshold)
{
	// Check if there are enough bones for a chain
	if (chainIndices.size() < 2)
	{
		return;
	}

	std::vector<Vec3> bonePositions;
	std::vector<float> boneLengths;
	float totalLength = 0.f;

	// Caching current positions and lengths
	for (int boneIndex = 0; boneIndex < static_cast<int>(chainIndices.size()); ++boneIndex)
	{
		bonePositions.push_back(m_bones[chainIndices[boneIndex]].GetWorldBonePosition3D());
		if (boneIndex > 0)
		{
			float boneLength = (bonePositions[boneIndex] - bonePositions[boneIndex - 1]).GetLength();
			boneLengths.push_back(boneLength);
			totalLength += boneLength;
		}
	}

	Vec3 rootPos = bonePositions[0];

	if ((targetPosition - rootPos).GetLength() > totalLength)
	{
		// Target is unreachable, let's stretch towards it
		for (int boneIndex = 1; boneIndex < static_cast<int>(bonePositions.size()); ++boneIndex)
		{
			Vec3 targetDirection = (targetPosition - bonePositions[boneIndex - 1]).GetNormalized();
			bonePositions[boneIndex] = bonePositions[boneIndex - 1] + targetDirection * boneLengths[boneIndex - 1];
		}
	}
	else
	{
		for (int iterationIndex = 0; iterationIndex < maxIterations; ++iterationIndex)
		{
			// Forward Reaching
			bonePositions.back() = targetPosition;
			for (int boneIndex = static_cast<int>(chainIndices.size() - 2); boneIndex >= 0; --boneIndex)
			{
				Vec3 targetDirection = (bonePositions[boneIndex] - bonePositions[boneIndex + 1]).GetNormalized();
				bonePositions[boneIndex] = bonePositions[boneIndex + 1] + targetDirection * boneLengths[boneIndex];
			}

			// Backward Reaching
			bonePositions[0] = rootPos;
			for (int boneIndex = 1; boneIndex < static_cast<int>(chainIndices.size()); ++boneIndex)
			{
				Vec3 targetDirection = (bonePositions[boneIndex] - bonePositions[boneIndex - 1]).GetNormalized();
				bonePositions[boneIndex] = bonePositions[boneIndex - 1] + targetDirection * boneLengths[boneIndex - 1];
			}

			if ((bonePositions.back() - targetPosition).GetLength() <= threshold)
			{
				break;
			}
		}
	}

	// Apply the new positions to our bones
	for (int chainIndex = 0; chainIndex < static_cast<int>(chainIndices.size()); ++chainIndex)
	{
		int boneIndex = chainIndices[chainIndex];
		Bone& bone = m_bones[boneIndex];

		if (bone.IsRootBone())
		{
			bone.SetLocalBonePosition(bonePositions[chainIndex]);
		}
		else
		{
			Vec3 parentWorldPos = bonePositions[chainIndex - 1];
			Vec3 boneLocalPos = bonePositions[chainIndex] - parentWorldPos;
			bone.SetLocalBonePosition(boneLocalPos);
		}
	}

	// Update the transforms
	UpdateSkeletonPose();
}
// -----------------------------------------------------------------------------

/* GEOMETRY */
// -----------------------------------------------------------------------------
void Skeleton::AddVertsForBone2D(std::vector<Vertex_PCU>& boneVerts, Bone const& bone, SkeletonStyle const& style)
{
	// Check if bone is being drawn
	if (!bone.m_isRenderable)
	{
		return;
	}

	// Get current position of bone in world space
	Vec2 bonePosition = bone.GetWorldBonePosition2D();
	Vec2 parentBonePos = Vec2::ZERO;

	// Check if bone has a parent bone
	if (bone.m_parentBoneIndex >= 0)
	{
		parentBonePos = m_bones[bone.m_parentBoneIndex].GetWorldBonePosition2D();
	}
	else
	{
		parentBonePos = bonePosition;
	}

	// Line is drawn between bone and its parent bone
	AddVertsForLineSegment2D(boneVerts, bonePosition, parentBonePos, style.m_boneRadius, style.m_boneColor);

	// Disc is drawn as the joint
	AddVertsForDisc2D(boneVerts, bonePosition, style.m_jointRadius, style.m_jointColor);
}

void Skeleton::AddVertsForBone3D(std::vector<Vertex_PCU>& boneVerts, Bone const& bone, SkeletonStyle const& style)
{
	// Check if bone is being drawn
	if (!bone.m_isRenderable)
	{
		return;
	}

	// Get current position of bone in world space
	Vec3 bonePosition = bone.GetWorldBonePosition3D();
	Vec3 parentBonePosition = Vec3::ZERO;

	// Check if bone has a parent bone
	if (bone.m_parentBoneIndex >= 0)
	{
		parentBonePosition = m_bones[bone.m_parentBoneIndex].GetWorldBonePosition3D();
	}
	else
	{
		parentBonePosition = bonePosition;
	}

	// Cylinder is drawn between bone and its parent bone
	AddVertsForCylinder3D(boneVerts, bonePosition, parentBonePosition, style.m_boneRadius, style.m_boneColor);

	// Sphere is drawn as the joint
	AddVertsForSphere3D(boneVerts, bonePosition, style.m_jointRadius, style.m_jointColor);
}

void Skeleton::AddVertsForSkeleton2D(std::vector<Vertex_PCU>& boneVerts, SkeletonStyle const& style)
{
	for (int boneIndex = 0; boneIndex < static_cast<int>(m_bones.size()); ++boneIndex)
	{
		Bone const& bone = m_bones[boneIndex];
		AddVertsForBone2D(boneVerts, bone, style);
	}
}

void Skeleton::AddVertsForSkeleton3D(std::vector<Vertex_PCU>& boneVerts, SkeletonStyle const& style)
{
	for (int boneIndex = 0; boneIndex < static_cast<int>(m_bones.size()); ++boneIndex)
	{
		Bone const& bone = m_bones[boneIndex];
		AddVertsForBone3D(boneVerts, bone, style);
	}
}

void Skeleton::AddVertsForBoneHierarchy(std::vector<Vertex_PCU>& textVerts, BitmapFont& font, float startYPos, float indentStep)
{
	float yPos = startYPos;
	GetBoneHierarchy(textVerts, -1, yPos, indentStep, font, 0);
}
// -----------------------------------------------------------------------------
