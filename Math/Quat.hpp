#pragma once
#include "Engine/Math/Mat44.hpp"
// -----------------------------------------------------------------------------
struct EulerAngles;
// -----------------------------------------------------------------------------
class Quat
{
public:
	static const Quat DEFAULT;

	Quat() = default;
	Quat(float x, float y, float z, float w);

	/* Operators */
	bool		operator==(Quat const& compare) const;
	bool		operator!=(Quat const& compare) const;
	Quat const  operator+(Quat const& quatToAdd) const;
	Quat const  operator-(Quat const& quatToSubtract) const;
	Quat const  operator*(Quat const& quatToMultiply) const;
	Quat const  operator*(float scalarValue) const;
	Quat const  operator/(float scalarValue) const;

	static Quat const MakeFromAxisAngle(Vec3 const& axis, float angle);
	static Quat const MakeRotationFromTwoVectors(Vec3 const& fromVector, Vec3 const& toVector);
	static Quat const MakeFromEulerAngles(EulerAngles const& eulerAngles);
	static Quat const MakeFromMat44(Mat44 const& matrixTransform);

	/* Length and Normalization */
	float GetLength() const;
	float GetLengthSquared() const;
	Quat const GetNormalized() const;
	void Normalize();

	/* Conversion methods */
	EulerAngles GetAsEulerAngles() const;
	Mat44 GetAsMatrix_IFwd_JLeft_KUp() const;

	/* Quaternion helper functions */
	float DotQuat(Quat const& quatToDot) const;
	Quat NegateQuat() const;
	Quat ConjugateQuat() const;
	Quat QuatInverse() const;

public:
	float m_x = 0.0f;
	float m_y = 0.0f;
	float m_z = 0.0f;
	float m_w = 1.0f;
};