#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/MathUtils.h"
#include "Engine/Core/EngineCommon.h"

Mat44::Mat44()
{
	m_values[Ix] = 1.f; m_values[Jx] = 0.f; m_values[Kx] = 0.f; m_values[Tx] = 0.f;
	m_values[Iy] = 0.f; m_values[Jy] = 1.f; m_values[Ky] = 0.f; m_values[Ty] = 0.f;
	m_values[Iz] = 0.f; m_values[Jz] = 0.f; m_values[Kz] = 1.f; m_values[Tz] = 0.f;
	m_values[Iw] = 0.f; m_values[Jw] = 0.f; m_values[Kw] = 0.f; m_values[Tw] = 1.f;
}

Mat44::Mat44(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translation2D)
{
	m_values[Ix] = iBasis2D.x; m_values[Jx] = jBasis2D.x; m_values[Kx] = 0.f;	m_values[Tx] = translation2D.x;
	m_values[Iy] = iBasis2D.y; m_values[Jy] = jBasis2D.y; m_values[Ky] = 0.f;	m_values[Ty] = translation2D.y;
	m_values[Iz] = 0.f;		   m_values[Jz] = 0.f;		  m_values[Kz] = 1.f;	m_values[Tz] = 0.f;
	m_values[Iw] = 0.f;		   m_values[Jw] = 0.f;		  m_values[Kw] = 0.f;	m_values[Tw] = 1.f;
}

Mat44::Mat44(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translation3D)
{
	m_values[Ix] = iBasis3D.x; m_values[Jx] = jBasis3D.x; m_values[Kx] = kBasis3D.x;	m_values[Tx] = translation3D.x;
	m_values[Iy] = iBasis3D.y; m_values[Jy] = jBasis3D.y; m_values[Ky] = kBasis3D.y;	m_values[Ty] = translation3D.y;
	m_values[Iz] = iBasis3D.z; m_values[Jz] = jBasis3D.z; m_values[Kz] = kBasis3D.z;	m_values[Tz] = translation3D.z;
	m_values[Iw] = 0.f;		   m_values[Jw] = 0.f;		  m_values[Kw] = 0.f;			m_values[Tw] = 1.f;
}

Mat44::Mat44(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D)
{
	m_values[Ix] = iBasis4D.x; m_values[Jx] = jBasis4D.x; m_values[Kx] = kBasis4D.x;	m_values[Tx] = translation4D.x;
	m_values[Iy] = iBasis4D.y; m_values[Jy] = jBasis4D.y; m_values[Ky] = kBasis4D.y;	m_values[Ty] = translation4D.y;
	m_values[Iz] = iBasis4D.z; m_values[Jz] = jBasis4D.z; m_values[Kz] = kBasis4D.z;	m_values[Tz] = translation4D.z;
	m_values[Iw] = iBasis4D.w; m_values[Jw] = jBasis4D.w; m_values[Kw] = kBasis4D.w;	m_values[Tw] = translation4D.w;
}

Mat44::Mat44(float const* sixteenValuesBasisMajor)
{
	for (int matrixValueIndex = 0; matrixValueIndex < 16; ++matrixValueIndex)
	{
		m_values[matrixValueIndex] = sixteenValuesBasisMajor[matrixValueIndex];
	}
}

Mat44 Mat44::operator*(Mat44 const& matToMultiply) const
{
	Mat44 matrix;

	for (int row = 0; row < 4; ++row)
	{
		for (int column = 0; column < 4; ++column)
		{
			float m1 = m_values[0 * 4 + row] * matToMultiply.m_values[column * 4 + 0];
			float m2 = m_values[1 * 4 + row] * matToMultiply.m_values[column * 4 + 1];
			float m3 = m_values[2 * 4 + row] * matToMultiply.m_values[column * 4 + 2];
			float m4 = m_values[3 * 4 + row] * matToMultiply.m_values[column * 4 + 3];

			matrix.m_values[column * 4 + row] = m1 + m2 + m3 + m4;
		}
	}

	return matrix;
}

// -----------------------------------------------------------------------------

Mat44 const Mat44::MakeTranslation2D(Vec2 const& translationXY)
{
	Mat44 translation2DMatrix;

	translation2DMatrix.m_values[Ix] = 1.f; translation2DMatrix.m_values[Jx] = 0.f; translation2DMatrix.m_values[Kx] = 0.f; translation2DMatrix.m_values[Tx] = translationXY.x;
	translation2DMatrix.m_values[Iy] = 0.f; translation2DMatrix.m_values[Jy] = 1.f; translation2DMatrix.m_values[Ky] = 0.f; translation2DMatrix.m_values[Ty] = translationXY.y;
	translation2DMatrix.m_values[Iz] = 0.f; translation2DMatrix.m_values[Jz] = 0.f; translation2DMatrix.m_values[Kz] = 1.f; translation2DMatrix.m_values[Tz] = 0.f;
	translation2DMatrix.m_values[Iw] = 0.f; translation2DMatrix.m_values[Jw] = 0.f; translation2DMatrix.m_values[Kw] = 0.f; translation2DMatrix.m_values[Tw] = 1.f;

	return translation2DMatrix;
}

