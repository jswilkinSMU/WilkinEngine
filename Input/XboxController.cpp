
#define WIN32_LEAN_AND_MEAN		// Always #define this before #including <windows.h>
#include <windows.h>			// #include this (massive, platform-specific) header in VERY few places (and .CPPs only)

#include <Xinput.h>
#pragma comment(lib, "xinput")

#include "XboxController.h"
#include <Engine/Math/MathUtils.h>

XboxController::XboxController()
{
}

XboxController::~XboxController()
{
}

bool XboxController::IsConnected() const
{
	if (m_isConnected)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int XboxController::GetControllerID() const
{
	return m_id;
}

AnalogJoystick const& XboxController::GetLeftStick() const
{
	return m_leftStick;
}

AnalogJoystick const& XboxController::GetRightStick() const
{
	return m_rightStick;
}

float XboxController::GetLeftTrigger() const
{
	return m_leftTrigger;
}

float XboxController::GetRightTrigger() const
{
	return m_rightTrigger;
}

KeyButtonState const& XboxController::GetButton(XboxButtonID buttonID) const
{

	return m_buttons[buttonID];
}

bool XboxController::IsButtonDown(XboxButtonID buttonID) const
{
	return m_buttons[buttonID].m_isPressed;
}

bool XboxController::WasButtonJustPressed(XboxButtonID buttonID) const
{
	return m_buttons[buttonID].m_isPressed && !m_buttons[buttonID].m_wasPressedLastFrame;
}

bool XboxController::WasButtonJustReleased(XboxButtonID buttonID) const
{
	return !m_buttons[buttonID].m_isPressed && m_buttons[buttonID].m_wasPressedLastFrame;
}

void XboxController::Update()
{
	XINPUT_STATE xboxControllerState;
	memset(&xboxControllerState, 0, sizeof(xboxControllerState));
	DWORD errorStatus = XInputGetState(m_id, &xboxControllerState);
	if (errorStatus != ERROR_SUCCESS)
	{
		Reset();
		m_isConnected = false;
		return;
	}

	m_isConnected = true;

	XINPUT_GAMEPAD const& state = xboxControllerState.Gamepad;
	UpdateJoyStick(m_leftStick, state.sThumbLX, state.sThumbLY);
	UpdateJoyStick(m_rightStick, state.sThumbRX, state.sThumbRY);

	UpdateTrigger(m_leftTrigger, state.bLeftTrigger);
	UpdateTrigger(m_rightTrigger, state.bRightTrigger);

	UpdateButton(XBOX_BUTTON_A, state.wButtons, XINPUT_GAMEPAD_A);
	UpdateButton(XBOX_BUTTON_B, state.wButtons, XINPUT_GAMEPAD_B);
	UpdateButton(XBOX_BUTTON_X, state.wButtons, XINPUT_GAMEPAD_X);
	UpdateButton(XBOX_BUTTON_Y, state.wButtons, XINPUT_GAMEPAD_Y);
	UpdateButton(XBOX_BUTTON_BACK, state.wButtons, XINPUT_GAMEPAD_BACK);
	UpdateButton(XBOX_BUTTON_START, state.wButtons, XINPUT_GAMEPAD_START);
	UpdateButton(XBOX_BUTTON_LSHOULDER, state.wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER);
	UpdateButton(XBOX_BUTTON_RSHOULDER, state.wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER);
	UpdateButton(XBOX_BUTTON_LTHUMB, state.wButtons, XINPUT_GAMEPAD_LEFT_THUMB);
	UpdateButton(XBOX_BUTTON_RTHUMB, state.wButtons, XINPUT_GAMEPAD_RIGHT_THUMB);
	UpdateButton(XBOX_BUTTON_DPAD_RIGHT, state.wButtons, XINPUT_GAMEPAD_DPAD_RIGHT);
	UpdateButton(XBOX_BUTTON_DPAD_UP, state.wButtons, XINPUT_GAMEPAD_DPAD_UP);
	UpdateButton(XBOX_BUTTON_DPAD_DOWN, state.wButtons, XINPUT_GAMEPAD_DPAD_DOWN);
	UpdateButton(XBOX_BUTTON_DPAD_LEFT, state.wButtons, XINPUT_GAMEPAD_DPAD_LEFT);
}

void XboxController::Reset()
{
	m_isConnected = false;
	m_leftTrigger = 0.f;
	m_rightTrigger = 0.f;

	for (int i = 0; i < NUM_XBOX_BUTTONS; ++i)
	{
		m_buttons[i].m_isPressed = false;
		m_buttons[i].m_wasPressedLastFrame = false;
	}

	m_leftStick.Reset();
	m_rightStick.Reset();
}

void XboxController::UpdateJoyStick(AnalogJoystick& out_joystick, short rawX, short rawY)
{
	float rawNormalizedX = RangeMapClamped(static_cast<float>(rawX), XBOX_JOYSTICK_MIN_AXIS_VALUE, XBOX_JOYSTICK_MAX_AXIS_VALUE, -1.f, 1.f);
	float rawNormalizedY = RangeMapClamped(static_cast<float>(rawY), XBOX_JOYSTICK_MIN_AXIS_VALUE, XBOX_JOYSTICK_MAX_AXIS_VALUE, -1.f, 1.f);
	out_joystick.UpdatePosition(rawNormalizedX, rawNormalizedY);
}

void XboxController::UpdateTrigger(float& out_triggerValue, unsigned char rawValue)
{
	out_triggerValue = RangeMapClamped(static_cast<float>(rawValue), 5.f, 250.f, 0.f, 1.f);
}

void XboxController::UpdateButton(XboxButtonID buttonID, unsigned short buttonFlags, unsigned short buttonFlag)
{
	KeyButtonState& button = m_buttons[buttonID];
	button.m_wasPressedLastFrame = button.m_isPressed;
	button.m_isPressed = (buttonFlags & buttonFlag) == buttonFlag;
}
