#pragma once
#include "Engine/Core/Vertex_PCU.h"
#include "Engine/Core/Vertex_PCUTBN.hpp"
#include "Engine/Math/AABB2.h"
#include <Vector>
// -----------------------------------------------------------------------------
constexpr int NUM_CAPSULE_SLICES = 16;
constexpr float DEGREES_PER_CAPSULESLICE = 180.f / (static_cast<float>(NUM_CAPSULE_SLICES));
constexpr int NUM_DISC_SLICES = 30;
// -----------------------------------------------------------------------------
class  Mat44;
struct AABB3;
struct OBB2;
struct OBB3;
struct LineSegment2;
struct Capsule2;
struct Triangle2;
struct Disc2;
// -----------------------------------------------------------------------------
void TransformVertexArrayXY3D(int numVerts, Vertex_PCU* verts, float uniformScaleXY, float rotationDegreesAboutZ, Vec2 const& translationXY);
void TransformVertexArray3D(std::vector<Vertex_PCU>& verts, Mat44 const& transform);
void TransformVertexArrayTBN3D(std::vector<Vertex_PCUTBN>& verts, Mat44 const& transform);
AABB2 GetVertexBounds(std::vector<Vertex_PCU> const& verts);
Vec2  GetDiscUV(Vec3 const& position, Vec3 const& center, Vec3 const& jBasis, Vec3 const& kBasis, float radius);
// -----------------------------------------------------------------------------
// 2D Verts utils
void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& discCenter, float discRadius, Rgba8 const& color);
void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& alignedBox, Rgba8 const& color, Vec2 const& uvMins = Vec2::ZERO, Vec2 const& uvMaxs = Vec2::ONE);
void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, OBB2 const& orientedBox, Rgba8 const& color);
void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Vec2 const& boneStart, Vec2 const& boneEnd, float radius, Rgba8 const& color);
void AddVertsForTriangle2D(std::vector<Vertex_PCU>& verts, Vec2 const& ccw0, Vec2 const& ccw1, Vec2 const& ccw2, Rgba8 const& color);
void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color);
void AddVertsForArrow2D(std::vector<Vertex_PCU>& verts, Vec2 tailPos, Vec2 tipPos, float arrowSize, float lineThickness, Rgba8 const& color);
void AddVertsForArc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, float startDegrees, float arcDegrees, Rgba8 const& color = Rgba8::WHITE, int numSegments = 32);
void AddVertsForDashedLine2D(std::vector<Vertex_PCU>& verts, Vec2 const& start, Vec2 const& end, float dashLength = 3.f, float gapLength = 1.f, float thickness = 1.f, Rgba8 const& color = Rgba8::WHITE);
// -----------------------------------------------------------------------------
// 3D Verts utils Vertex_PCU
void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight,
	Vec3 const& topLeft, Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2(Vec2::ZERO, Vec2::ONE));
void AddVertsForTriangle3D(std::vector<Vertex_PCU>& verts, Vec3 const& ccw0, Vec3 const& ccw1, Vec3 const& ccw2, Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForAABB3D(std::vector<Vertex_PCU>& verts, AABB3 const& bounds, Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForOBB3D(std::vector<Vertex_PCU>& verts, OBB3 const& box, Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForSphere3D(std::vector<Vertex_PCU>& verts, Vec3 const& center, float radius, Rgba8 const& color = Rgba8::WHITE,
	AABB2 const& UVs = AABB2(Vec2::ZERO, Vec2::ONE), int numSlices = 32, int numStacks = 16);
void AddVertsForCylinderZ3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, float radius, float height, Rgba8 color = Rgba8::WHITE, AABB2 UVs = AABB2::ZERO_TO_ONE, int numSlices = 32);
void AddVertsForCylinder3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, Rgba8 const& color = Rgba8::WHITE,
	AABB2 const& UVs = AABB2(Vec2::ZERO, Vec2::ONE), int numSlices = 8);
void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, Rgba8 const& color = Rgba8::WHITE,
	AABB2 const& UVs = AABB2(Vec2::ZERO, Vec2::ONE), int numSlices = 8);
void AddVertsForArrow3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, Rgba8 const& color = Rgba8::WHITE, int numSlices = 32);
void AddVertsForMathArrow3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, Rgba8 const& color = Rgba8::WHITE, int numSlices = 32);
void AddVertsForPyramidZ3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, float size, float height, Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForPyramid3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float size, float height, Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForRotatedAABB3D(std::vector<Vertex_PCU>& verts, AABB3 const& box, float degrees, Vec3 const& pivot, Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
// -----------------------------------------------------------------------------
// 3D Verts utils Vertex_PCUTBN
void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft,
	Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft,
	Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForRoundedQuad3D(std::vector<Vertex_PCUTBN>& vertexes, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft,
	Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForAABB3D(std::vector<Vertex_PCUTBN>& vertexes, std::vector<unsigned int>& indexes, AABB3 const& bounds,
	Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForAABB3D(std::vector<Vertex_PCUTBN>& vertexes, AABB3 const& bounds, Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForOBB3D(std::vector<Vertex_PCUTBN>& vertexes, std::vector<unsigned int>& indexes, OBB3 const& bounds,
	Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForSphere3D(std::vector<Vertex_PCUTBN>& vertexes, Vec3 const& center, float radius,
	Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVS = AABB2::ZERO_TO_ONE, int numSlices = 32, int numStacks = 16);
void AddVertsForSphere3D(std::vector<Vertex_PCUTBN>& vertexes, std::vector<unsigned int>& indexes, Vec3 const& center, float radius,
	Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVS = AABB2::ZERO_TO_ONE, int numSlices = 32, int numStacks = 16);
void AddVertsForCylinderZ3D(std::vector<Vertex_PCUTBN>& vertexes, std::vector<unsigned int>& indexes, Vec3 const& start, float radius, float height,
	Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE, int numSlices = 8);
void AddVertsForCylinder3D(std::vector<Vertex_PCUTBN>& vertexes, Vec3 const& start, Vec3 const& end, float radius,
	Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE, int numSlices = 8);
void AddVertsForCylinderOriented3D(std::vector<Vertex_PCUTBN>& vertexes, std::vector<unsigned int>& indexes, Vec3 const& start, Vec3 const& end, float radius, Rgba8 const& color, AABB2 const& UVs, int numSlices);
// -----------------------------------------------------------------------------