Mat44 const Mat44::MakeTranslation3D(Vec3 const& translationXYZ)
{
	Mat44 translation3DMatrix;

	translation3DMatrix.m_values[Ix] = 1.f; translation3DMatrix.m_values[Jx] = 0.f; translation3DMatrix.m_values[Kx] = 0.f; translation3DMatrix.m_values[Tx] = translationXYZ.x;
	translation3DMatrix.m_values[Iy] = 0.f; translation3DMatrix.m_values[Jy] = 1.f; translation3DMatrix.m_values[Ky] = 0.f; translation3DMatrix.m_values[Ty] = translationXYZ.y;
	translation3DMatrix.m_values[Iz] = 0.f; translation3DMatrix.m_values[Jz] = 0.f; translation3DMatrix.m_values[Kz] = 1.f; translation3DMatrix.m_values[Tz] = translationXYZ.z;
	translation3DMatrix.m_values[Iw] = 0.f; translation3DMatrix.m_values[Jw] = 0.f; translation3DMatrix.m_values[Kw] = 0.f; translation3DMatrix.m_values[Tw] = 1.f;

	return translation3DMatrix;
}

Mat44 const Mat44::MakeUniformScale2D(float uniformScaleXY)
{
	Mat44 uniformScale2DMatrix;

	uniformScale2DMatrix.m_values[Ix] = uniformScaleXY; uniformScale2DMatrix.m_values[Jx] = 0.f;			uniformScale2DMatrix.m_values[Kx] = 0.f; uniformScale2DMatrix.m_values[Tx] = 0.f;
	uniformScale2DMatrix.m_values[Iy] = 0.f;			uniformScale2DMatrix.m_values[Jy] = uniformScaleXY; uniformScale2DMatrix.m_values[Ky] = 0.f; uniformScale2DMatrix.m_values[Ty] = 0.f;
	uniformScale2DMatrix.m_values[Iz] = 0.f;			uniformScale2DMatrix.m_values[Jz] = 0.f;			uniformScale2DMatrix.m_values[Kz] = 1.f; uniformScale2DMatrix.m_values[Tz] = 0.f;
	uniformScale2DMatrix.m_values[Iw] = 0.f;			uniformScale2DMatrix.m_values[Jw] = 0.f;			uniformScale2DMatrix.m_values[Kw] = 0.f; uniformScale2DMatrix.m_values[Tw] = 1.f;

	return uniformScale2DMatrix;
}

Mat44 const Mat44::MakeUniformScale3D(float uniformScaleXYZ)
{
	Mat44 uniformScale3DMatrix;

	uniformScale3DMatrix.m_values[Ix] = uniformScaleXYZ; uniformScale3DMatrix.m_values[Jx] = 0.f;			  uniformScale3DMatrix.m_values[Kx] = 0.f;				uniformScale3DMatrix.m_values[Tx] = 0.f;
	uniformScale3DMatrix.m_values[Iy] = 0.f;			 uniformScale3DMatrix.m_values[Jy] = uniformScaleXYZ; uniformScale3DMatrix.m_values[Ky] = 0.f;				uniformScale3DMatrix.m_values[Ty] = 0.f;
	uniformScale3DMatrix.m_values[Iz] = 0.f;			 uniformScale3DMatrix.m_values[Jz] = 0.f;			  uniformScale3DMatrix.m_values[Kz] = uniformScaleXYZ;  uniformScale3DMatrix.m_values[Tz] = 0.f;
	uniformScale3DMatrix.m_values[Iw] = 0.f;			 uniformScale3DMatrix.m_values[Jw] = 0.f;			  uniformScale3DMatrix.m_values[Kw] = 0.f;				uniformScale3DMatrix.m_values[Tw] = 1.f;

	return uniformScale3DMatrix;
}

