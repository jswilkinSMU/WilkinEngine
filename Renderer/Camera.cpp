#include "Engine/Renderer/Camera.h"
#include "Engine/Math/MathUtils.h"
// -----------------------------------------------------------------------------
void Camera::SetOrthoView(Vec2 const& bottomLeft, Vec2 const& topRight, float near, float far)
{
	m_mode = eMode_OrthoGraphic;

	m_bottomLeft = bottomLeft;
	m_topRight = topRight;
	m_orthographicNear = near;
	m_orthographicFar = far;
}

void Camera::SetPerspectiveView(float aspect, float fov, float near, float far)
{
	m_mode = eMode_Perspective;

	m_perspectiveAspect = aspect;
	m_perspectiveFOV = fov;
	m_perspectiveNear = near;
	m_perspectiveFar = far;
}

void Camera::SetPositionAndOrientation(Vec3 const& position, EulerAngles const& orientation)
{
	m_position = position;
	m_orientation = orientation;
}

void Camera::SetPosition(Vec3 const& position)
{
	m_position = position;
}

Vec3 Camera::GetPosition() const
{
	return m_position;
}

void Camera::SetOrientation(EulerAngles const& orientation)
{
	m_orientation = orientation;
}

EulerAngles Camera::GetOrientation() const
{
	return m_orientation;
}

void Camera::SetNormalizedViewport(Vec2 const& normalizedBottomLeft, Vec2 const& normalizedTopRight)
{
	m_normalizedViewport.m_mins = normalizedBottomLeft;
	m_normalizedViewport.m_maxs = normalizedTopRight;
}

Vec2 Camera::GetClientToWorld(Vec2 const& clientPos, IntVec2 const& clientDims) const
{
	float normalizedX = clientPos.x / static_cast<float>(clientDims.x);
	float normalizedY = 1.f - (clientPos.y / static_cast<float>(clientDims.y));
	Vec2  viewMins = m_normalizedViewport.m_mins;
	Vec2  viewMaxs = m_normalizedViewport.m_maxs;

	normalizedX  = RangeMap(normalizedX, viewMins.x, viewMaxs.x, 0.f, 1.f);
	normalizedX  = RangeMap(normalizedX, viewMins.y, viewMaxs.y, 0.f, 1.f);
	float worldX = RangeMap(normalizedX, 0.f, 1.f, m_bottomLeft.x, m_topRight.x);
	float worldY = RangeMap(normalizedY, 0.f, 1.f, m_bottomLeft.y, m_topRight.y);

	return Vec2(worldX, worldY);
}

Mat44 Camera::GetCameraToWorldTransform() const
{
	Mat44 cameraToWorldTransform = Mat44::MakeTranslation3D(m_position);
	cameraToWorldTransform.Append(m_orientation.GetAsMatrix_IFwd_JLeft_KUp());
	return cameraToWorldTransform;
}

Mat44 Camera::GetWorldToCameraTransform() const
{
	return GetCameraToWorldTransform().GetOrthonormalInverse();
}

void Camera::SetCameraToRenderTransform(Mat44 const& m)
{
	m_cameraToRenderTransform = m;
}

Mat44 Camera::GetCameraToRenderTransform() const
{
	return m_cameraToRenderTransform;
}

Mat44 Camera::GetRenderToClipTransform() const
{
	return GetProjectionMatrix();
}

Vec2 Camera::GetOrthoBottomLeft() const
{
	return m_bottomLeft;
}

Vec2 Camera::GetOrthoTopRight() const
{
	return m_topRight;
}

AABB2 Camera::GetOrthoBounds() const
{
	return AABB2(m_bottomLeft, m_topRight);
}

AABB2 Camera::GetNormalizedViewport() const
{
	return m_normalizedViewport;
}

Vec2 Camera::Translate2D(const Vec2& translation2D)
{
	m_bottomLeft += translation2D;
	m_topRight += translation2D;
	return Vec2(m_bottomLeft.x, m_topRight.y);
}

Mat44 Camera::GetOrthographicMatrix() const
{
	return Mat44::MakeOrthoProjection(m_bottomLeft.x, m_topRight.x, m_bottomLeft.y, m_topRight.y, m_orthographicNear, m_orthographicFar);
}

Mat44 Camera::GetPerspectiveMatrix() const
{
	return Mat44::MakePerspectiveProjection(m_perspectiveFOV, m_perspectiveAspect, m_perspectiveNear, m_perspectiveFar);
}

Mat44 Camera::GetProjectionMatrix() const
{
	Mat44 projectionMatrix;

	if (m_mode == eMode_OrthoGraphic)
	{
		projectionMatrix = GetOrthographicMatrix();
	}
	else if (m_mode == eMode_Perspective)
	{
		projectionMatrix = GetPerspectiveMatrix();
	}
	else
	{
		projectionMatrix = Mat44();
	}

	return projectionMatrix;
}