
#define WIN32_LEAN_AND_MEAN		// Always #define this before #including <windows.h>
#include <windows.h>			// #include this (massive, platform-specific) header in VERY few places (and .CPPs only)
#include "Engine/Input/InputSystem.h"
#include "Engine/Window/Window.hpp"

InputSystem* g_theInput = nullptr;
// -----------------------------------------------------------------------------
unsigned char const KEYCODE_F1 = VK_F1; // 112 == 0x70
unsigned char const KEYCODE_F2 = VK_F2;
unsigned char const KEYCODE_F3 = VK_F3;
unsigned char const KEYCODE_F4 = VK_F4;
unsigned char const KEYCODE_F5 = VK_F5;
unsigned char const KEYCODE_F6 = VK_F6;
unsigned char const KEYCODE_F7 = VK_F7;
unsigned char const KEYCODE_F8 = VK_F8;
unsigned char const KEYCODE_F9 = VK_F9;
unsigned char const KEYCODE_F10 = VK_F10;
unsigned char const KEYCODE_F11 = VK_F11;
unsigned char const KEYCODE_ESC = VK_ESCAPE;
unsigned char const KEYCODE_UPARROW = VK_UP;
unsigned char const KEYCODE_DOWNARROW = VK_DOWN;
unsigned char const KEYCODE_LEFTARROW = VK_LEFT;
unsigned char const KEYCODE_RIGHTARROW = VK_RIGHT;
unsigned char const KEYCODE_LEFT_MOUSE = VK_LBUTTON;
unsigned char const KEYCODE_MIDDLE_MOUSE = VK_MBUTTON;
unsigned char const KEYCODE_RIGHT_MOUSE = VK_RBUTTON;
unsigned char const KEYCODE_TILDE = 0xC0;
unsigned char const KEYCODE_LEFTBRACKET = 0xDB;
unsigned char const KEYCODE_RIGHTBRACKET = 0xDD;
unsigned char const KEYCODE_ENTER = VK_RETURN;
unsigned char const KEYCODE_BACKSPACE = VK_BACK;
unsigned char const KEYCODE_INSERT = VK_INSERT;
unsigned char const KEYCODE_DELETE = VK_DELETE;
unsigned char const KEYCODE_HOME = VK_HOME;
unsigned char const KEYCODE_END = VK_END;
unsigned char const KEYCODE_SHIFT = VK_SHIFT;
unsigned char const KEYCODE_CTRL = VK_CONTROL;
unsigned char const KEYCODE_ALT = VK_MENU;
unsigned char const KEYCODE_TAB = VK_TAB;
unsigned char const KEYCODE_CAPS = VK_CAPITAL;
unsigned char const KEYCODE_SPACE = VK_SPACE;
unsigned char const KEYCODE_PAGEUP = VK_PRIOR;
unsigned char const KEYCODE_PAGEDOWN = VK_NEXT;
unsigned char const KEYCODE_LEFTWINDOWS = VK_LWIN;
unsigned char const KEYCODE_RIGHTWINDOWS = VK_RWIN;
unsigned char const KEYCODE_NUMPAD0 = VK_NUMPAD0;
unsigned char const KEYCODE_NUMPAD1 = VK_NUMPAD1;
unsigned char const KEYCODE_NUMPAD2 = VK_NUMPAD2;
unsigned char const KEYCODE_NUMPAD3 = VK_NUMPAD3;
unsigned char const KEYCODE_NUMPAD4 = VK_NUMPAD4;
unsigned char const KEYCODE_NUMPAD5 = VK_NUMPAD5;
unsigned char const KEYCODE_NUMPAD6 = VK_NUMPAD6;
unsigned char const KEYCODE_NUMPAD7 = VK_NUMPAD7;
unsigned char const KEYCODE_NUMPAD8 = VK_NUMPAD8;
unsigned char const KEYCODE_NUMPAD9 = VK_NUMPAD9;
unsigned char const KEYCODE_MULTIPLY = VK_MULTIPLY;
unsigned char const KEYCODE_ADD = VK_ADD;
unsigned char const KEYCODE_DIVIDE = VK_DIVIDE;
unsigned char const KEYCODE_SUBTRACT = VK_SUBTRACT;
unsigned char const KEYCODE_SEPARATOR = VK_SEPARATOR;
// -----------------------------------------------------------------------------
InputSystem::InputSystem(InputSystemConfig const& config)
	:m_config(config)
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::Startup()
{
	SubscribeEventCallbackFunction("KeyPressed", Event_HandleKeyPressed);
	SubscribeEventCallbackFunction("KeyReleased", Event_HandleKeyReleased);
	SubscribeEventCallbackFunction("MouseWheelScrolled", Event_HandleMouseWheelScrolled);
}

void InputSystem::Shutdown()
{
}

