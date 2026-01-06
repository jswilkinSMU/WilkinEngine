#include "Engine/Core/VertexUtils.h"
#include "Engine/Core/EngineCommon.h"
#include "Engine/Math/MathUtils.h"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/OBB3.hpp"

void TransformVertexArrayXY3D(int numVerts, Vertex_PCU* verts, float uniformScaleXY, float rotationDegreesAboutZ, Vec2 const& translationXY)
{
	for (int vertIndex = 0; vertIndex < numVerts; ++vertIndex)
	{
		Vec3& pos = verts[vertIndex].m_position;
		TransformPositionXY3D(pos, uniformScaleXY, rotationDegreesAboutZ, translationXY);
	}
}

void TransformVertexArray3D(std::vector<Vertex_PCU>& verts, Mat44 const& transform)
{
	for (int vertIndex = 0; vertIndex < (int)verts.size(); ++vertIndex)
	{
		Vec3& pos = verts[vertIndex].m_position;
		pos = transform.TransformPosition3D(pos);
	}
}

void TransformVertexArrayTBN3D(std::vector<Vertex_PCUTBN>& verts, Mat44 const& transform)
{
	for (int vertIndex = 0; vertIndex < (int)verts.size(); ++vertIndex)
	{
		Vec3& pos = verts[vertIndex].m_position;
		pos = transform.TransformPosition3D(pos);
	}
}

AABB2 GetVertexBounds(std::vector<Vertex_PCU> const& verts)
{
	float minX = verts[0].m_position.x;
	float minY = verts[0].m_position.y;
	float maxX = verts[0].m_position.x;
	float maxY = verts[0].m_position.y;

	for (int vertIndex = 1; vertIndex < (int)verts.size(); ++vertIndex)
	{
		Vec3 const& pos = verts[vertIndex].m_position;
		if (pos.x < minX)
		{
			minX = pos.x;
		}
		if (pos.y < minY)
		{
			minY = pos.y;
		}
		if (pos.x > maxX)
		{
			maxX = pos.x;
		}
		if (pos.y > maxY)
		{
			maxY = pos.y;
		}
	}
	return AABB2(minX, minY, maxX, maxY);
}

Vec2 GetDiscUV(Vec3 const& position, Vec3 const& center, Vec3 const& jBasis, Vec3 const& kBasis, float radius)
{
	Vec3 localPosition = position - center;
	float u = 0.5f + DotProduct3D(localPosition, jBasis) / (2.f * radius);
	float v = 0.5f + DotProduct3D(localPosition, kBasis) / (2.f * radius);
	return Vec2(u, v);
}

void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& discCenter, float discRadius, Rgba8 const& color)
{
	for (int sliceNum = 0; sliceNum < NUM_DISC_SLICES; ++sliceNum)
	{
		float sliceStartTheta = (360.f / NUM_DISC_SLICES) * sliceNum;
		float sliceEndTheta = (360.f / NUM_DISC_SLICES) * (sliceNum + 1);

		Vec2 point = discCenter + Vec2::MakeFromPolarDegrees(sliceStartTheta, discRadius);
		Vec2 point2 = discCenter + Vec2::MakeFromPolarDegrees(sliceEndTheta, discRadius);

		verts.push_back(Vertex_PCU(discCenter, color));
		verts.push_back(Vertex_PCU(point, color));
		verts.push_back(Vertex_PCU(point2, color));
	}
}

void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& alignedBox, Rgba8 const& color, Vec2 const& uvMins, Vec2 const& uvMaxs)
{
	Vec2 bottomLeft = alignedBox.m_mins;  
	Vec2 topLeft = Vec2(bottomLeft.x, alignedBox.m_maxs.y);
	Vec2 topRight = alignedBox.m_maxs; 
	Vec2 bottomRight = Vec2(alignedBox.m_maxs.x, bottomLeft.y);

	Vec2 uvBottomLeft = uvMins;
	Vec2 uvBottomRight = Vec2(uvMaxs.x, uvMins.y);
	Vec2 uvTopRight = uvMaxs;
	Vec2 uvTopLeft = Vec2(uvMins.x, uvMaxs.y);

	verts.push_back(Vertex_PCU(bottomLeft, color, uvBottomLeft));
	verts.push_back(Vertex_PCU(topLeft, color, uvTopLeft));
	verts.push_back(Vertex_PCU(topRight, color, uvTopRight));

	verts.push_back(Vertex_PCU(bottomLeft, color, uvBottomLeft));
	verts.push_back(Vertex_PCU(topRight, color, uvTopRight));
	verts.push_back(Vertex_PCU(bottomRight, color, uvBottomRight));
}

void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, OBB2 const& orientedBox, Rgba8 const& color)
{
	Vec2 corners[4];
	orientedBox.GetCornerPoints(corners);

	verts.push_back(Vertex_PCU(corners[0], color));
	verts.push_back(Vertex_PCU(corners[1], color));
	verts.push_back(Vertex_PCU(corners[2], color));

	verts.push_back(Vertex_PCU(corners[0], color));
	verts.push_back(Vertex_PCU(corners[2], color));
	verts.push_back(Vertex_PCU(corners[3], color));
}

void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Vec2 const& boneStart, Vec2 const& boneEnd, float radius, Rgba8 const& color)
{
	Vec2 forwardStep = boneEnd - boneStart;
	forwardStep.SetLength(radius);

	Vec2 leftStep = Vec2(-forwardStep.y, forwardStep.x);

	Vec2 StartRight = boneStart - leftStep;
	Vec2 StartLeft = boneStart + leftStep;
	Vec2 EndRight = boneEnd - leftStep;
	Vec2 EndLeft = boneEnd + leftStep;

	verts.push_back(Vertex_PCU(StartRight, color));
	verts.push_back(Vertex_PCU(EndRight, color));
	verts.push_back(Vertex_PCU(EndLeft, color));

	verts.push_back(Vertex_PCU(StartRight, color));
	verts.push_back(Vertex_PCU(EndLeft, color));
	verts.push_back(Vertex_PCU(StartLeft, color));

	float startTheta = forwardStep.GetOrientationDegrees() + 90.f;
	for (int sliceNum = 0; sliceNum < NUM_CAPSULE_SLICES; ++sliceNum)
	{
		float sliceStartTheta = startTheta + DEGREES_PER_CAPSULESLICE * static_cast<float>(sliceNum);
		float sliceEndTheta = sliceStartTheta + DEGREES_PER_CAPSULESLICE;

		Vec2 sliceStartDisplacement = Vec2::MakeFromPolarDegrees(sliceStartTheta, radius);
		Vec2 sliceEndDisplacement = Vec2::MakeFromPolarDegrees(sliceEndTheta, radius);

		Vec2 sliceStartOuterPos = boneStart + sliceStartDisplacement;
		Vec2 sliceEndOuterPos = boneStart + sliceEndDisplacement;

		verts.push_back(Vertex_PCU(boneStart, color));
		verts.push_back(Vertex_PCU(sliceStartOuterPos, color));
		verts.push_back(Vertex_PCU(sliceEndOuterPos, color));
	}

	startTheta = forwardStep.GetOrientationDegrees() - 90.f;
	for (int sliceNum = 0; sliceNum < NUM_CAPSULE_SLICES; ++sliceNum)
	{
		float sliceStartTheta = startTheta + DEGREES_PER_CAPSULESLICE * static_cast<float>(sliceNum);
		float sliceEndTheta = sliceStartTheta + DEGREES_PER_CAPSULESLICE;

		Vec2 sliceStartDisplacement = Vec2::MakeFromPolarDegrees(sliceStartTheta, radius);
		Vec2 sliceEndDisplacement = Vec2::MakeFromPolarDegrees(sliceEndTheta, radius);

		Vec2 sliceStartOuterPos = boneEnd + sliceStartDisplacement;
		Vec2 sliceEndOuterPos = boneEnd + sliceEndDisplacement;

		verts.push_back(Vertex_PCU(boneEnd, color));
		verts.push_back(Vertex_PCU(sliceEndOuterPos, color));
		verts.push_back(Vertex_PCU(sliceStartOuterPos, color));
	}
}