Mat44 const Mat44::MakeNonUniformScale2D(Vec2 const& nonUniformScaleXY)
{
	Mat44 nonUniformScale2DMatrix;

	nonUniformScale2DMatrix.m_values[Ix] = nonUniformScaleXY.x; nonUniformScale2DMatrix.m_values[Jx] = 0.f;					nonUniformScale2DMatrix.m_values[Kx] = 0.f; nonUniformScale2DMatrix.m_values[Tx] = 0.f;
	nonUniformScale2DMatrix.m_values[Iy] = 0.f;					nonUniformScale2DMatrix.m_values[Jy] = nonUniformScaleXY.y; nonUniformScale2DMatrix.m_values[Ky] = 0.f; nonUniformScale2DMatrix.m_values[Ty] = 0.f;
	nonUniformScale2DMatrix.m_values[Iz] = 0.f;					nonUniformScale2DMatrix.m_values[Jz] = 0.f;					nonUniformScale2DMatrix.m_values[Kz] = 1.f; nonUniformScale2DMatrix.m_values[Tz] = 0.f;
	nonUniformScale2DMatrix.m_values[Iw] = 0.f;					nonUniformScale2DMatrix.m_values[Jw] = 0.f;					nonUniformScale2DMatrix.m_values[Kw] = 0.f; nonUniformScale2DMatrix.m_values[Tw] = 1.f;

	return nonUniformScale2DMatrix;
}

Mat44 const Mat44::MakeNonUniformScale3D(Vec3 const& nonUniformScaleXYZ)
{
	Mat44 nonUniformScale3DMatrix;

	nonUniformScale3DMatrix.m_values[Ix] = nonUniformScaleXYZ.x; nonUniformScale3DMatrix.m_values[Jx] = 0.f;					nonUniformScale3DMatrix.m_values[Kx] = 0.f;						nonUniformScale3DMatrix.m_values[Tx] = 0.f;
	nonUniformScale3DMatrix.m_values[Iy] = 0.f;					 nonUniformScale3DMatrix.m_values[Jy] = nonUniformScaleXYZ.y;	nonUniformScale3DMatrix.m_values[Ky] = 0.f;						nonUniformScale3DMatrix.m_values[Ty] = 0.f;
	nonUniformScale3DMatrix.m_values[Iz] = 0.f;					 nonUniformScale3DMatrix.m_values[Jz] = 0.f;					nonUniformScale3DMatrix.m_values[Kz] = nonUniformScaleXYZ.z;	nonUniformScale3DMatrix.m_values[Tz] = 0.f;
	nonUniformScale3DMatrix.m_values[Iw] = 0.f;					 nonUniformScale3DMatrix.m_values[Jw] = 0.f;					nonUniformScale3DMatrix.m_values[Kw] = 0.f;						nonUniformScale3DMatrix.m_values[Tw] = 1.f;

	return nonUniformScale3DMatrix;
}

Mat44 const Mat44::MakeZRotationDegrees(float rotationDegreesAboutZ)
{
	Mat44 rotateAboutZMatrix;

	rotateAboutZMatrix.m_values[Ix] = CosDegrees(rotationDegreesAboutZ);  rotateAboutZMatrix.m_values[Jx] = SinDegrees(-rotationDegreesAboutZ);  rotateAboutZMatrix.m_values[Kx] = 0.f;	rotateAboutZMatrix.m_values[Tx] = 0.f;
	rotateAboutZMatrix.m_values[Iy] = SinDegrees(rotationDegreesAboutZ);  rotateAboutZMatrix.m_values[Jy] = CosDegrees(rotationDegreesAboutZ);   rotateAboutZMatrix.m_values[Ky] = 0.f;	rotateAboutZMatrix.m_values[Ty] = 0.f;
	rotateAboutZMatrix.m_values[Iz] = 0.f;								  rotateAboutZMatrix.m_values[Jz] = 0.f;								 rotateAboutZMatrix.m_values[Kz] = 1.f;	rotateAboutZMatrix.m_values[Tz] = 0.f;
	rotateAboutZMatrix.m_values[Iw] = 0.f;								  rotateAboutZMatrix.m_values[Jw] = 0.f;								 rotateAboutZMatrix.m_values[Kw] = 0.f;	rotateAboutZMatrix.m_values[Tw] = 1.f;

	return rotateAboutZMatrix;
}

Mat44 const Mat44::MakeYRotationDegrees(float rotationDegreesAboutY)
{
	Mat44 rotateAboutYMatrix;

	rotateAboutYMatrix.m_values[Ix] = CosDegrees(rotationDegreesAboutY);  rotateAboutYMatrix.m_values[Jx] = 0.f;   rotateAboutYMatrix.m_values[Kx] = SinDegrees(rotationDegreesAboutY);	rotateAboutYMatrix.m_values[Tx] = 0.f;
	rotateAboutYMatrix.m_values[Iy] = 0.f;								  rotateAboutYMatrix.m_values[Jy] = 1.f;   rotateAboutYMatrix.m_values[Ky] = 0.f;									rotateAboutYMatrix.m_values[Ty] = 0.f;
	rotateAboutYMatrix.m_values[Iz] = SinDegrees(-rotationDegreesAboutY); rotateAboutYMatrix.m_values[Jz] = 0.f;   rotateAboutYMatrix.m_values[Kz] = CosDegrees(rotationDegreesAboutY);		rotateAboutYMatrix.m_values[Tz] = 0.f;
	rotateAboutYMatrix.m_values[Iw] = 0.f;								  rotateAboutYMatrix.m_values[Jw] = 0.f;   rotateAboutYMatrix.m_values[Kw] = 0.f;									rotateAboutYMatrix.m_values[Tw] = 1.f;

	return rotateAboutYMatrix;
}

