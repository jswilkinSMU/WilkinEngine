#pragma once
// -----------------------------------------------------------------------------
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.h"
#include "Engine/Math/Vec4.hpp"
// -----------------------------------------------------------------------------
class Quat;
// -----------------------------------------------------------------------------
class Mat44
{
public:
	enum { Ix,Iy,Iz,Iw,  Jx,Jy,Jz,Jw,  Kx,Ky,Kz,Kw,  Tx,Ty,Tz,Tw};
	float m_values[16];

	Mat44(); // Default constructor is the Identity Matrix
	explicit Mat44(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translation2D);
	explicit Mat44(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translation3D);
	explicit Mat44(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D);
	explicit Mat44(float const* sixteenValuesBasisMajor);

	Mat44 operator*(Mat44 const& matToMultiply) const;

	static Mat44 const MakeTranslation2D(Vec2 const& translationXY);
	static Mat44 const MakeTranslation3D(Vec3 const& translationXYZ);
	static Mat44 const MakeUniformScale2D(float uniformScaleXY);
	static Mat44 const MakeUniformScale3D(float uniformScaleXYZ);
	static Mat44 const MakeNonUniformScale2D(Vec2 const& nonUniformScaleXY);
	static Mat44 const MakeNonUniformScale3D(Vec3 const& nonUniformScaleXYZ);
	static Mat44 const MakeZRotationDegrees(float rotationDegreesAboutZ);
	static Mat44 const MakeYRotationDegrees(float rotationDegreesAboutY);
	static Mat44 const MakeXRotationDegrees(float rotationDegreesAboutX);
	static Mat44 const MakeAxisRotationDegrees(Vec3 const& axis, float rotationDegreesAboutAxis);
	static Mat44 const MakeOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar);
	static Mat44 const MakePerspectiveProjection(float fovYDegrees, float aspect, float zNear, float zFar);
	static Mat44 const MakeTransform(Vec3 const& position, Quat const& rotation, Vec3 const& scale);

	Vec2 const TransformVectorQuantity2D(Vec2 const& vectorQuantityXY) const;	// Assumes z=0, w=0
	Vec3 const TransformVectorQuantity3D(Vec3 const& vectorQuantityXYZ) const;  // Assumes w=0
	Vec2 const TransformPosition2D(Vec2 const& positionXY) const;				// Assumes z=0, w=1
	Vec3 const TransformPosition3D(Vec3 const& position3D) const;				// Assumes w=1
	Vec4 const TransformHomogeneous3D(Vec4 const& homogenousPoint3D) const;		// w is provided

	float* GetAsFloatArray(); // non const mutable version
	float const* GetAsFloatArray() const; // const version, used only when Mat44 is const
	Vec2 const  GetIBasis2D() const;
	Vec2 const  GetJBasis2D() const;
	Vec2 const  GetTranslation2D() const;
	Vec3 const  GetIBasis3D() const;
	Vec3 const  GetJBasis3D() const;
	Vec3 const  GetKBasis3D() const;
	Vec3 const  GetTranslation3D() const;
	Vec4 const  GetIBasis4D() const;
	Vec4 const  GetJBasis4D() const;
	Vec4 const  GetKBasis4D() const;
	Vec4 const  GetTranslation4D() const;
	Mat44 const GetOrthonormalInverse() const;	// Only works for OrthoNormal affine matrices

	void SetTranslation2D(Vec2 const& translationXY);
	void SetTranslation3D(Vec3 const& translationXYZ);
	void SetIJ2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D);
	void SetIJT2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translationXY);
	void SetIJK3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D);
	void SetIJKT3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translationXYZ);
	void SetIJKT4D(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D);
	void Transpose(); // Swaps columns with rows
	void Orthonormalize_IFwd_JLeft_KUp(); // Forward is canonical, Up is secondary, Left tertiary

	void Append(Mat44 const& appendThis);							// Multiply on right in column notation / on left in row notation
	void AppendZRotation(float degreesRotationAboutZ);				// Same as appending (*= in column notation) a z-rotation matrix
	void AppendYRotation(float degreesRotationAboutY);				// Same as appending (*= in column notation) a y-rotation matrix
	void AppendXRotation(float degreesRotationAboutX);				// Same as appending (*= in column notation) a x - rotation matrix
	void AppendTranslation2D(Vec2 const& translationXY);			// Same as appending (*= in column notation) a translation matrix
	void AppendTranslation3D(Vec3 const& translationXYZ);			// Same as appending (*= in column notation) a translation matrix
	void AppendScaleUniform2D(float uniformScaleXY);				// K and T bases should remain unaffected
	void AppendScaleUniform3D(float uniformScaleXYZ);				// translation should remain unaffected
	void AppendScaleNonUniform2D(Vec2 const& nonUniformScaleXY);    // K and T bases should remain unaffected
	void AppendScaleNonUniform3D(Vec3 const& nonUniformScaleXYZ);	// translation should remain unaffected
};