void AddVertsForTriangle2D(std::vector<Vertex_PCU>& verts, Vec2 const& ccw0, Vec2 const& ccw1, Vec2 const& ccw2, Rgba8 const& color)
{
	verts.push_back(Vertex_PCU(ccw0, color));
	verts.push_back(Vertex_PCU(ccw1, color));
	verts.push_back(Vertex_PCU(ccw2, color));
}

void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color)
{
	Vec2 startEnd = end - start;

	Vec2 h = Vec2(-startEnd.y, startEnd.x);
	h.Normalize();
	h *= thickness * 0.5f;

	Vec2 startLeft = start + h;
	Vec2 endLeft = end + h;
	Vec2 endRight = end - h;
	Vec2 startRight = start - h;

	verts.push_back(Vertex_PCU(startLeft, color));
	verts.push_back(Vertex_PCU(startRight, color));
	verts.push_back(Vertex_PCU(endRight, color));
	verts.push_back(Vertex_PCU(startLeft, color));
	verts.push_back(Vertex_PCU(endLeft, color));
	verts.push_back(Vertex_PCU(endRight, color));
}

void AddVertsForArrow2D(std::vector<Vertex_PCU>& verts, Vec2 tailPos, Vec2 tipPos, float arrowSize, float lineThickness, Rgba8 const& color)
{
	Vec2 tailToTip = tipPos - tailPos;
	tailToTip.Normalize();

	AddVertsForLineSegment2D(verts, tailPos, tipPos, lineThickness, color);

	float arrowHeadLength = arrowSize;
	float arrowHeadAngle = 145.f;

	Vec2 arrowLeft = tailToTip.GetRotatedDegrees(arrowHeadAngle);
	Vec2 arrowRight = tailToTip.GetRotatedDegrees(-arrowHeadAngle);
	Vec2 arrowLeftPoint = tipPos + arrowLeft * arrowHeadLength;
	Vec2 arrowRightPoint = tipPos + arrowRight * arrowHeadLength;
	
	AddVertsForLineSegment2D(verts, tipPos, arrowLeftPoint, lineThickness, color);
	AddVertsForLineSegment2D(verts, tipPos, arrowRightPoint, lineThickness, color);
}

void AddVertsForArc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, float startDegrees, float arcDegrees, Rgba8 const& color, int numSegments)
{
	float anglesPerStep = arcDegrees / static_cast<float>(numSegments);

	for (int segmentIndex = 0; segmentIndex < numSegments; ++segmentIndex)
	{
		float upperAngle = startDegrees + anglesPerStep * segmentIndex;
		float lowerAngle = startDegrees + anglesPerStep * (segmentIndex + 1);
		Vec2 point0 = center + radius * Vec2(CosDegrees(upperAngle), SinDegrees(upperAngle));
		Vec2 point1 = center + radius * Vec2(CosDegrees(lowerAngle), SinDegrees(lowerAngle));

		AddVertsForTriangle2D(verts, center, point0, point1, color);
	}
}

void AddVertsForDashedLine2D(std::vector<Vertex_PCU>& verts, Vec2 const& start, Vec2 const& end, float dashLength, float gapLength, float thickness, Rgba8 const& color)
{
	Vec2 lineDirection = end - start;
	float lineLength = lineDirection.GetLength();
	lineDirection.Normalize();

	float lineProgression = 0.f;
	while (lineProgression < lineLength)
	{
		float dashStart = lineProgression;
		float dashEnd = lineProgression + dashLength;

		if (dashEnd > lineLength)
		{
			dashEnd = lineLength;
		}

		Vec2 point0 = start + lineDirection * dashStart;
		Vec2 point1 = start + lineDirection * dashEnd;
		AddVertsForLineSegment2D(verts, point0, point1, thickness, color);
		lineProgression += dashLength + gapLength;
	}
}

void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Rgba8 const& color, AABB2 const& UVs)
{
	// Triangle A
	verts.push_back(Vertex_PCU(bottomLeft, color, UVs.m_mins));
	verts.push_back(Vertex_PCU(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y)));
	verts.push_back(Vertex_PCU(topRight, color, UVs.m_maxs));

	// Triangle B
	verts.push_back(Vertex_PCU(topRight, color, UVs.m_maxs));
	verts.push_back(Vertex_PCU(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y)));
	verts.push_back(Vertex_PCU(bottomLeft, color, UVs.m_mins));
}

void AddVertsForTriangle3D(std::vector<Vertex_PCU>& verts, Vec3 const& ccw0, Vec3 const& ccw1, Vec3 const& ccw2, Rgba8 const& color, AABB2 const& UVs)
{
	verts.push_back(Vertex_PCU(ccw0, color, UVs.m_mins));
	verts.push_back(Vertex_PCU(ccw1, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y)));
	verts.push_back(Vertex_PCU(ccw2, color, UVs.m_maxs));
}

void AddVertsForAABB3D(std::vector<Vertex_PCU>& verts, AABB3 const& bounds, Rgba8 const& color, AABB2 const& UVs)
{
	// Bottom Left and Bottom Right
	Vec3 bLNear = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_mins.z);
	Vec3 bRNear = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec3 bRFar = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec3 bLFar = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_mins.z);

	// Top Left and Top Right
	Vec3 tLNear = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_maxs.z);
	Vec3 tRNear = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_maxs.z);
	Vec3 tRFar = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_maxs.z);
	Vec3 tLFar = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_maxs.z);

	// Create aabb3d
	AddVertsForQuad3D(verts, bLNear, bRNear, tRNear, tLNear, color, UVs);
	AddVertsForQuad3D(verts, bRNear, bRFar, tRFar, tRNear, color, UVs);
	AddVertsForQuad3D(verts, bRFar, bLFar, tLFar, tRFar, color, UVs);
	AddVertsForQuad3D(verts, bLFar, bLNear, tLNear, tLFar, color, UVs);
	AddVertsForQuad3D(verts, tLNear, tRNear, tRFar, tLFar, color, UVs);
	AddVertsForQuad3D(verts, bLFar, bRFar, bRNear, bLNear, color, UVs);
}

void AddVertsForAABB3D(std::vector<Vertex_PCUTBN>& vertexes, AABB3 const& bounds, Rgba8 const& color /*= Rgba8::WHITE*/, AABB2 const& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	// Bottom Left and Bottom Right
	Vec3 bLNear = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_mins.z);
	Vec3 bRNear = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec3 bRFar = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec3 bLFar = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_mins.z);

	// Top Left and Top Right
	Vec3 tLNear = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_maxs.z);
	Vec3 tRNear = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_maxs.z);
	Vec3 tRFar = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_maxs.z);
	Vec3 tLFar = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_maxs.z);

	// Create aabb3d
	AddVertsForQuad3D(vertexes, bLNear, bRNear, tRNear, tLNear, color, UVs);
	AddVertsForQuad3D(vertexes, bRNear, bRFar, tRFar, tRNear, color, UVs);
	AddVertsForQuad3D(vertexes, bRFar, bLFar, tLFar, tRFar, color, UVs);
	AddVertsForQuad3D(vertexes, bLFar, bLNear, tLNear, tLFar, color, UVs);
	AddVertsForQuad3D(vertexes, tLNear, tRNear, tRFar, tLFar, color, UVs);
	AddVertsForQuad3D(vertexes, bLFar, bRFar, bRNear, bLNear, color, UVs);
}