Mat44 const Mat44::MakeXRotationDegrees(float rotationDegreesAboutX)
{
	Mat44 rotateAboutXMatrix;

	rotateAboutXMatrix.m_values[Ix] = 1.f;  rotateAboutXMatrix.m_values[Jx] = 0.f;									rotateAboutXMatrix.m_values[Kx] = 0.f;									rotateAboutXMatrix.m_values[Tx] = 0.f;
	rotateAboutXMatrix.m_values[Iy] = 0.f;	rotateAboutXMatrix.m_values[Jy] = CosDegrees(rotationDegreesAboutX);    rotateAboutXMatrix.m_values[Ky] = SinDegrees(-rotationDegreesAboutX);	rotateAboutXMatrix.m_values[Ty] = 0.f;
	rotateAboutXMatrix.m_values[Iz] = 0.f;  rotateAboutXMatrix.m_values[Jz] = SinDegrees(rotationDegreesAboutX);    rotateAboutXMatrix.m_values[Kz] = CosDegrees(rotationDegreesAboutX);	rotateAboutXMatrix.m_values[Tz] = 0.f;
	rotateAboutXMatrix.m_values[Iw] = 0.f;	rotateAboutXMatrix.m_values[Jw] = 0.f;									rotateAboutXMatrix.m_values[Kw] = 0.f;									rotateAboutXMatrix.m_values[Tw] = 1.f;

	return rotateAboutXMatrix;
}

Mat44 const Mat44::MakeAxisRotationDegrees(Vec3 const& axis, float rotationDegreesAboutAxis)
{
	Vec3  axisNormal = axis.GetNormalized();
	float axisX = axisNormal.x;
	float axisY = axisNormal.y;
	float axisZ = axisNormal.z;
	float cosAxis = CosDegrees(rotationDegreesAboutAxis);
	float sinAxis = SinDegrees(rotationDegreesAboutAxis);
	float negCosAxis = 1.f - cosAxis;

	Mat44 rotateAboutAxis;

	rotateAboutAxis.m_values[Ix] = negCosAxis * axisX * axisX + cosAxis;
	rotateAboutAxis.m_values[Iy] = negCosAxis * axisX * axisY + sinAxis * axisZ;
	rotateAboutAxis.m_values[Iz] = negCosAxis * axisX * axisZ - sinAxis * axisY;
	rotateAboutAxis.m_values[Iw] = 0.f;

	rotateAboutAxis.m_values[Jx] = negCosAxis * axisX * axisY - sinAxis * axisZ;
	rotateAboutAxis.m_values[Jy] = negCosAxis * axisY * axisY + cosAxis;
	rotateAboutAxis.m_values[Jz] = negCosAxis * axisY * axisZ + sinAxis * axisX;
	rotateAboutAxis.m_values[Jw] = 0.f;

	rotateAboutAxis.m_values[Kx] = negCosAxis * axisX * axisZ + sinAxis * axisY;
	rotateAboutAxis.m_values[Ky] = negCosAxis * axisY * axisZ - sinAxis * axisX;
	rotateAboutAxis.m_values[Kz] = negCosAxis * axisZ * axisZ + cosAxis;
	rotateAboutAxis.m_values[Kw] = 0.f;

	rotateAboutAxis.m_values[Tx] = 0.f;
	rotateAboutAxis.m_values[Ty] = 0.f;
	rotateAboutAxis.m_values[Tz] = 0.f;
	rotateAboutAxis.m_values[Tw] = 1.f;

	return rotateAboutAxis;
}

Mat44 const Mat44::MakeOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Mat44 orthoProjection;
	orthoProjection.m_values[Ix] = 2.f / (right - left);
	orthoProjection.m_values[Jy] = 2.f / (top - bottom);
	orthoProjection.m_values[Kz] = 1.f / (zFar - zNear);
	orthoProjection.m_values[Tx] = (left + right) / (left - right);
	orthoProjection.m_values[Ty] = (bottom + top) / (bottom - top);
	orthoProjection.m_values[Tz] = zNear / (zNear - zFar);
	return orthoProjection;
}

