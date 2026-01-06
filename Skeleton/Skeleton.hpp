#pragma once
#include "Engine/Skeleton/Bone.hpp"
#include "Engine/Skeleton/Pose.hpp"
#include "Engine/Core/Vertex_PCU.h"
#include "Engine/Renderer/BitmapFont.hpp"
#include <vector>
// -----------------------------------------------------------------------------
struct SkeletonStyle
{
	SkeletonStyle() = default;

	float m_boneRadius  = 0.05f;
	float m_jointRadius = 0.1f;
	Rgba8 m_boneColor   = Rgba8::WHITE;
	Rgba8 m_jointColor  = Rgba8::WHITE;
};
// -----------------------------------------------------------------------------
class Skeleton
{
public:
	void UpdateSkeletonPose();
	void ResetSkeletonPose();

	// Geometry
	void AddVertsForBone2D(std::vector<Vertex_PCU>& boneVerts, Bone const& bone, SkeletonStyle const& style = SkeletonStyle());
	void AddVertsForBone3D(std::vector<Vertex_PCU>& boneVerts, Bone const& bone, SkeletonStyle const& style = SkeletonStyle());
	void AddVertsForSkeleton2D(std::vector<Vertex_PCU>& boneVerts, SkeletonStyle const& style = SkeletonStyle());
	void AddVertsForSkeleton3D(std::vector<Vertex_PCU>& boneVerts, SkeletonStyle const& style = SkeletonStyle());
	void AddVertsForBoneHierarchy(std::vector<Vertex_PCU>& textVerts, BitmapFont& font, float startYPos, float indentStep = 2.f);

	// Helper utilities
	int   FindBoneIndexByName(std::string const& boneName) const;
	Bone* GetBoneByName(std::string const& boneName);
	Bone* GetBoneByIndex(int boneIndex);
	void  GetBoneHierarchy(std::vector<Vertex_PCU>& textVerts, int parentIndex, float& textYPos, float indentStep, BitmapFont& font, int depth);

	// Bone/Skeleton math utilities
	float AngleBetweenBones(float a, float b, float c);
	void  ApplyIKToBone(Bone& bone, float angle, Vec3 const& axisOfRotation);

	// IK
	void SolveTwoBoneIK(int shoulderIndex, int elbowIndex, int handIndex, Vec3 targetPos);
	void SolveCCDIK(std::vector<int> const& chainIndices, Vec3 const& targetPosition, int maxIterations = 10, float threshold = 0.01f);
	void SolveCCDIKConstrained(std::vector<int> const& chainIndices, Vec3 const& targetPosition, int maxIterations = 10, float threshold = 0.01f);
	void SolveFABRIK(std::vector<int> const& chainIndices, Vec3 const& targetPosition, int maxIterations = 10, float threshold = 0.01f);

public:
	Mat44 m_skeletonModelTransform = Mat44();
	std::vector<Bone> m_bones;
};
// -----------------------------------------------------------------------------