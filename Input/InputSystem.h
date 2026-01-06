#pragma once
#include "Engine/Input/KeyButtonState.h"
#include "Engine/Input/XboxController.h"
#include "Engine/Core/EventSystem.hpp"
// -----------------------------------------------------------------------------
extern unsigned char const KEYCODE_F1;
extern unsigned char const KEYCODE_F2;
extern unsigned char const KEYCODE_F3;
extern unsigned char const KEYCODE_F4;
extern unsigned char const KEYCODE_F5;
extern unsigned char const KEYCODE_F6;
extern unsigned char const KEYCODE_F7;
extern unsigned char const KEYCODE_F8;
extern unsigned char const KEYCODE_F9;
extern unsigned char const KEYCODE_F10;
extern unsigned char const KEYCODE_F11;
extern unsigned char const KEYCODE_ESC;
extern unsigned char const KEYCODE_UPARROW;
extern unsigned char const KEYCODE_DOWNARROW;
extern unsigned char const KEYCODE_LEFTARROW;
extern unsigned char const KEYCODE_RIGHTARROW;
extern unsigned char const KEYCODE_LEFT_MOUSE;
extern unsigned char const KEYCODE_MIDDLE_MOUSE;
extern unsigned char const KEYCODE_RIGHT_MOUSE;
extern unsigned char const KEYCODE_TILDE;
extern unsigned char const KEYCODE_LEFTBRACKET;
extern unsigned char const KEYCODE_RIGHTBRACKET;
extern unsigned char const KEYCODE_ENTER;
extern unsigned char const KEYCODE_BACKSPACE;
extern unsigned char const KEYCODE_INSERT;
extern unsigned char const KEYCODE_DELETE;
extern unsigned char const KEYCODE_HOME;
extern unsigned char const KEYCODE_END;
extern unsigned char const KEYCODE_SHIFT;
extern unsigned char const KEYCODE_CTRL;
extern unsigned char const KEYCODE_ALT;
extern unsigned char const KEYCODE_TAB;
extern unsigned char const KEYCODE_CAPS;
extern unsigned char const KEYCODE_SPACE;
extern unsigned char const KEYCODE_PAGEUP;
extern unsigned char const KEYCODE_PAGEDOWN;
extern unsigned char const KEYCODE_LEFTWINDOWS;
extern unsigned char const KEYCODE_RIGHTWINDOWS;
extern unsigned char const KEYCODE_NUMPAD0;
extern unsigned char const KEYCODE_NUMPAD1;
extern unsigned char const KEYCODE_NUMPAD2;
extern unsigned char const KEYCODE_NUMPAD3;
extern unsigned char const KEYCODE_NUMPAD4;
extern unsigned char const KEYCODE_NUMPAD5;
extern unsigned char const KEYCODE_NUMPAD6;
extern unsigned char const KEYCODE_NUMPAD7;
extern unsigned char const KEYCODE_NUMPAD8;
extern unsigned char const KEYCODE_NUMPAD9;
extern unsigned char const KEYCODE_MULTIPLY;
extern unsigned char const KEYCODE_ADD;
extern unsigned char const KEYCODE_DIVIDE;
extern unsigned char const KEYCODE_SUBTRACT;
extern unsigned char const KEYCODE_SEPARATOR;
// -----------------------------------------------------------------------------
constexpr int NUM_KEYCODES = 256;
constexpr int NUM_XBOX_CONTROLLERS = 4;
//------------------------------------------------------------------------------
struct InputSystemConfig
{

};
// -----------------------------------------------------------------------------
enum class CursorMode
{
	POINTER,
	FPS
};
// -----------------------------------------------------------------------------
struct CursorState
{
	IntVec2 m_cursorClientDelta;
	IntVec2 m_cursorClientPosition;

	CursorMode m_cursorMode = CursorMode::POINTER;
};
class InputSystem
{
public:
	InputSystem(InputSystemConfig const& config);
	~InputSystem();
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();
	bool WasKeyJustPressed(unsigned char keyCode);
	bool WasKeyJustReleased(unsigned char keyCode);
	bool IsKeyDown(unsigned char keyCode);
	void HandleKeyPressed(unsigned char keyCode);
	void HandleKeyReleased(unsigned char keyCode);

	static bool Event_HandleKeyPressed(EventArgs& args);
	static bool Event_HandleKeyReleased(EventArgs& args);
	static bool Event_HandleMouseWheelScrolled(EventArgs& args);
	
	XboxController const& GetController(int controllerID);

	void SetCursorMode(CursorMode cursorMode);
	void CenterCursor();
	Vec2 GetCursorClientDelta() const;
	Vec2 GetCursorClientPosition() const;
	Vec2 GetCursorNormalizedPosition() const;
	int  GetMouseWheelDelta() const;
	bool WasMouseWheelScrolledUp() const;
	bool WasMouseWheelScrolledDown() const;

protected:
	KeyButtonState m_keyStates[NUM_KEYCODES];
	XboxController m_controllers[NUM_XBOX_CONTROLLERS];

private:
	InputSystemConfig m_config;
	CursorState       m_cursorState;
	IntVec2           m_previousCursorPosition;
	int               m_mouseWheelDelta = 0;
};