void AddVertsForOBB3D(std::vector<Vertex_PCU>& verts, OBB3 const& box, Rgba8 const& color, AABB2 const& UVs)
{
	AABB3 alignedBox = AABB3(-box.m_halfDimensions, box.m_halfDimensions);
	AddVertsForAABB3D(verts, alignedBox, color, UVs);

	Mat44 transform = Mat44(box.m_iBasis, box.m_jBasis, box.m_kBasis, box.m_center);
	TransformVertexArray3D(verts, transform);
}

void AddVertsForSphere3D(std::vector<Vertex_PCU>& verts, Vec3 const& center, float radius, Rgba8 const& color, AABB2 const& UVs, int numSlices, int numStacks)
{
	float stackStep = 180.f / numStacks;
	float sliceStep = 360.f / numSlices;

	float uvStepX = UVs.GetDimensions().x / numSlices;
	float uvStepY = UVs.GetDimensions().y / numStacks;

	for (int stackIndex = 0; stackIndex < numStacks; ++stackIndex)
	{
		for (int sliceIndex = 0; sliceIndex < numSlices; ++sliceIndex)
		{
			float pitchDegrees = 90.f - stackIndex * stackStep;
			float yawDegrees = sliceIndex * sliceStep;

			Vec3 bL = center + Vec3::MakeFromPolarDegrees(pitchDegrees, yawDegrees, radius);
			Vec3 bR = center + Vec3::MakeFromPolarDegrees(pitchDegrees, yawDegrees + sliceStep, radius);
			Vec3 tL = center + Vec3::MakeFromPolarDegrees(pitchDegrees - stackStep, yawDegrees, radius);
			Vec3 tR = center + Vec3::MakeFromPolarDegrees(pitchDegrees - stackStep, yawDegrees + sliceStep, radius);

			AABB2 quadUVs = AABB2(sliceIndex * uvStepX, stackIndex * uvStepY, (sliceIndex + 1) * uvStepX, (stackIndex + 1) * uvStepY);

			AddVertsForQuad3D(verts, bL, bR, tR, tL, color, quadUVs);
		}
	}
}

void AddVertsForSphere3D(std::vector<Vertex_PCUTBN>& vertexes, Vec3 const& center, float radius, Rgba8 const& color, AABB2 const& UVS, int numSlices, int numStacks)
{
	float stackStep = 180.f / numStacks;
	float sliceStep = 360.f / numSlices;

	float uvStepX = UVS.GetDimensions().x / numSlices;
	float uvStepY = UVS.GetDimensions().y / numStacks;

	for (int stackIndex = 0; stackIndex < numStacks; ++stackIndex)
	{
		for (int sliceIndex = 0; sliceIndex < numSlices; ++sliceIndex)
		{
			float pitchDegrees = 90.f - stackIndex * stackStep;
			float yawDegrees = sliceIndex * sliceStep;

			Vec3 bL = center + Vec3::MakeFromPolarDegrees(pitchDegrees, yawDegrees, radius);
			Vec3 bR = center + Vec3::MakeFromPolarDegrees(pitchDegrees, yawDegrees + sliceStep, radius);
			Vec3 tL = center + Vec3::MakeFromPolarDegrees(pitchDegrees - stackStep, yawDegrees, radius);
			Vec3 tR = center + Vec3::MakeFromPolarDegrees(pitchDegrees - stackStep, yawDegrees + sliceStep, radius);
			Vec3 edge1 = bR - bL;
			Vec3 edge2 = tL - bL;

			// Calculate normals
			Vec3 normal_bL = (bL - center).GetNormalized();
			Vec3 normal_bR = (bR - center).GetNormalized();
			Vec3 normal_tL = (tL - center).GetNormalized();
			Vec3 normal_tR = (tR - center).GetNormalized();

			// Calculate UVs
			AABB2 quadUVs = AABB2(sliceIndex * uvStepX, stackIndex * uvStepY, (sliceIndex + 1) * uvStepX, (stackIndex + 1) * uvStepY);
			Vec2 uv_bL = quadUVs.m_mins;
			Vec2 uv_bR = Vec2(quadUVs.m_maxs.x, quadUVs.m_mins.y);
			Vec2 uv_tL = Vec2(quadUVs.m_mins.x, quadUVs.m_maxs.y);
			Vec2 uv_tR = quadUVs.m_maxs;
			Vec2 rightUV = uv_bR - uv_bL;
			Vec2 upUV = uv_tL - uv_bL;

			// Calculate tangent
			Vec3 tangent = (edge1 * upUV.y - edge2 * rightUV.y).GetNormalized();

			// Calculate bitangent
			Vec3 bitangent = (edge2 * rightUV.x - edge1 * upUV.x).GetNormalized();

			// First triangle
			vertexes.push_back(Vertex_PCUTBN(bL, color, uv_bL, tangent, bitangent, normal_bL));
			vertexes.push_back(Vertex_PCUTBN(bR, color, uv_bR, tangent, bitangent, normal_bR));
			vertexes.push_back(Vertex_PCUTBN(tR, color, uv_tR, tangent, bitangent, normal_tR));

			// Second triangle
			vertexes.push_back(Vertex_PCUTBN(bL, color, uv_bL, tangent, bitangent, normal_bL));
			vertexes.push_back(Vertex_PCUTBN(tR, color, uv_tR, tangent, bitangent, normal_tR));
			vertexes.push_back(Vertex_PCUTBN(tL, color, uv_tL, tangent, bitangent, normal_tL));
		}
	}
}

void AddVertsForCylinderZ3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, float radius, float height, Rgba8 color, AABB2 UVs, int numSlices)
{
	float sliceStep = 360.f / numSlices;
	Vec3 zUP = Vec3(0.f, 0.f, height);

	Vec3 iBasis = Vec3::XAXE; 
	iBasis.Normalize();

	Vec3 jBasis = Vec3::YAXE;
	jBasis.Normalize();

	Vec3 kBasis = Vec3::ZAXE;
	kBasis.Normalize();
	float kLength = kBasis.GetLength();

	Mat44 matrix;
	matrix.SetIJKT3D(iBasis, jBasis, kBasis, start);

	for (int sliceIndex = 0; sliceIndex < numSlices; ++sliceIndex)
	{
		AABB2 setUVs;
		UVs = setUVs;

		const float startDegree = sliceIndex * sliceStep;
		const float endDegree = (sliceIndex + 1) * sliceStep;

		float uvLeft = RangeMap(startDegree, 0.f, 360.f, 0.f, 1.f);
		float uvCenter = RangeMap(startDegree + (sliceStep / 2.f), 0.f, 360.f, 0.f, 1.f);
		float uvRight = RangeMap(endDegree, 0.f, 360.f, 0.f, 1.f);
		float bottomDiscY = (radius / (kLength + (2.f * radius)));
		float topDiscY = (radius + kLength) / (kLength + (2.f * radius));

		Vec3 bL = Vec3::MakeFromPolarDegrees(0.f, startDegree, radius);
		Vec3 bR = Vec3::MakeFromPolarDegrees(0.f, endDegree, radius);
		Vec3 tR = zUP + Vec3::MakeFromPolarDegrees(0.f, endDegree, radius);
		Vec3 tL = zUP + Vec3::MakeFromPolarDegrees(0.f, startDegree, radius);

		// Cylinder bottom disc
		verts.push_back(Vertex_PCU(Vec3::ZERO, color, Vec2(uvCenter, 0.f)));
		verts.push_back(Vertex_PCU(bR, color, Vec2(uvRight, bottomDiscY)));
		verts.push_back(Vertex_PCU(bL, color, Vec2(uvLeft, bottomDiscY)));

		// Cylinder body
		AABB2 quadUVs = AABB2(uvLeft, 0.f, uvRight, 1.f);
		AddVertsForQuad3D(verts, bL, bR, tR, tL, color, quadUVs);

		// Cylinder top disc
		verts.push_back(Vertex_PCU(zUP, color, Vec2(uvCenter, 1.f)));
		verts.push_back(Vertex_PCU(tL, color, Vec2(uvLeft, topDiscY)));
		verts.push_back(Vertex_PCU(tR, color, Vec2(uvRight, topDiscY)));
	}
	TransformVertexArray3D(verts, matrix);
}

