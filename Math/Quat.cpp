#include "Engine/Math/Quat.hpp"
#include "Engine/Math/MathUtils.h"
#include "Engine/Math/EulerAngles.hpp"
#include <cmath>

Quat const Quat::DEFAULT(0.f, 0.f, 0.f, 1.f);

Quat::Quat(float x, float y, float z, float w)
	:m_x(x),
	 m_y(y),
	 m_z(z),
	 m_w(w)
{
}

bool Quat::operator==(Quat const& compare) const
{
	return m_x == compare.m_x && m_y == compare.m_y && m_z == compare.m_z && m_w == compare.m_w;
}

bool Quat::operator!=(Quat const& compare) const
{
	return m_x != compare.m_x || m_y != compare.m_y || m_z != compare.m_z || m_w != compare.m_w;
}

Quat const Quat::operator+(Quat const& quatToAdd) const
{
	Quat quaternionSum = Quat(m_x + quatToAdd.m_x, m_y + quatToAdd.m_y, m_z + quatToAdd.m_z, m_w + quatToAdd.m_w);
	return quaternionSum;
}

Quat const Quat::operator-(Quat const& quatToSubtract) const
{
	Quat quaternionDiff = Quat(m_x - quatToSubtract.m_x, m_y - quatToSubtract.m_y, m_z - quatToSubtract.m_z, m_w - quatToSubtract.m_w);
	return quaternionDiff;
}

Quat const Quat::operator*(Quat const& quatToMultiply) const
{
	float xToMultiply = (m_w * quatToMultiply.m_x) + (m_x * quatToMultiply.m_w) + (m_y * quatToMultiply.m_z) - (m_z * quatToMultiply.m_y);
	float yToMultiply = (m_w * quatToMultiply.m_y) - (m_x * quatToMultiply.m_z) + (m_y * quatToMultiply.m_w) + (m_z * quatToMultiply.m_x);
	float zToMultiply = (m_w * quatToMultiply.m_z) + (m_x * quatToMultiply.m_y) - (m_y * quatToMultiply.m_x) + (m_z * quatToMultiply.m_w);
	float wToMultiply = (m_w * quatToMultiply.m_w) - (m_x * quatToMultiply.m_x) - (m_y * quatToMultiply.m_y) - (m_z * quatToMultiply.m_z);

	return Quat(xToMultiply, yToMultiply, zToMultiply, wToMultiply);
}

Quat const Quat::operator*(float scalarValue) const
{
	return Quat(m_x * scalarValue, m_y * scalarValue, m_z * scalarValue, m_w * scalarValue);
}

Quat const Quat::operator/(float scalarValue) const
{
	if (scalarValue != 0.f)
	{
		return Quat(m_x / scalarValue, m_y / scalarValue, m_z / scalarValue, m_w / scalarValue);
	}
	return Quat::DEFAULT;
}

Quat const Quat::MakeFromAxisAngle(Vec3 const& axis, float angle)
{
	Vec3 normalizedAxis = axis.GetNormalized();

	float halfAngle = angle * 0.5f;
	float sinHalfAngle = sinf(halfAngle);
	float cosHalfAngle = cosf(halfAngle);

	Quat quat;

	quat.m_x = normalizedAxis.x * sinHalfAngle;
	quat.m_y = normalizedAxis.y * sinHalfAngle;
	quat.m_z = normalizedAxis.z * sinHalfAngle;
	quat.m_w = cosHalfAngle;

	return quat;
}

Quat const Quat::MakeRotationFromTwoVectors(Vec3 const& fromVector, Vec3 const& toVector)
{
	Vec3 fromNormalized = fromVector.GetNormalized();
	Vec3 toNormalized = toVector.GetNormalized();
	float fromToDot = DotProduct3D(fromNormalized, toNormalized);

	// Check if our vectors are already aligned
	if (fromToDot > 0.9999f)
	{
		return Quat::DEFAULT;
	}

	// Check if our vectors are opposite
	if (fromToDot < -0.9999f)
	{
		// Find a perpendicular vector to rotate 180 degrees around
		Vec3 perpendicularVector = Vec3::XAXE;
		if (fabsf(fromNormalized.x) > 0.999f)
		{
			perpendicularVector = Vec3::YAXE;
		}
		Vec3 axis = CrossProduct3D(fromNormalized, perpendicularVector).GetNormalized();
		return Quat::MakeFromAxisAngle(axis, PI);
	}

	Vec3  axis = CrossProduct3D(fromNormalized, toNormalized);
	float scalar = sqrtf((1 + fromToDot) * 2.f);
	float scalarInverse = 1.f / scalar;

	float quatX = axis.x * scalarInverse;
	float quatY = axis.y * scalarInverse;
	float quatZ = axis.z * scalarInverse;
	float quatW = scalar * 0.5f;

	return Quat(quatX, quatY, quatZ, quatW).GetNormalized();
}

