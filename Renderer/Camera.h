#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.h"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/AABB2.h"
#include "Engine/Math/IntVec2.h"
// -----------------------------------------------------------------------------
class Camera 
{
public:
	enum Mode
	{
		eMode_OrthoGraphic,
		eMode_Perspective,
		eMode_Count
	};

	void SetOrthoView(Vec2 const& bottomLeft, Vec2 const& topRight, float near = 0.0f, float far = 1.0f);
	void SetPerspectiveView(float aspect, float fov, float near, float far);

	void SetPositionAndOrientation(Vec3 const& position, EulerAngles const& orientation);
	void SetPosition(Vec3 const& position);
	Vec3 GetPosition() const;
	void SetOrientation(EulerAngles const& orientation);
	EulerAngles GetOrientation() const;
	void SetNormalizedViewport(Vec2 const& normalizedBottomLeft, Vec2 const& normalizedTopRight);
	Vec2 GetClientToWorld(Vec2 const& clientPos, IntVec2 const& clientDims) const;

	Mat44 GetCameraToWorldTransform() const;
	Mat44 GetWorldToCameraTransform() const;

	void SetCameraToRenderTransform(Mat44 const& m);
	Mat44 GetCameraToRenderTransform() const;

	Mat44 GetRenderToClipTransform() const;

	Vec2 GetOrthoBottomLeft() const;
	Vec2 GetOrthoTopRight() const;
	AABB2 GetOrthoBounds() const;
	AABB2 GetNormalizedViewport() const;
	Vec2 Translate2D(const Vec2& translation2D);

	Mat44 GetOrthographicMatrix() const;
	Mat44 GetPerspectiveMatrix() const;
	Mat44 GetProjectionMatrix() const;

	AABB2 m_normalizedViewport = AABB2::ZERO_TO_ONE;
protected:
	Mode m_mode = eMode_OrthoGraphic;

	Vec3 m_position = Vec3::ZERO;
	EulerAngles m_orientation = EulerAngles::ZERO;

	Vec2 m_bottomLeft = Vec2::ZERO;
	Vec2 m_topRight = Vec2::ONE;
	float m_orthographicNear = 0.0f;
	float m_orthographicFar = 1.0f;

	float m_perspectiveAspect = 0.0f;
	float m_perspectiveFOV = 0.0f;
	float m_perspectiveNear = 0.0f;
	float m_perspectiveFar = 1.0f;

	Mat44 m_cameraToRenderTransform = Mat44();
};