void AddVertsForCylinder3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, Rgba8 const& color, AABB2 const& UVs, int numSlices)
{
	UNUSED(UVs);

	float sliceStep = 360.f / numSlices;
	Vec3 iBasis = end - start; 
	iBasis.Normalize();

	Vec3 jBasis = CrossProduct3D(iBasis, Vec3::ZAXE);
	if (jBasis.GetLengthSquared() < 0.01f)
	{
		jBasis = CrossProduct3D(iBasis, Vec3::YAXE);
	}
	jBasis.Normalize();

	Vec3 kBasis = CrossProduct3D(iBasis, jBasis);
	kBasis.Normalize();

	for (int sliceIndex = 0; sliceIndex < numSlices; ++sliceIndex)
	{
		const float startDegree = sliceIndex * sliceStep;
		const float endDegree = (sliceIndex + 1) * sliceStep;
		float uvLeft = RangeMap(startDegree, 0.f, 360.f, 0.f, 1.f);
		float uvRight = RangeMap(endDegree, 0.f, 360.f, 0.f, 1.f);

		Vec3 offsetBR = ((CosDegrees(startDegree) * jBasis + SinDegrees(startDegree) * kBasis) * radius);
		Vec3 offsetBL = ((CosDegrees(endDegree) * jBasis + SinDegrees(endDegree) * kBasis) * radius);

		Vec3 bL = start + offsetBL;
		Vec3 bR = start + offsetBR;
		Vec3 tL = end + offsetBL;
		Vec3 tR = end + offsetBR;

		// UVs
		Vec2 uv_tR_disc = GetDiscUV(tR, end, jBasis, kBasis, radius);
		Vec2 uv_tL_disc = GetDiscUV(tL, end, jBasis, kBasis, radius);
		Vec2 uv_center_top = Vec2::ONEHALF;
		AABB2 quadUVs = AABB2(uvLeft, 0.f, uvRight, 1.f);
		Vec2 uv_bR_disc = GetDiscUV(bR, start, jBasis, kBasis, radius);
		Vec2 uv_bL_disc = GetDiscUV(bL, start, jBasis, kBasis, radius);
		Vec2 uv_center_bottom = Vec2::ONEHALF;

		// Top disc
		verts.push_back(Vertex_PCU(tR, color, uv_tR_disc));
		verts.push_back(Vertex_PCU(tL, color, uv_tL_disc));
		verts.push_back(Vertex_PCU(end, color, uv_center_top));

		// Cylinder aabb
		AddVertsForQuad3D(verts, bR, bL, tL, tR, color, quadUVs);

		// Bottom disc
		verts.push_back(Vertex_PCU(bL, color, uv_bL_disc));
		verts.push_back(Vertex_PCU(bR, color, uv_bR_disc));
		verts.push_back(Vertex_PCU(start, color, uv_center_bottom));
	}
}

void AddVertsForCylinder3D(std::vector<Vertex_PCUTBN>& vertexes, Vec3 const& start, Vec3 const& end, float radius, Rgba8 const& color, AABB2 const& UVs, int numSlices)
{
	UNUSED(UVs);

	Vec3 iBasis = (end - start).GetNormalized();
	Vec3 jBasis = CrossProduct3D(iBasis, Vec3::ZAXE);

	if (jBasis.GetLengthSquared() < 0.01f)
	{
		jBasis = CrossProduct3D(iBasis, Vec3::YAXE);
	}

	jBasis.Normalize();
	Vec3 kBasis = CrossProduct3D(iBasis, jBasis);
	kBasis.Normalize();

	float sliceStep = 360.f / numSlices;

	for (int sliceIndex = 0; sliceIndex < numSlices; ++sliceIndex)
	{
		float startDegree = sliceIndex * sliceStep;
		float endDegree = (sliceIndex + 1) * sliceStep;
		float uvLeft = RangeMap(startDegree, 0.f, 360.f, 0.f, 1.f);
		float uvRight = RangeMap(endDegree, 0.f, 360.f, 0.f, 1.f);

		Vec3 offsetBR = ((CosDegrees(startDegree) * jBasis + SinDegrees(startDegree) * kBasis) * radius);
		Vec3 offsetBL = ((CosDegrees(endDegree) * jBasis + SinDegrees(endDegree) * kBasis) * radius);

		Vec3 bL = start + offsetBL;
		Vec3 bR = start + offsetBR;
		Vec3 tL = end + offsetBL;
		Vec3 tR = end + offsetBR;

		Vec3 normal_bL = bL.GetNormalized();
		Vec3 normal_bR = bR.GetNormalized();
		Vec3 normal_tL = tL.GetNormalized();
		Vec3 normal_tR = tR.GetNormalized();

		// UVs
		Vec2 uv_tR_disc = GetDiscUV(tR, end, jBasis, kBasis, radius);
		Vec2 uv_tL_disc = GetDiscUV(tL, end, jBasis, kBasis, radius);
		Vec2 uv_center_top = Vec2::ONEHALF;
		AABB2 quadUVs = AABB2(uvLeft, 0.f, uvRight, 1.f);
		Vec2 uv_bR_disc = GetDiscUV(bR, start, jBasis, kBasis, radius);
		Vec2 uv_bL_disc = GetDiscUV(bL, start, jBasis, kBasis, radius);
		Vec2 uv_center_bottom = Vec2::ONEHALF;

		Vec2 uv_bL = Vec2(quadUVs.m_mins.x, quadUVs.m_mins.y);
		Vec2 uv_bR = Vec2(quadUVs.m_maxs.x, quadUVs.m_mins.y);
		Vec2 uv_tL = Vec2(quadUVs.m_mins.x, quadUVs.m_maxs.y);
		Vec2 uv_tR = Vec2(quadUVs.m_maxs.x, quadUVs.m_maxs.y);

		Vec3 edge1 = bR - bL;
		Vec3 edge2 = tL - bL;
		Vec2 rightUV = uv_bR - uv_bL;
		Vec2 upUV = uv_tL - uv_bL;

		Vec3 tangent = (edge1 * upUV.y - edge2 * rightUV.y).GetNormalized();
		Vec3 bitangent = (edge2 * rightUV.x - edge1 * upUV.x).GetNormalized();

		// Top disc
		vertexes.push_back(Vertex_PCUTBN(tR, color, uv_tR_disc, tangent, bitangent, iBasis));
		vertexes.push_back(Vertex_PCUTBN(tL, color, uv_tL_disc, tangent, bitangent, iBasis));
		vertexes.push_back(Vertex_PCUTBN(end, color, uv_center_top, tangent, bitangent, iBasis));

		// Cylinder body
		AddVertsForQuad3D(vertexes, bR, bL, tL, tR, color, quadUVs);

		// Bottom disc
		vertexes.push_back(Vertex_PCUTBN(bL, color, uv_bL_disc, tangent, bitangent, -iBasis));
		vertexes.push_back(Vertex_PCUTBN(bR, color, uv_bR_disc, tangent, bitangent, -iBasis));
		vertexes.push_back(Vertex_PCUTBN(start, color, uv_center_bottom, tangent, bitangent, -iBasis));
	}
}