Quat const Quat::MakeFromEulerAngles(EulerAngles const& eulerAngles)
{
	float yawRadians   = ConvertDegreesToRadians(eulerAngles.m_yawDegrees);
	float pitchRadians = ConvertDegreesToRadians(eulerAngles.m_pitchDegrees);
	float rollRadians  = ConvertDegreesToRadians(eulerAngles.m_rollDegrees);

	float cosX = cosf(yawRadians * 0.5f);
	float sinX = sinf(yawRadians * 0.5f);
	float cosY = cosf(pitchRadians * 0.5f);
	float sinY = sinf(pitchRadians * 0.5f);
	float cosZ = cosf(rollRadians * 0.5f);
	float sinZ = sinf(rollRadians * 0.5f);

	Quat quat;

	quat.m_w = cosX * cosY * cosZ - sinX * sinY * sinZ;
	quat.m_x = sinX * cosY * cosZ + cosX * sinY * sinZ;
	quat.m_y = cosX * sinY * cosZ - sinX * cosY * sinZ;
	quat.m_z = cosX * cosY * sinZ + sinX * sinY * cosZ;

	return quat;
}

Quat const Quat::MakeFromMat44(Mat44 const& matrixTransform)
{
	Vec3 iBasis = matrixTransform.GetIBasis3D();
	Vec3 jBasis = matrixTransform.GetJBasis3D();
	Vec3 kBasis = matrixTransform.GetKBasis3D();

	float rotationTrace = iBasis.x + jBasis.y + kBasis.z;
	Quat  quat;

	if (rotationTrace > 0.f)
	{
		float scalar = sqrtf(rotationTrace + 1.f) * 2.f;
		quat.m_w = 0.25f * scalar;
		quat.m_x = (jBasis.z - kBasis.y) / scalar;
		quat.m_y = (kBasis.x - iBasis.z) / scalar;
		quat.m_z = (iBasis.y - jBasis.x) / scalar;
	}
	else if (iBasis.x > jBasis.y && iBasis.x > kBasis.z)
	{
		float scalar = sqrtf(1.f + iBasis.x - jBasis.y - kBasis.z) * 2.f;
		quat.m_w = (jBasis.z - kBasis.y) / scalar;
		quat.m_x = 0.25f * scalar;
		quat.m_y = (jBasis.x + iBasis.y) / scalar;
		quat.m_z = (kBasis.x + iBasis.z) / scalar;
	}
	else if (jBasis.y > kBasis.z)
	{
		float scalar = sqrtf(1.f + jBasis.y - iBasis.x - kBasis.z) * 2.f;
		quat.m_w = (kBasis.x - iBasis.z) / scalar;
		quat.m_x = (jBasis.x + iBasis.y) / scalar;
		quat.m_y = 0.25f * scalar;
		quat.m_z = (kBasis.y + jBasis.z) / scalar;
	}
	else
	{
		float scalar = sqrtf(1.f + kBasis.z - iBasis.x - jBasis.y) * 2.f;
		quat.m_w = (iBasis.y - jBasis.x) / scalar;
		quat.m_x = (kBasis.x + iBasis.z) / scalar;
		quat.m_y = (kBasis.y + jBasis.z) / scalar;
		quat.m_z = 0.25f * scalar;
	}

	return quat;
}

float Quat::GetLength() const
{
	return sqrtf((m_x * m_x) + (m_y * m_y) + (m_z * m_z) + (m_w * m_w));
}

float Quat::GetLengthSquared() const
{
	return (m_x * m_x) + (m_y * m_y) + (m_z * m_z) + (m_w * m_w);
}

