#include "AnalogJoystick.h"
#include <Engine/Math/Vec2.hpp>
#include <Engine/Math/MathUtils.h>


Vec2 AnalogJoystick::GetPosition() const
{
    return m_correctedPosition;
}

float AnalogJoystick::GetMagnitude() const
{
    return m_correctedPosition.GetLength();
}

float AnalogJoystick::GetOrientationDegrees() const
{
    return m_correctedPosition.GetOrientationDegrees();
}

Vec2 AnalogJoystick::GetRawUncorrectedPosition() const
{
    return m_rawPosition;
}

float AnalogJoystick::GetInnerDeadZoneFraction() const
{
    return m_innerDeadZoneFraction;
}

float AnalogJoystick::GetOuterDeadZoneFraction() const
{
    return m_outerDeadZoneFraction;
}

void AnalogJoystick::Reset()
{
}

void AnalogJoystick::SetDeadZoneThresholds(float normalizedInnerDeadZoneThreshold, float normalizedOuterDeadZoneThreshold)
{
    m_innerDeadZoneFraction = normalizedInnerDeadZoneThreshold;
    m_outerDeadZoneFraction = normalizedOuterDeadZoneThreshold;
}

void AnalogJoystick::UpdatePosition(float rawNormalizedX, float rawNormalizedY)
{
    m_rawPosition = Vec2(rawNormalizedX, rawNormalizedY);
    float thetaRadians = m_rawPosition.GetOrientationRadians();
    float rawMagnitude = m_rawPosition.GetLength();
    float correctedMagnitude = RangeMapClamped(rawMagnitude, m_innerDeadZoneFraction, m_outerDeadZoneFraction, 0.0f, 1.0f);
    m_correctedPosition.SetPolarRadians(thetaRadians, correctedMagnitude);
}