void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, Rgba8 const& color, AABB2 const& UVs, int numSlices)
{
	UNUSED(UVs);
	const float sliceStep = 360.f / numSlices;
	Vec3 coneBottom = start;
	Vec3 coneTop = end;

	Vec3 iBasis = end - start;
	iBasis.Normalize();
	
	Vec3 jBasis = CrossProduct3D(iBasis, Vec3::ZAXE);
	if (jBasis.GetLengthSquared() < 0.01f)
	{
		jBasis = CrossProduct3D(iBasis, Vec3::YAXE);
	}
	jBasis.Normalize();

	Vec3 kBasis = CrossProduct3D(iBasis, jBasis);
	kBasis.Normalize();

	for (int sliceIndex = 0; sliceIndex < numSlices; ++sliceIndex)
	{
		const float startDegree = sliceIndex * sliceStep;
		const float endDegree = ((sliceIndex + 1) * sliceStep);
		float uvLeft = RangeMap(startDegree, 0.f, 360.f, 0.f, 1.f);
		float uvRight = RangeMap(endDegree, 0.f, 360.f, 0.f, 1.f);

		Vec3 bR = ((CosDegrees(startDegree) * jBasis + SinDegrees(startDegree) * kBasis) * radius);
		Vec3 bL = ((CosDegrees(endDegree) * jBasis + SinDegrees(endDegree) * kBasis) * radius);

		// UVs
		AABB2 quadUVs = AABB2(uvLeft, 0.f, uvRight, 1.f);
		Vec2 uv_bR_disc = GetDiscUV(bR, start, jBasis, kBasis, radius);
		Vec2 uv_bL_disc = GetDiscUV(bL, start, jBasis, kBasis, radius);
		Vec2 uv_center_bottom = Vec2::ONEHALF;

		// Cone quad
		AddVertsForQuad3D(verts, bR + coneBottom, bL + coneBottom, coneTop, coneTop, color, quadUVs);

		// Cone bottom disc
		verts.push_back(Vertex_PCU(bL + coneBottom, color, uv_bL_disc));
		verts.push_back(Vertex_PCU(bR + coneBottom, color, uv_bR_disc));
		verts.push_back(Vertex_PCU(coneBottom, color, uv_center_bottom));
	}
}

void AddVertsForArrow3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, Rgba8 const& color, int numSlices)
{
	Vec3 cylinderEnd = start + (end - start) * 0.7f;
	AddVertsForCylinder3D(verts, start, cylinderEnd, radius * 0.6f, color, AABB2::ZERO_TO_ONE, numSlices);
	AddVertsForCone3D(verts, cylinderEnd, end, radius, color, AABB2::ZERO_TO_ONE, numSlices);
}

void AddVertsForMathArrow3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, Rgba8 const& color, int numSlices)
{
	Vec3 cylinderEnd = start + (end - start) * 0.7f;
	AddVertsForCylinder3D(verts, start, cylinderEnd, radius * 0.2f, color, AABB2::ZERO_TO_ONE, numSlices);

	Vec3 arrowDirection = (end - cylinderEnd);
	arrowDirection.Normalize();

	float pyramidSize = radius * 0.9f;
	float pyramidHeight = (end - cylinderEnd).GetLength();
	AddVertsForPyramid3D(verts, cylinderEnd, arrowDirection, pyramidSize, pyramidHeight, color, AABB2::ZERO_TO_ONE);
}

void AddVertsForPyramidZ3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, float size, float height, Rgba8 const& color, AABB2 const& UVs)
{
	Vec3 pyramidHalf = Vec3(size * 0.5f, size * 0.5f, 0.f);
	Vec3 pyramidApex = start + Vec3(0.f, 0.f, height);

	Vec3 bL = (start - pyramidHalf);
	Vec3 bR = (start + Vec3(pyramidHalf.x, -pyramidHalf.y, 0.f));
	Vec3 tR = (start + pyramidHalf);
	Vec3 tL = (start + Vec3(-pyramidHalf.x, pyramidHalf.y, 0.f));

	// Pyramid base
	AddVertsForQuad3D(verts, bL, tL, tR, bR, color, UVs);

	// Pyramid sides
	AddVertsForTriangle3D(verts, bL, bR, pyramidApex, color, UVs);
	AddVertsForTriangle3D(verts, bR, tR, pyramidApex, color, UVs);
	AddVertsForTriangle3D(verts, tR, tL, pyramidApex, color, UVs);
	AddVertsForTriangle3D(verts, tL, bL, pyramidApex, color, UVs);
}

void AddVertsForPyramid3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float size, float height, Rgba8 const& color, AABB2 const& UVs)
{
	Vec3 pyramidHalf = Vec3(size * 0.5f, size * 0.5f, 0.f);

	Vec3 iBasis = end;
	iBasis.Normalize();
	Vec3 jBasis = CrossProduct3D(iBasis, Vec3::ZAXE);
	if (jBasis.GetLengthSquared() < 0.01f)
	{
		jBasis = CrossProduct3D(iBasis, Vec3::YAXE);
	}
	jBasis.Normalize();

	Vec3 kBasis = CrossProduct3D(iBasis, jBasis);
	kBasis.Normalize();

	Vec3 bL = (start - (pyramidHalf.x * jBasis) - (pyramidHalf.y * kBasis));
	Vec3 bR = (start + (pyramidHalf.x * jBasis) - (pyramidHalf.y * kBasis));
	Vec3 tR = (start + (pyramidHalf.x * jBasis) + (pyramidHalf.y * kBasis));
	Vec3 tL = (start - (pyramidHalf.x * jBasis) + (pyramidHalf.y * kBasis));

	Vec3 pyramidApex = start + height * iBasis;

	// Pyramid base
	AddVertsForQuad3D(verts, bL, tL, tR, bR, color, UVs);

	// Pyramid sides
	AddVertsForTriangle3D(verts, bL, bR, pyramidApex, color, UVs);
	AddVertsForTriangle3D(verts, bR, tR, pyramidApex, color, UVs);
	AddVertsForTriangle3D(verts, tR, tL, pyramidApex, color, UVs);
	AddVertsForTriangle3D(verts, tL, bL, pyramidApex, color, UVs);
}

void AddVertsForRotatedAABB3D(std::vector<Vertex_PCU>& verts, AABB3 const& box, float degrees, Vec3 const& pivot, Rgba8 const& color, AABB2 const& UVs)
{
	std::vector<Vertex_PCU> tempVerts;
	AddVertsForAABB3D(tempVerts, box, color, UVs);
	Mat44 boxRotation = Mat44::MakeYRotationDegrees(degrees);

	for (Vertex_PCU& vert : tempVerts)
	{
		Vec3 position = vert.m_position;

		position -= pivot;
		position = boxRotation.TransformPosition3D(position);
		position += pivot;

		vert.m_position = position;
	}

	verts.insert(verts.end(), tempVerts.begin(), tempVerts.end());
}

void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Rgba8 const& color, AABB2 const& UVs)
{
	Vec3 bottomLeftNormal = CrossProduct3D(bottomRight - bottomLeft, topLeft - bottomLeft).GetNormalized();
	Vec3 bottomRightNormal = CrossProduct3D(topRight - bottomRight, bottomLeft - bottomRight).GetNormalized();
	Vec3 topRightNormal = CrossProduct3D(topLeft - topRight, bottomRight - topRight).GetNormalized();
	Vec3 topLeftNormal = CrossProduct3D(bottomLeft - topLeft, topRight - topLeft).GetNormalized();

	Vec3 edge1 = bottomRight - bottomLeft;
	Vec3 edge2 = topLeft - bottomLeft;
	Vec3 fullNormal = CrossProduct3D(edge1, edge2).GetNormalized();

	// Calculate UVs
	Vec2 uv_bL = UVs.m_mins;
	Vec2 uv_bR = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	Vec2 uv_tL = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	Vec2 uv_tR = UVs.m_maxs;
	Vec2 rightUV = uv_bR - uv_bL;
	Vec2 upUV = uv_tL - uv_bL;

	// Calculate tangent and bitangent
	Vec3 tangent = Vec3::ZERO;
	Vec3 biTangent = Vec3::ZERO;
	if (fullNormal == Vec3::ZAXE)
	{
		tangent = Vec3::XAXE;
		biTangent = Vec3::YAXE;
	}
	else if (fullNormal == -Vec3::ZAXE)
	{
		tangent = -Vec3::XAXE;
		biTangent = -Vec3::YAXE;
	}
	else
	{
		tangent = ((edge1 * upUV.y - edge2 * rightUV.y)).GetNormalized();
		biTangent = ((edge2 * rightUV.x - edge1 * upUV.x)).GetNormalized();
	}

	// First triangle
	verts.push_back(Vertex_PCUTBN(bottomLeft, color, UVs.m_mins, tangent, biTangent, bottomLeftNormal));
	verts.push_back(Vertex_PCUTBN(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y), tangent, biTangent, bottomRightNormal));
	verts.push_back(Vertex_PCUTBN(topRight, color, UVs.m_maxs, tangent, biTangent, topRightNormal));

	// Second triangle
	verts.push_back(Vertex_PCUTBN(bottomLeft, color, UVs.m_mins, tangent, biTangent, bottomLeftNormal));
	verts.push_back(Vertex_PCUTBN(topRight, color, UVs.m_maxs, tangent, biTangent, topRightNormal));
	verts.push_back(Vertex_PCUTBN(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y), tangent, biTangent, topLeftNormal));
}

