#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/MathUtils.h"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Core/StringUtils.hpp"

// -----------------------------------------------------------------------------
const EulerAngles EulerAngles::ZERO(0.f, 0.f, 0.f);
// -----------------------------------------------------------------------------

EulerAngles::EulerAngles(float yawDegrees, float pitchDegrees, float rollDegrees)
	:m_yawDegrees(yawDegrees), 
	 m_pitchDegrees(pitchDegrees), 
	 m_rollDegrees(rollDegrees)
{
}

void EulerAngles::GetAsVectors_IFwd_JLeft_KUp(Vec3& out_forwardIBasis, Vec3& out_leftJBasis, Vec3& out_upKBasis) const
{
	float cYaw = CosDegrees(m_yawDegrees);
	float sYaw = SinDegrees(m_yawDegrees);
	float cPitch = CosDegrees(m_pitchDegrees);
	float sPitch = SinDegrees(m_pitchDegrees);
	float cRoll = CosDegrees(m_rollDegrees);
	float sRoll = SinDegrees(m_rollDegrees);

	out_forwardIBasis.x = cYaw * cPitch;
	out_forwardIBasis.y = sYaw * cPitch;
	out_forwardIBasis.z = -sPitch;

	out_leftJBasis.x = cYaw * sPitch * sRoll - sYaw * cRoll;
	out_leftJBasis.y = sYaw * sPitch * sRoll + cYaw * cRoll;
	out_leftJBasis.z = cPitch * sRoll;

	out_upKBasis.x = cYaw * sPitch * cRoll + sYaw * sRoll;
	out_upKBasis.y = sYaw * sPitch * cRoll - cYaw * sRoll;
	out_upKBasis.z = cPitch * cRoll;
}

Mat44 EulerAngles::GetAsMatrix_IFwd_JLeft_KUp() const
{
	// Multiply 64 times using append
	//Mat44 eulerMatrix;
	//eulerMatrix.AppendZRotation(m_yawDegrees);
	//eulerMatrix.AppendYRotation(m_pitchDegrees);
	//eulerMatrix.AppendXRotation(m_rollDegrees);
	//return eulerMatrix;

	// Multiply 16 times using CosDegrees and SinDegrees
	float cYaw	 = CosDegrees(m_yawDegrees);  
	float sYaw	 = SinDegrees(m_yawDegrees);    
	float cPitch = CosDegrees(m_pitchDegrees); 
	float sPitch = SinDegrees(m_pitchDegrees);  
	float cRoll  = CosDegrees(m_rollDegrees);   
	float sRoll  = SinDegrees(m_rollDegrees);  

	Mat44 eulerMatrix;
	eulerMatrix.m_values[Mat44::Ix] = cYaw * cPitch;
	eulerMatrix.m_values[Mat44::Iy] = sYaw * cPitch;
	eulerMatrix.m_values[Mat44::Iz] = -sPitch;
	eulerMatrix.m_values[Mat44::Iw] = 0.f;

	eulerMatrix.m_values[Mat44::Jx] = cYaw * sPitch * sRoll - sYaw * cRoll;
	eulerMatrix.m_values[Mat44::Jy] = sYaw * sPitch * sRoll + cYaw * cRoll;
	eulerMatrix.m_values[Mat44::Jz] = cPitch * sRoll;
	eulerMatrix.m_values[Mat44::Jw] = 0.f;

	eulerMatrix.m_values[Mat44::Kx] = cYaw * sPitch * cRoll + sYaw * sRoll;
	eulerMatrix.m_values[Mat44::Ky] = sYaw * sPitch * cRoll - cYaw * sRoll;
	eulerMatrix.m_values[Mat44::Kz] = cPitch * cRoll;
	eulerMatrix.m_values[Mat44::Kw] = 0.f;

	eulerMatrix.m_values[Mat44::Tx] = 0.f;
	eulerMatrix.m_values[Mat44::Ty] = 0.f;
	eulerMatrix.m_values[Mat44::Tz] = 0.f;
	eulerMatrix.m_values[Mat44::Tw] = 1.f;

	return eulerMatrix;
}

void EulerAngles::SetFromText(char const* text)
{
	Strings commaSplit = SplitStringOnDelimiter(text, ',');
	m_yawDegrees = static_cast<float>(atof(commaSplit[0].c_str()));
	m_pitchDegrees = static_cast<float>(atof(commaSplit[1].c_str()));
	m_rollDegrees = static_cast<float>(atof(commaSplit[2].c_str()));
}