Mat44 const Mat44::MakePerspectiveProjection(float fovYDegrees, float aspect, float zNear, float zFar)
{
	Mat44 perspectiveMatrix;

	float c = CosDegrees(fovYDegrees * 0.5f);
	float s = SinDegrees(fovYDegrees * 0.5f);
	float scaleY = c / s;
	float scaleX = scaleY / aspect;
	float scaleZ = zFar / (zFar - zNear);
	float translateZ = (zNear * zFar) / (zNear - zFar);

	perspectiveMatrix.m_values[Ix] = scaleX;
	perspectiveMatrix.m_values[Jy] = scaleY;
	perspectiveMatrix.m_values[Kz] = scaleZ;
	perspectiveMatrix.m_values[Kw] = 1.f;
	perspectiveMatrix.m_values[Tz] = translateZ;
	perspectiveMatrix.m_values[Tw] = 0.f;

	return perspectiveMatrix;
}

Mat44 const Mat44::MakeTransform(Vec3 const& position, Quat const& rotation, Vec3 const& scale)
{
	Mat44 rotationMatrix = rotation.GetAsMatrix_IFwd_JLeft_KUp();

	Vec3  iBasis = rotationMatrix.GetIBasis3D() * scale.x;
	Vec3  jBasis = rotationMatrix.GetJBasis3D() * scale.y;
	Vec3  kBasis = rotationMatrix.GetKBasis3D() * scale.z;

	Mat44 transformedMatrix;
	transformedMatrix.SetIJKT3D(iBasis, jBasis, kBasis, position);

	return transformedMatrix;
}

// -----------------------------------------------------------------------------

Vec2 const Mat44::TransformVectorQuantity2D(Vec2 const& vectorQuantityXY) const
{
	Vec2 const& p = vectorQuantityXY;
	float x = (m_values[Ix] * p.x) + (m_values[Jx] * p.y); 
	float y = (m_values[Iy] * p.x) + (m_values[Jy] * p.y);
	return Vec2(x, y);
}

Vec3 const Mat44::TransformVectorQuantity3D(Vec3 const& vectorQuantityXYZ) const
{
	Vec3 const& p = vectorQuantityXYZ;
	float x = (m_values[Ix] * p.x) + (m_values[Jx] * p.y) + (m_values[Kx] * p.z);
	float y = (m_values[Iy] * p.x) + (m_values[Jy] * p.y) + (m_values[Ky] * p.z);
	float z = (m_values[Iz] * p.x) + (m_values[Jz] * p.y) + (m_values[Kz] * p.z);
	return Vec3(x, y, z);
}

Vec2 const Mat44::TransformPosition2D(Vec2 const& positionXY) const
{
	Vec2 const& p = positionXY;
	float x = (m_values[Ix] * p.x) + (m_values[Jx] * p.y) + m_values[Tx];
	float y = (m_values[Iy] * p.x) + (m_values[Jy] * p.y) + m_values[Ty];
	return Vec2(x, y);
}

Vec3 const Mat44::TransformPosition3D(Vec3 const& position3D) const
{
	Vec3 const& p = position3D;
	float x = (m_values[Ix] * p.x) + (m_values[Jx] * p.y) + (m_values[Kx] * p.z) + m_values[Tx];
	float y = (m_values[Iy] * p.x) + (m_values[Jy] * p.y) + (m_values[Ky] * p.z) + m_values[Ty];
	float z = (m_values[Iz] * p.x) + (m_values[Jz] * p.y) + (m_values[Kz] * p.z) + m_values[Tz];
	return Vec3(x, y, z);
}

Vec4 const Mat44::TransformHomogeneous3D(Vec4 const& homogenousPoint3D) const
{
	Vec4 const& p = homogenousPoint3D;
	float x = (m_values[Ix] * p.x) + (m_values[Jx] * p.y) + (m_values[Kx] * p.z) + (m_values[Tx] * p.w);
	float y = (m_values[Iy] * p.x) + (m_values[Jy] * p.y) + (m_values[Ky] * p.z) + (m_values[Ty] * p.w);
	float z = (m_values[Iz] * p.x) + (m_values[Jz] * p.y) + (m_values[Kz] * p.z) + (m_values[Tz] * p.w);
	float w = (m_values[Iw] * p.x) + (m_values[Jw] * p.y) + (m_values[Kw] * p.z) + (m_values[Tw] * p.w);
	return Vec4(x, y, z, w);
}

// -----------------------------------------------------------------------------

float* Mat44::GetAsFloatArray()
{
	return m_values;
}

float const* Mat44::GetAsFloatArray() const
{
	return m_values;
}

Vec2 const Mat44::GetIBasis2D() const
{
	return Vec2(m_values[Ix], m_values[Iy]);
}