void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Rgba8 const& color, AABB2 const& UVs)
{
	unsigned int index = static_cast<unsigned int>(verts.size());
	indexes.push_back(index);
	indexes.push_back(index + 1);
	indexes.push_back(index + 2);
	indexes.push_back(index);
	indexes.push_back(index + 2);
	indexes.push_back(index + 3);

	Vec3 edge1 = bottomRight - bottomLeft;
	Vec3 edge2 = topLeft - bottomLeft;

	// Calculate normals
	Vec3 fullNormal = CrossProduct3D(edge1, edge2).GetNormalized();
	Vec3 bottomLeftNormal = CrossProduct3D(bottomRight - bottomLeft, topLeft - bottomLeft).GetNormalized();
	Vec3 bottomRightNormal = CrossProduct3D(topRight - bottomRight, bottomLeft - bottomRight).GetNormalized();
	Vec3 topRightNormal = CrossProduct3D(topLeft - topRight, bottomRight - topRight).GetNormalized();
	Vec3 topLeftNormal = CrossProduct3D(bottomLeft - topLeft, topRight - topLeft).GetNormalized();

	// Calculate UVs
	Vec2 uv_bL = UVs.m_mins;
	Vec2 uv_bR = Vec2(UVs.m_maxs.x, UVs.m_mins.y);
	Vec2 uv_tL = Vec2(UVs.m_mins.x, UVs.m_maxs.y);
	Vec2 uv_tR = UVs.m_maxs;
	Vec2 rightUV = uv_bR - uv_bL;
	Vec2 upUV = uv_tL - uv_bL;

	// Calculate tangent and bitangent
	Vec3 tangent = Vec3::ZERO;
	Vec3 biTangent = Vec3::ZERO;
	if (fullNormal == Vec3::ZAXE)
	{
		tangent = Vec3::XAXE;
		biTangent = Vec3::YAXE;
	}
	else if (fullNormal == -Vec3::ZAXE)
	{
		tangent = -Vec3::XAXE;
		biTangent = -Vec3::YAXE;
	}
	else
	{
		tangent = ((edge1 * upUV.y - edge2 * rightUV.y)).GetNormalized();
		biTangent = ((edge2 * rightUV.x - edge1 * upUV.x)).GetNormalized();
	}

	verts.push_back(Vertex_PCUTBN(bottomLeft, color, uv_bL, tangent, biTangent, bottomLeftNormal));
	verts.push_back(Vertex_PCUTBN(bottomRight, color, uv_bR, tangent, biTangent, bottomRightNormal));
	verts.push_back(Vertex_PCUTBN(topRight, color, uv_tR, tangent, biTangent, topRightNormal));
	verts.push_back(Vertex_PCUTBN(topLeft, color, uv_tL, tangent, biTangent, topLeftNormal));
}

void AddVertsForRoundedQuad3D(std::vector<Vertex_PCUTBN>& vertexes, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Rgba8 const& color, AABB2 const& UVs)
{
	// Calculate midway points
	float bottomHalf = (bottomRight - bottomLeft).GetLength() * 0.5f;
	float topHalf = (topRight - topLeft).GetLength() * 0.5f;
	Vec3 bottomMiddle = bottomLeft + (bottomRight - bottomLeft).GetNormalized() * bottomHalf;
	Vec3 topMiddle = topLeft + (topRight - topLeft).GetNormalized() * topHalf;

	// Get midway uv coordinates
	Vec2 bMUVs = Vec2(UVs.GetCenter().x, UVs.m_mins.y);
	Vec2 tMUVs = Vec2(UVs.GetCenter().x, UVs.m_maxs.y);

	// Calculate bottom normals
	Vec3 bLNormal = (bottomLeft - bottomMiddle).GetNormalized();
	Vec3 bMNormal = (CrossProduct3D(topMiddle - bottomMiddle, bottomLeft - bottomMiddle).GetNormalized() + CrossProduct3D(bottomRight - bottomMiddle, topMiddle - bottomMiddle).GetNormalized()) * 0.5f;
	Vec3 bRNormal = (bottomRight - bottomMiddle).GetNormalized();

	// Calculate top normals
	Vec3 tRNormal = (topRight - topMiddle).GetNormalized();
	Vec3 tMNormal = (CrossProduct3D(bottomMiddle - topMiddle, topRight - topMiddle).GetNormalized() + CrossProduct3D(topLeft - topMiddle, bottomMiddle - topMiddle).GetNormalized()) * 0.5f;
	Vec3 tLNormal = (topLeft - topMiddle).GetNormalized();

	// Push back vertexes first quad
	vertexes.push_back(Vertex_PCUTBN(bottomLeft, color, UVs.m_mins, Vec3::ZERO, Vec3::ZERO, bLNormal));
	vertexes.push_back(Vertex_PCUTBN(bottomMiddle, color, bMUVs, Vec3::ZERO, Vec3::ZERO, bMNormal));
	vertexes.push_back(Vertex_PCUTBN(topMiddle, color, tMUVs, Vec3::ZERO, Vec3::ZERO, tMNormal));
	vertexes.push_back(Vertex_PCUTBN(bottomLeft, color, UVs.m_mins, Vec3::ZERO, Vec3::ZERO, bLNormal));
	vertexes.push_back(Vertex_PCUTBN(topMiddle, color, tMUVs, Vec3::ZERO, Vec3::ZERO, tMNormal));
	vertexes.push_back(Vertex_PCUTBN(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y), Vec3::ZERO, Vec3::ZERO, tLNormal));

	// Push back vertexes second quad
	vertexes.push_back(Vertex_PCUTBN(bottomMiddle, color, bMUVs, Vec3::ZERO, Vec3::ZERO, bMNormal));
	vertexes.push_back(Vertex_PCUTBN(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y), Vec3::ZERO, Vec3::ZERO, bRNormal));
	vertexes.push_back(Vertex_PCUTBN(topRight, color, UVs.m_maxs, Vec3::ZERO, Vec3::ZERO, tRNormal));
	vertexes.push_back(Vertex_PCUTBN(bottomMiddle, color, bMUVs, Vec3::ZERO, Vec3::ZERO, bMNormal));
	vertexes.push_back(Vertex_PCUTBN(topRight, color, UVs.m_maxs, Vec3::ZERO, Vec3::ZERO, tRNormal));
	vertexes.push_back(Vertex_PCUTBN(topMiddle, color, tMUVs, Vec3::ZERO, Vec3::ZERO, tMNormal));
}

