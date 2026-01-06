#pragma once
#include "KeyButtonState.h"
#include "AnalogJoystick.h"

const short XBOX_JOYSTICK_MIN_AXIS_VALUE = -32768;
const short XBOX_JOYSTICK_MAX_AXIS_VALUE = 32767;

enum XboxButtonID
{
	XBOX_BUTTON_A,
	XBOX_BUTTON_B,
	XBOX_BUTTON_X,
	XBOX_BUTTON_Y,
	XBOX_BUTTON_BACK,
	XBOX_BUTTON_START,
	XBOX_BUTTON_LSHOULDER,
	XBOX_BUTTON_RSHOULDER,
	XBOX_BUTTON_LTHUMB,
	XBOX_BUTTON_RTHUMB,
	XBOX_BUTTON_DPAD_RIGHT,
	XBOX_BUTTON_DPAD_UP,
	XBOX_BUTTON_DPAD_DOWN,
	XBOX_BUTTON_DPAD_LEFT,
	NUM_XBOX_BUTTONS

};

class XboxController
{
	friend class InputSystem;

public:
	XboxController();
	~XboxController();
	bool IsConnected() const;
	int GetControllerID() const;
	AnalogJoystick const& GetLeftStick() const;
	AnalogJoystick const& GetRightStick() const;
	float GetLeftTrigger() const;
	float GetRightTrigger() const;
	KeyButtonState const& GetButton(XboxButtonID buttonID) const;
	bool IsButtonDown(XboxButtonID buttonID) const;
	bool WasButtonJustPressed(XboxButtonID buttonID) const;
	bool WasButtonJustReleased(XboxButtonID buttonID) const;

private:
	void Update();
	void Reset();
	void UpdateJoyStick(AnalogJoystick& out_joystick, short rawX, short rawY);
	void UpdateTrigger(float& out_triggerValue, unsigned char rawValue);
	void UpdateButton(XboxButtonID buttonID, unsigned short buttonFlags, unsigned short buttonFlag);

private:
	int m_id = 0;
	bool m_isConnected = false;
	float m_leftTrigger = 0.f;
	float m_rightTrigger = 0.f;
	KeyButtonState m_buttons[(int)XboxButtonID::NUM_XBOX_BUTTONS]; 
	AnalogJoystick m_leftStick;
	AnalogJoystick m_rightStick;

};