Vec2 const Mat44::GetJBasis2D() const
{
	return Vec2(m_values[Jx], m_values[Jy]);
}

Vec2 const Mat44::GetTranslation2D() const
{
	return Vec2(m_values[Tx], m_values[Ty]);
}

Vec3 const Mat44::GetIBasis3D() const
{
	return Vec3(m_values[Ix], m_values[Iy], m_values[Iz]);
}

Vec3 const Mat44::GetJBasis3D() const
{
	return Vec3(m_values[Jx], m_values[Jy], m_values[Jz]);
}

Vec3 const Mat44::GetKBasis3D() const
{
	return Vec3(m_values[Kx], m_values[Ky], m_values[Kz]);
}

Vec3 const Mat44::GetTranslation3D() const
{
	return Vec3(m_values[Tx], m_values[Ty], m_values[Tz]);
}

Vec4 const Mat44::GetIBasis4D() const
{
	return Vec4(m_values[Ix], m_values[Iy], m_values[Iz], m_values[Iw]);
}

Vec4 const Mat44::GetJBasis4D() const
{
	return Vec4(m_values[Jx], m_values[Jy], m_values[Jz], m_values[Jw]);
}

Vec4 const Mat44::GetKBasis4D() const
{
	return Vec4(m_values[Kx], m_values[Ky], m_values[Kz], m_values[Kw]);
}

Vec4 const Mat44::GetTranslation4D() const
{
	return Vec4(m_values[Tx], m_values[Ty], m_values[Tz], m_values[Tw]);
}

Mat44 const Mat44::GetOrthonormalInverse() const
{
	Mat44 inverseMatrix;

	inverseMatrix.m_values[Ix] = m_values[Ix];
	inverseMatrix.m_values[Iy] = m_values[Jx];
	inverseMatrix.m_values[Iz] = m_values[Kx];

	inverseMatrix.m_values[Jx] = m_values[Iy];
	inverseMatrix.m_values[Jy] = m_values[Jy];
	inverseMatrix.m_values[Jz] = m_values[Ky];

	inverseMatrix.m_values[Kx] = m_values[Iz];
	inverseMatrix.m_values[Ky] = m_values[Jz];
	inverseMatrix.m_values[Kz] = m_values[Kz];

	inverseMatrix.m_values[Tx] = -DotProduct3D(Vec3(m_values[Ix], m_values[Iy], m_values[Iz]), Vec3(m_values[Tx], m_values[Ty], m_values[Tz]));
	inverseMatrix.m_values[Ty] = -DotProduct3D(Vec3(m_values[Jx], m_values[Jy], m_values[Jz]), Vec3(m_values[Tx], m_values[Ty], m_values[Tz]));
	inverseMatrix.m_values[Tz] = -DotProduct3D(Vec3(m_values[Kx], m_values[Ky], m_values[Kz]), Vec3(m_values[Tx], m_values[Ty], m_values[Tz]));

	return inverseMatrix;
}

// -----------------------------------------------------------------------------

void Mat44::SetTranslation2D(Vec2 const& translationXY)
{
	m_values[Tx] = translationXY.x;
	m_values[Ty] = translationXY.y;
	m_values[Tz] = 0.f;
	m_values[Tw] = 1.f;
}

void Mat44::SetTranslation3D(Vec3 const& translationXYZ)
{
	m_values[Tx] = translationXYZ.x;
	m_values[Ty] = translationXYZ.y;
	m_values[Tz] = translationXYZ.z;
	m_values[Tw] = 1.f;
}

void Mat44::SetIJ2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Iz] = 0.f;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0.f;
	m_values[Jw] = 0.f;
}

void Mat44::SetIJT2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translationXY)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Iz] = 0.f;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0.f;
	m_values[Jw] = 0.f;

	m_values[Tx] = translationXY.x;
	m_values[Ty] = translationXY.y;
	m_values[Tz] = 0.f;
	m_values[Tw] = 1.f;
}

void Mat44::SetIJK3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0.f;

	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;
	m_values[Kw] = 0.f;
}

void Mat44::SetIJKT3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translationXYZ)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0.f;

	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;
	m_values[Kw] = 0.f;

	m_values[Tx] = translationXYZ.x;
	m_values[Ty] = translationXYZ.y;
	m_values[Tz] = translationXYZ.z;
	m_values[Tw] = 1.f;
}