void AddVertsForAABB3D(std::vector<Vertex_PCUTBN>& vertexes, std::vector<unsigned int>& indexes, AABB3 const& bounds, Rgba8 const& color, AABB2 const& UVs)
{
	// Bottom Left and Bottom Right
	Vec3 bLNear = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_mins.z);
	Vec3 bRNear = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec3 bRFar = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec3 bLFar = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_mins.z);

	// Top Left and Top Right
	Vec3 tLNear = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_maxs.z);
	Vec3 tRNear = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_maxs.z);
	Vec3 tRFar = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_maxs.z);
	Vec3 tLFar = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_maxs.z);

	// Create aabb3d
	AddVertsForQuad3D(vertexes, indexes, bLNear, bRNear, tRNear, tLNear, color, UVs);
	AddVertsForQuad3D(vertexes, indexes, bRNear, bRFar, tRFar, tRNear, color, UVs);
	AddVertsForQuad3D(vertexes, indexes, bRFar, bLFar, tLFar, tRFar, color, UVs);
	AddVertsForQuad3D(vertexes, indexes, bLFar, bLNear, tLNear, tLFar, color, UVs);
	AddVertsForQuad3D(vertexes, indexes, tLNear, tRNear, tRFar, tLFar, color, UVs);
	AddVertsForQuad3D(vertexes, indexes, bLFar, bRFar, bRNear, bLNear, color, UVs);
}

void AddVertsForOBB3D(std::vector<Vertex_PCUTBN>& vertexes, std::vector<unsigned int>& indexes, OBB3 const& bounds, Rgba8 const& color, AABB2 const& UVs)
{
	Vec3 corners[8];
	bounds.GetCornerPoints(corners);

	// Bottom Front
	AddVertsForQuad3D(vertexes, indexes, corners[1], corners[0], corners[4], corners[5], color, UVs);

	// Bottom Right
	AddVertsForQuad3D(vertexes, indexes, corners[2], corners[3], corners[7], corners[6], color, UVs); 

	// Bottom Back
	AddVertsForQuad3D(vertexes, indexes, corners[3], corners[1], corners[5], corners[7], color, UVs); 

	// Bottom Left
	AddVertsForQuad3D(vertexes, indexes, corners[0], corners[2], corners[6], corners[4], color, UVs); 

	// Top Front
	AddVertsForQuad3D(vertexes, indexes, corners[5], corners[4], corners[6], corners[7], color, UVs); 

	// Top Back
	AddVertsForQuad3D(vertexes, indexes, corners[3], corners[2], corners[0], corners[1], color, UVs); 

}

void AddVertsForSphere3D(std::vector<Vertex_PCUTBN>& vertexes, std::vector<unsigned int>& indexes, Vec3 const& center, float radius, Rgba8 const& color, AABB2 const& UVS, int numSlices, int numStacks)
{
	float stackStep = 180.f / numStacks;
	float sliceStep = 360.f / numSlices;

	float uvStepX = UVS.GetDimensions().x / numSlices;
	float uvStepY = UVS.GetDimensions().y / numStacks;

	for (int stackIndex = 0; stackIndex < numStacks; ++stackIndex)
	{
		for (int sliceIndex = 0; sliceIndex < numSlices; ++sliceIndex)
		{
			float pitchDegrees = 90.f - stackIndex * stackStep;
			float yawDegrees = sliceIndex * sliceStep;

			Vec3 bL = center + Vec3::MakeFromPolarDegrees(pitchDegrees, yawDegrees, radius);
			Vec3 bR = center + Vec3::MakeFromPolarDegrees(pitchDegrees, yawDegrees + sliceStep, radius);
			Vec3 tL = center + Vec3::MakeFromPolarDegrees(pitchDegrees - stackStep, yawDegrees, radius);
			Vec3 tR = center + Vec3::MakeFromPolarDegrees(pitchDegrees - stackStep, yawDegrees + sliceStep, radius);
			Vec3 edge1 = bR - bL;
			Vec3 edge2 = tL - bL;

			// Calculate normals
			Vec3 normal_bL = (bL - center).GetNormalized();
			Vec3 normal_bR = (bR - center).GetNormalized();
			Vec3 normal_tL = (tL - center).GetNormalized();
			Vec3 normal_tR = (tR - center).GetNormalized();

			// Calculate UVs
			AABB2 quadUVs = AABB2(sliceIndex * uvStepX, stackIndex * uvStepY, (sliceIndex + 1) * uvStepX, (stackIndex + 1) * uvStepY);
			Vec2 uv_bL = quadUVs.m_mins;
			Vec2 uv_bR = Vec2(quadUVs.m_maxs.x, quadUVs.m_mins.y);
			Vec2 uv_tL = Vec2(quadUVs.m_mins.x, quadUVs.m_maxs.y);
			Vec2 uv_tR = quadUVs.m_maxs;
			Vec2 rightUV = uv_bR - uv_bL;
			Vec2 upUV = uv_tL - uv_bL;

			// Calculate tangent
			Vec3 tangent = (edge1 * upUV.y - edge2 * rightUV.y).GetNormalized();

			// Calculate bitangent
			Vec3 bitangent = (edge2 * rightUV.x - edge1 * upUV.x).GetNormalized();

			vertexes.push_back(Vertex_PCUTBN(bL, color, quadUVs.m_mins, tangent, bitangent, normal_bL));
			vertexes.push_back(Vertex_PCUTBN(bR, color, Vec2(quadUVs.m_maxs.x, quadUVs.m_mins.y), tangent, bitangent, normal_bR));
			vertexes.push_back(Vertex_PCUTBN(tR, color, quadUVs.m_maxs, tangent, bitangent, normal_tR));
			vertexes.push_back(Vertex_PCUTBN(tL, color, Vec2(quadUVs.m_mins.x, quadUVs.m_maxs.y), tangent, bitangent, normal_tL));

			unsigned int index = static_cast<unsigned int>(vertexes.size());
			indexes.push_back(index);
			indexes.push_back(index + 1);
			indexes.push_back(index + 2);
			indexes.push_back(index);
			indexes.push_back(index + 2);
			indexes.push_back(index + 3);
		}
	}
}