Quat const Quat::GetNormalized() const
{
	float lengthSquared = GetLengthSquared();

	if (lengthSquared > 0)
	{
		float length = sqrtf(lengthSquared);
		Quat normalizedVector = Quat(m_x / length, m_y / length, m_z / length, m_w / length);
		return normalizedVector;
	}

	return Quat::DEFAULT;
}

EulerAngles Quat::GetAsEulerAngles() const
{
	EulerAngles eulerAngles;

	// Yaw
	float sinPitch = 2.f * (m_w * m_x - m_y * m_z);
	float cosPitch = 1.f - 2.f * (m_x * m_x + m_y * m_y);
	eulerAngles.m_yawDegrees = Atan2Degrees(sinPitch, cosPitch);

	// Pitch
	float sinYaw = 2.f * (m_w * m_y + m_z * m_x);
	float cosYaw = 1.f - 2.f * (m_y * m_y + m_z * m_z);
	eulerAngles.m_pitchDegrees = Atan2Degrees(sinYaw, cosYaw);

	// Roll
	float sinRoll = 2.f * (m_w * m_z + m_x * m_y);
	float cosRoll = 1.f - 2.f * (m_z * m_z + m_x * m_x);
	eulerAngles.m_rollDegrees = Atan2Degrees(sinRoll, cosRoll);

	return eulerAngles;
}

Mat44 Quat::GetAsMatrix_IFwd_JLeft_KUp() const
{
	Quat quatAsMat = *this;
	quatAsMat.Normalize();

	float x = quatAsMat.m_x;
	float y = quatAsMat.m_y;
	float z = quatAsMat.m_z;
	float w = quatAsMat.m_w;

	float xx = x * x;
	float yy = y * y;
	float zz = z * z;
	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;

	Mat44 matrix = Mat44();

	matrix.m_values[Mat44::Ix] = 1.f - 2.f * (yy + zz);
	matrix.m_values[Mat44::Iy] = 2.f * (xy + wz);
	matrix.m_values[Mat44::Iz] = 2.f * (xz - wy);
	matrix.m_values[Mat44::Iw] = 0.f;

	matrix.m_values[Mat44::Jx] = 2.f * (xy - wz);
	matrix.m_values[Mat44::Jy] = 1.f - 2.f * (xx + zz);
	matrix.m_values[Mat44::Jz] = 2.f * (yz + wx);
	matrix.m_values[Mat44::Jw] = 0.f;

	matrix.m_values[Mat44::Kx] = 2.f * (xz + wy);
	matrix.m_values[Mat44::Ky] = 2.f * (yz - wx);
	matrix.m_values[Mat44::Kz] = 1.f - 2.f * (xx + yy);
	matrix.m_values[Mat44::Kw] = 0.f;

	matrix.m_values[Mat44::Tx] = 0.f;
	matrix.m_values[Mat44::Ty] = 0.f;
	matrix.m_values[Mat44::Tz] = 0.f;
	matrix.m_values[Mat44::Tw] = 1.f;

	return matrix;
}

float Quat::DotQuat(Quat const& quatToDot) const
{
	float dotX = m_x * quatToDot.m_x;
	float dotY = m_y * quatToDot.m_y;
	float dotZ = m_z * quatToDot.m_z;
	float dotW = m_w * quatToDot.m_w;

	return dotX + dotY + dotZ + dotW;
}

Quat Quat::NegateQuat() const
{
	// Negating all parts of the quaternion
	return Quat(-m_x, -m_y, -m_z, -m_w);
}

Quat Quat::ConjugateQuat() const
{
	// Negating the vector parts and keeping the scalar part the same
	return Quat(-m_x, -m_y, -m_z, m_w);
}

Quat Quat::QuatInverse() const
{
	float lengthSquared = GetLengthSquared();

	if (lengthSquared > 0)
	{
		Quat quatConjugated = ConjugateQuat();
		return quatConjugated / lengthSquared;
	}

	return Quat::DEFAULT;
}

void Quat::Normalize()
{
	float length = GetLength();

	if (length > 0)
	{
		m_x /= length;
		m_y /= length;
		m_z /= length;
		m_w /= length;
	}
}