void Mat44::SetIJKT4D(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D)
{
	m_values[Ix] = iBasis4D.x;
	m_values[Iy] = iBasis4D.y;
	m_values[Iz] = iBasis4D.z;
	m_values[Iw] = iBasis4D.w;

	m_values[Jx] = jBasis4D.x;
	m_values[Jy] = jBasis4D.y;
	m_values[Jz] = jBasis4D.z;
	m_values[Jw] = jBasis4D.w;

	m_values[Kx] = kBasis4D.x;
	m_values[Ky] = kBasis4D.y;
	m_values[Kz] = kBasis4D.z;
	m_values[Kw] = kBasis4D.w;

	m_values[Tx] = translation4D.x;
	m_values[Ty] = translation4D.y;
	m_values[Tz] = translation4D.z;
	m_values[Tw] = translation4D.w;
}

void Mat44::Transpose()
{
	Mat44 transposedMatrix;

	transposedMatrix.m_values[Ix] = m_values[Ix];
	transposedMatrix.m_values[Iy] = m_values[Jx];
	transposedMatrix.m_values[Iz] = m_values[Kx];
	transposedMatrix.m_values[Iw] = m_values[Tx];

	transposedMatrix.m_values[Jx] = m_values[Iy];
	transposedMatrix.m_values[Jy] = m_values[Jy];
	transposedMatrix.m_values[Jz] = m_values[Ky];
	transposedMatrix.m_values[Jw] = m_values[Ty];

	transposedMatrix.m_values[Kx] = m_values[Iz];
	transposedMatrix.m_values[Ky] = m_values[Jz];
	transposedMatrix.m_values[Kz] = m_values[Kz];
	transposedMatrix.m_values[Kw] = m_values[Tz];

	transposedMatrix.m_values[Tx] = m_values[Iw];
	transposedMatrix.m_values[Ty] = m_values[Jw];
	transposedMatrix.m_values[Tz] = m_values[Kw];
	transposedMatrix.m_values[Tw] = m_values[Tw];

	*this = transposedMatrix;
}

void Mat44::Orthonormalize_IFwd_JLeft_KUp()
{
	Vec3 iBasis = GetIBasis3D();
	Vec3 jBasis = GetJBasis3D();
	Vec3 kBasis = GetKBasis3D();

	iBasis.Normalize();

	jBasis = jBasis - (DotProduct3D(iBasis, jBasis) * iBasis);
	jBasis.Normalize();

	kBasis = kBasis - (DotProduct3D(iBasis, kBasis) * iBasis);
	kBasis = kBasis - (DotProduct3D(jBasis, kBasis) * jBasis);
	kBasis.Normalize();

	SetIJK3D(iBasis, jBasis, kBasis);
}

// -----------------------------------------------------------------------------