void AddVertsForCylinderZ3D(std::vector<Vertex_PCUTBN>& vertexes, std::vector<unsigned int>& indexes, Vec3 const& start, float radius, float height, Rgba8 const& color, AABB2 const& UVs, int numSlices)
{
	UNUSED(UVs);

	Vec3 cylinderBase = start;
	cylinderBase = Vec3::ZERO;

	float sliceStep = 360.f / numSlices;
	Vec3 zUP = Vec3(0.f, 0.f, height);  
	Vec3 topCenter = zUP;

	for (int sliceIndex = 0; sliceIndex < numSlices; ++sliceIndex)
	{
		const float startDegree = sliceIndex * sliceStep;
		const float endDegree = (sliceIndex + 1) * sliceStep;
		float uvLeft = RangeMap(startDegree, 0.f, 360.f, 0.f, 1.f);
		float uvCenter = RangeMap(startDegree + (sliceStep / 2.f), 0.f, 360.f, 0.f, 1.f);
		float uvRight = RangeMap(endDegree, 0.f, 360.f, 0.f, 1.f);

		Vec3 bL = Vec3::MakeFromPolarDegrees(0.f, startDegree, radius);
		Vec3 bR = Vec3::MakeFromPolarDegrees(0.f, endDegree, radius);
		Vec3 tL = zUP + Vec3::MakeFromPolarDegrees(0.f, startDegree, radius);
		Vec3 tR = zUP + Vec3::MakeFromPolarDegrees(0.f, endDegree, radius);

		// Calculate normals
		Vec3 normal_bL = bL.GetNormalized();  
		Vec3 normal_bR = bR.GetNormalized();  
		Vec3 normal_tL = tL.GetNormalized();  
		Vec3 normal_tR = tR.GetNormalized();  

		Vec3 edge1 = bR - bL;
		Vec3 edge2 = tR - bL;

		Vec2 uv_bL = Vec2(uvLeft, 0.f);
		Vec2 uv_bR = Vec2(uvRight, 0.f);
		Vec2 uv_tL = Vec2(uvLeft, 1.f);
		Vec2 uv_tR = Vec2(uvRight, 1.f);
		Vec2 rightUV = uv_bR - uv_bL;
		Vec2 upUV = uv_tL - uv_bL;

		// Calculate tangent and bitangent
		Vec3 tangent = (edge1 * upUV.y - edge2 * rightUV.y).GetNormalized();
		Vec3 bitangent = (edge2 * rightUV.x - edge1 * upUV.x).GetNormalized();

		// Bottom Disc
		vertexes.push_back(Vertex_PCUTBN(cylinderBase, color, Vec2(uvCenter, 0.f), tangent, bitangent, -Vec3::ZAXE));
		vertexes.push_back(Vertex_PCUTBN(bR, color, Vec2(uvRight, 0.f), tangent, bitangent, normal_bR));
		vertexes.push_back(Vertex_PCUTBN(bL, color, Vec2(uvLeft, 0.f), tangent, bitangent, normal_bL));

		// Cylinder Body
		AABB2 quadUVs = AABB2(uvLeft, 0.f, uvRight, 1.f);
		vertexes.push_back(Vertex_PCUTBN(bL, color, Vec2(uvLeft, 0.f), tangent, bitangent, normal_bL));
		vertexes.push_back(Vertex_PCUTBN(bR, color, Vec2(uvRight, 0.f), tangent, bitangent, normal_bR));
		vertexes.push_back(Vertex_PCUTBN(tR, color, Vec2(uvRight, 1.f), tangent, bitangent, normal_tR));
		vertexes.push_back(Vertex_PCUTBN(bL, color, Vec2(uvLeft, 0.f), tangent, bitangent, normal_bL));
		vertexes.push_back(Vertex_PCUTBN(tR, color, Vec2(uvRight, 1.f), tangent, bitangent, normal_tR));
		vertexes.push_back(Vertex_PCUTBN(tL, color, Vec2(uvLeft, 1.f), tangent, bitangent, normal_tL));

		// Top Disc
		vertexes.push_back(Vertex_PCUTBN(topCenter, color, Vec2(uvCenter, 1.f), tangent, bitangent, Vec3::ZAXE));
		vertexes.push_back(Vertex_PCUTBN(tL, color, Vec2(uvLeft, 1.f), tangent, bitangent, normal_tL));
		vertexes.push_back(Vertex_PCUTBN(tR, color, Vec2(uvRight, 1.f), tangent, bitangent, normal_tR));

		int indexOffset = sliceIndex * 12;
		indexes.push_back(indexOffset);
		indexes.push_back(indexOffset + 1);
		indexes.push_back(indexOffset + 2);

		indexes.push_back(indexOffset + 3);
		indexes.push_back(indexOffset + 4);
		indexes.push_back(indexOffset + 5);

		indexes.push_back(indexOffset + 6);
		indexes.push_back(indexOffset + 7);
		indexes.push_back(indexOffset + 8);

		indexes.push_back(indexOffset + 9);
		indexes.push_back(indexOffset + 10);
		indexes.push_back(indexOffset + 11);
	}
}

void AddVertsForCylinderOriented3D(std::vector<Vertex_PCUTBN>& vertexes, std::vector<unsigned int>& indexes, Vec3 const& start, Vec3 const& end, float radius, Rgba8 const& color, AABB2 const& UVs, int numSlices)
{
	UNUSED(UVs);

    Vec3 iBasis = (end - start).GetNormalized();
    Vec3 jBasis = CrossProduct3D(iBasis, Vec3::ZAXE);

    if (jBasis.GetLengthSquared() < 0.01f)
    {
        jBasis = CrossProduct3D(iBasis, Vec3::YAXE);
    }

    jBasis.Normalize();
    Vec3 kBasis = CrossProduct3D(iBasis, jBasis);
    kBasis.Normalize();

    float sliceStep = 360.f / numSlices;

    for (int sliceIndex = 0; sliceIndex < numSlices; ++sliceIndex)
    {
        float startAngle = sliceIndex * sliceStep;
        float endAngle = (sliceIndex + 1) * sliceStep;

        Vec3 offsetBL = (CosDegrees(startAngle) * jBasis + SinDegrees(startAngle) * kBasis) * radius;
        Vec3 offsetBR = (CosDegrees(endAngle) * jBasis + SinDegrees(endAngle) * kBasis) * radius;

        Vec3 bL = start + offsetBL;
        Vec3 bR = start + offsetBR;
        Vec3 tL = end + offsetBL;
        Vec3 tR = end + offsetBR;

        Vec3 normal_bL = offsetBL.GetNormalized();
        Vec3 normal_bR = offsetBR.GetNormalized();
        Vec3 normal_tL = normal_bL;
        Vec3 normal_tR = normal_bR;

        // UV mapping (wraps around horizontally)
        float u0 = static_cast<float>(sliceIndex) / numSlices;
        float u1 = static_cast<float>(sliceIndex + 1) / numSlices;

        AABB2 quadUVs(u0, 0.f, u1, 1.f);
        Vec2 uv_bL = Vec2(quadUVs.m_mins.x, quadUVs.m_mins.y);
        Vec2 uv_bR = Vec2(quadUVs.m_maxs.x, quadUVs.m_mins.y);
        Vec2 uv_tL = Vec2(quadUVs.m_mins.x, quadUVs.m_maxs.y);
        Vec2 uv_tR = Vec2(quadUVs.m_maxs.x, quadUVs.m_maxs.y);

        Vec3 edge1 = bR - bL;
        Vec3 edge2 = tL - bL;
        Vec2 rightUV = uv_bR - uv_bL;
        Vec2 upUV = uv_tL - uv_bL;

        Vec3 tangent = (edge1 * upUV.y - edge2 * rightUV.y).GetNormalized();
        Vec3 bitangent = (edge2 * rightUV.x - edge1 * upUV.x).GetNormalized();

        // Body vertices
        vertexes.push_back(Vertex_PCUTBN(bL, color, uv_bL, tangent, bitangent, normal_bL)); // 0
        vertexes.push_back(Vertex_PCUTBN(bR, color, uv_bR, tangent, bitangent, normal_bR)); // 1
        vertexes.push_back(Vertex_PCUTBN(tR, color, uv_tR, tangent, bitangent, normal_tR)); // 2
        vertexes.push_back(Vertex_PCUTBN(tL, color, uv_tL, tangent, bitangent, normal_tL)); // 3

        // Bottom disc
        vertexes.push_back(Vertex_PCUTBN(start, color, Vec2((u0 + u1) * 0.5f, 0.f), tangent, bitangent, -iBasis)); 
        vertexes.push_back(Vertex_PCUTBN(bR, color, uv_bR, tangent, bitangent, -iBasis));                        
        vertexes.push_back(Vertex_PCUTBN(bL, color, uv_bL, tangent, bitangent, -iBasis));        

        // Top disc
        vertexes.push_back(Vertex_PCUTBN(end, color, Vec2((u0 + u1) * 0.5f, 1.f), tangent, bitangent, iBasis));     
        vertexes.push_back(Vertex_PCUTBN(tL, color, uv_tL, tangent, bitangent, iBasis));                          
        vertexes.push_back(Vertex_PCUTBN(tR, color, uv_tR, tangent, bitangent, iBasis));       

		unsigned int base = static_cast<unsigned int>(vertexes.size());
		indexes.push_back(base);
		indexes.push_back(base + 1);
		indexes.push_back(base + 2);

		indexes.push_back(base + 3);
		indexes.push_back(base + 4);
		indexes.push_back(base + 5);

		indexes.push_back(base + 6);
		indexes.push_back(base + 7);
		indexes.push_back(base + 8);

		indexes.push_back(base + 9);
		indexes.push_back(base + 10);
		indexes.push_back(base + 11);

		indexes.push_back(base + 12);
		indexes.push_back(base + 13);
		indexes.push_back(base + 14);
    }
}