void InputSystem::BeginFrame()
{
	m_controllers[0].IsConnected();
	m_controllers[0].Update();

	while (m_cursorState.m_cursorMode == CursorMode::FPS && ShowCursor(false) >= 0)
	{
		ShowCursor(false);
	}
	while (m_cursorState.m_cursorMode == CursorMode::POINTER && ShowCursor(true) < 0)
	{
		ShowCursor(true);
	}

	HWND windowHandle = static_cast<HWND>(Window::s_mainWindow->GetHwnd());
	m_previousCursorPosition = m_cursorState.m_cursorClientPosition;
	POINT cursorCoords;
	GetCursorPos(&cursorCoords);
	ScreenToClient(windowHandle, &cursorCoords);
	m_cursorState.m_cursorClientPosition = IntVec2(cursorCoords.x, cursorCoords.y);
	if (m_cursorState.m_cursorMode == CursorMode::FPS)
	{
		m_cursorState.m_cursorClientDelta = m_previousCursorPosition - m_cursorState.m_cursorClientPosition;
		POINT centerCoords;
		RECT clientRect;
		GetClientRect(windowHandle, &clientRect);

		centerCoords.x = (long)(((float)clientRect.right - (float)clientRect.left) * 0.5f);
		centerCoords.y = (long)(((float)clientRect.bottom - (float)clientRect.top) * 0.5f);
		ClientToScreen(windowHandle, &centerCoords);
		SetCursorPos(centerCoords.x, centerCoords.y);
		GetCursorPos(&cursorCoords);

		ScreenToClient(windowHandle, &cursorCoords);
		m_cursorState.m_cursorClientPosition = IntVec2(cursorCoords.x, cursorCoords.y);
	}
	else
	{
		m_cursorState.m_cursorClientDelta = IntVec2::ZERO;
	}
}

void InputSystem::EndFrame()
{
	// Reset wheel delta every frame
	m_mouseWheelDelta = 0;

	for (int keyIndex = 0; keyIndex < 256; ++keyIndex)
	{
		m_keyStates[keyIndex].m_wasPressedLastFrame = m_keyStates[keyIndex].m_isPressed; 
	}
}

bool InputSystem::WasKeyJustPressed(unsigned char keyCode)
{
	KeyButtonState& keyState = m_keyStates[keyCode];
	return keyState.m_isPressed && !keyState.m_wasPressedLastFrame;
}

bool InputSystem::WasKeyJustReleased(unsigned char keyCode)
{
	KeyButtonState& keyState = m_keyStates[keyCode];
	return !keyState.m_isPressed && keyState.m_wasPressedLastFrame;
}

bool InputSystem::IsKeyDown(unsigned char keyCode)
{
	KeyButtonState& keyState = m_keyStates[keyCode];
	return keyState.m_isPressed;
}

void InputSystem::HandleKeyPressed(unsigned char keyCode)
{
	KeyButtonState& keyState = m_keyStates[keyCode];
	keyState.m_isPressed = true;
}

void InputSystem::HandleKeyReleased(unsigned char keyCode)
{
	KeyButtonState& keyState = m_keyStates[keyCode];
	keyState.m_isPressed = false;
	keyState.m_wasPressedLastFrame = true;
}

bool InputSystem::Event_HandleKeyPressed(EventArgs& args)
{
	if (!g_theInput)
	{
		return false;
	}
	unsigned char keyCode = (unsigned char)args.GetValue("KeyCode", -1);
	g_theInput->HandleKeyPressed(keyCode);
	return true;
}

bool InputSystem::Event_HandleKeyReleased(EventArgs& args)
{
	if (!g_theInput)
	{
		return false;
	}
	unsigned char keyCode = (unsigned char)args.GetValue("KeyCode", -1);
	g_theInput->HandleKeyReleased(keyCode);
	return true;
}

bool InputSystem::Event_HandleMouseWheelScrolled(EventArgs& args)
{
	if (!g_theInput)
	{
		return false;
	}

	int mouseDelta = args.GetValue("MouseDelta", 0);
	g_theInput->m_mouseWheelDelta += mouseDelta;

	return true;
}

XboxController const& InputSystem::GetController(int controllerID)
{
	return m_controllers[controllerID];
}

void InputSystem::SetCursorMode(CursorMode cursorMode)
{
	m_cursorState.m_cursorMode = cursorMode;

	if (cursorMode == CursorMode::POINTER)
	{
		ShowCursor(true);
	}
	else if (cursorMode == CursorMode::FPS)
	{
		ShowCursor(false);
	}
}

Vec2 InputSystem::GetCursorClientDelta() const
{
	if (m_cursorState.m_cursorMode == CursorMode::FPS)
	{
		return Vec2(static_cast<float>(m_cursorState.m_cursorClientDelta.x), static_cast<float>(m_cursorState.m_cursorClientDelta.y));
	}
	return Vec2::ZERO;
}

Vec2 InputSystem::GetCursorClientPosition() const
{
	return Vec2(static_cast<float>(m_cursorState.m_cursorClientPosition.x), static_cast<float>(m_cursorState.m_cursorClientPosition.y));
}

Vec2 InputSystem::GetCursorNormalizedPosition() const
{
	HWND windowHandle = static_cast<HWND>(Window::s_mainWindow->GetHwnd());
	IntVec2 const& cursorPosition = m_cursorState.m_cursorClientPosition;

	RECT clientRect;
	::GetClientRect(windowHandle, &clientRect);

	float cursorX = static_cast<float>(cursorPosition.x) / static_cast<float>(clientRect.right);
	float cursorY = static_cast<float>(cursorPosition.y) / static_cast<float>(clientRect.bottom);

	return Vec2(cursorX, 1.f - cursorY);
}

int InputSystem::GetMouseWheelDelta() const
{
	return m_mouseWheelDelta;
}

bool InputSystem::WasMouseWheelScrolledUp() const
{
	return m_mouseWheelDelta > 0;
}

bool InputSystem::WasMouseWheelScrolledDown() const
{
	return m_mouseWheelDelta < 0;
}

void InputSystem::CenterCursor()
{
	IntVec2 clientDimensions = Window::s_mainWindow->GetClientDimensions();

	int clientCenterX = (clientDimensions.x / 2);
	int clientCenterY = (clientDimensions.y / 2);
	
	SetCursorPos(clientCenterX, clientCenterY);
}