void Mat44::Append(Mat44 const& appendThis)
{
	Mat44 copyOfMatrix;
	// x values
	copyOfMatrix.m_values[Ix] = (m_values[Ix] * appendThis.m_values[Ix]) + (m_values[Jx] * appendThis.m_values[Iy]) + (m_values[Kx] * appendThis.m_values[Iz]) + (m_values[Tx] * appendThis.m_values[Iw]);
	copyOfMatrix.m_values[Jx] = (m_values[Ix] * appendThis.m_values[Jx]) + (m_values[Jx] * appendThis.m_values[Jy]) + (m_values[Kx] * appendThis.m_values[Jz]) + (m_values[Tx] * appendThis.m_values[Jw]);
	copyOfMatrix.m_values[Kx] = (m_values[Ix] * appendThis.m_values[Kx]) + (m_values[Jx] * appendThis.m_values[Ky]) + (m_values[Kx] * appendThis.m_values[Kz]) + (m_values[Tx] * appendThis.m_values[Kw]);
	copyOfMatrix.m_values[Tx] = (m_values[Ix] * appendThis.m_values[Tx]) + (m_values[Jx] * appendThis.m_values[Ty]) + (m_values[Kx] * appendThis.m_values[Tz]) + (m_values[Tx] * appendThis.m_values[Tw]);

	// y values
	copyOfMatrix.m_values[Iy] = (m_values[Iy] * appendThis.m_values[Ix]) + (m_values[Jy] * appendThis.m_values[Iy]) + (m_values[Ky] * appendThis.m_values[Iz]) + (m_values[Ty] * appendThis.m_values[Iw]);
	copyOfMatrix.m_values[Jy] = (m_values[Iy] * appendThis.m_values[Jx]) + (m_values[Jy] * appendThis.m_values[Jy]) + (m_values[Ky] * appendThis.m_values[Jz]) + (m_values[Ty] * appendThis.m_values[Jw]);
	copyOfMatrix.m_values[Ky] = (m_values[Iy] * appendThis.m_values[Kx]) + (m_values[Jy] * appendThis.m_values[Ky]) + (m_values[Ky] * appendThis.m_values[Kz]) + (m_values[Ty] * appendThis.m_values[Kw]);
	copyOfMatrix.m_values[Ty] = (m_values[Iy] * appendThis.m_values[Tx]) + (m_values[Jy] * appendThis.m_values[Ty]) + (m_values[Ky] * appendThis.m_values[Tz]) + (m_values[Ty] * appendThis.m_values[Tw]);

	// z values
	copyOfMatrix.m_values[Iz] = (m_values[Iz] * appendThis.m_values[Ix]) + (m_values[Jz] * appendThis.m_values[Iy]) + (m_values[Kz] * appendThis.m_values[Iz]) + (m_values[Tz] * appendThis.m_values[Iw]);
	copyOfMatrix.m_values[Jz] = (m_values[Iz] * appendThis.m_values[Jx]) + (m_values[Jz] * appendThis.m_values[Jy]) + (m_values[Kz] * appendThis.m_values[Jz]) + (m_values[Tz] * appendThis.m_values[Jw]);
	copyOfMatrix.m_values[Kz] = (m_values[Iz] * appendThis.m_values[Kx]) + (m_values[Jz] * appendThis.m_values[Ky]) + (m_values[Kz] * appendThis.m_values[Kz]) + (m_values[Tz] * appendThis.m_values[Kw]);
	copyOfMatrix.m_values[Tz] = (m_values[Iz] * appendThis.m_values[Tx]) + (m_values[Jz] * appendThis.m_values[Ty]) + (m_values[Kz] * appendThis.m_values[Tz]) + (m_values[Tz] * appendThis.m_values[Tw]);

	// w values
	copyOfMatrix.m_values[Iw] = (m_values[Iw] * appendThis.m_values[Ix]) + (m_values[Jw] * appendThis.m_values[Iy]) + (m_values[Kw] * appendThis.m_values[Iz]) + (m_values[Tw] * appendThis.m_values[Iw]);
	copyOfMatrix.m_values[Jw] = (m_values[Iw] * appendThis.m_values[Jx]) + (m_values[Jw] * appendThis.m_values[Jy]) + (m_values[Kw] * appendThis.m_values[Jz]) + (m_values[Tw] * appendThis.m_values[Jw]);
	copyOfMatrix.m_values[Kw] = (m_values[Iw] * appendThis.m_values[Kx]) + (m_values[Jw] * appendThis.m_values[Ky]) + (m_values[Kw] * appendThis.m_values[Kz]) + (m_values[Tw] * appendThis.m_values[Kw]);
	copyOfMatrix.m_values[Tw] = (m_values[Iw] * appendThis.m_values[Tx]) + (m_values[Jw] * appendThis.m_values[Ty]) + (m_values[Kw] * appendThis.m_values[Tz]) + (m_values[Tw] * appendThis.m_values[Tw]);

	*this = copyOfMatrix;
}

void Mat44::AppendZRotation(float degreesRotationAboutZ)
{
	Mat44 rotateAboutZMatrix = MakeZRotationDegrees(degreesRotationAboutZ);
	Append(rotateAboutZMatrix);
}

void Mat44::AppendYRotation(float degreesRotationAboutY)
{
	Mat44 rotateAboutYMatrix = MakeYRotationDegrees(degreesRotationAboutY);
	Append(rotateAboutYMatrix);
}

void Mat44::AppendXRotation(float degreesRotationAboutX)
{
	Mat44 rotateAboutXMatrix = MakeXRotationDegrees(degreesRotationAboutX);
	Append(rotateAboutXMatrix);
}

void Mat44::AppendTranslation2D(Vec2 const& translationXY)
{
	Mat44 translation2DMatrix = MakeTranslation2D(translationXY);
	Append(translation2DMatrix);
}

void Mat44::AppendTranslation3D(Vec3 const& translationXYZ)
{
	Mat44 translation3DMatrix = MakeTranslation3D(translationXYZ);
	Append(translation3DMatrix);
}

void Mat44::AppendScaleUniform2D(float uniformScaleXY)
{
	Mat44 uniformScale2DMatrix = MakeUniformScale2D(uniformScaleXY);
	Append(uniformScale2DMatrix);
}

void Mat44::AppendScaleUniform3D(float uniformScaleXYZ)
{
	Mat44 uniformScale3DMatrix = MakeUniformScale3D(uniformScaleXYZ);
	Append(uniformScale3DMatrix);
}

void Mat44::AppendScaleNonUniform2D(Vec2 const& nonUniformScaleXY)
{
	Mat44 nonUniformScale2DMatrix = MakeNonUniformScale2D(nonUniformScaleXY);
	Append(nonUniformScale2DMatrix);
}

void Mat44::AppendScaleNonUniform3D(Vec3 const& nonUniformScaleXYZ)
{
	Mat44 nonUniformScale3DMatrix = MakeNonUniformScale3D(nonUniformScaleXYZ);
	Append(nonUniformScale3DMatrix);
}





