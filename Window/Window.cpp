#include "Engine/Window/Window.hpp"

#define WIN32_LEAN_AND_MEAN		// Always #define this before #including <windows.h>
#include <windows.h>			// #include this (massive, platform-specific) header in VERY few places (and .CPPs only)
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Input/InputSystem.h"
#include "Engine/Core/EventSystem.hpp"

//--------------------------------------------------------------------------------------------------------
// Global Variables
Window* Window::s_mainWindow = nullptr;

//-----------------------------------------------------------------------------------------------
// Handles Windows (Win32) messages/events; i.e. the OS is trying to tell us something happened.
// This function is called back by Windows whenever we tell it to (by calling DispatchMessage).

LRESULT CALLBACK WindowsMessageHandlingProcedure(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam)
{
	InputSystem* input = nullptr;
	if (Window::s_mainWindow)
	{
		WindowConfig const& config = Window::s_mainWindow->GetConfig();
		input = config.m_inputSystem;
	}

	switch (wmMessageCode)
	{
		// App close requested via "X" button, or right-click "Close Window" on task bar, or "Close" from system menu, or Alt-F4
		case WM_CLOSE:
		{
			FireEvent("Quit");
			return 0;
		}

		// Raw physical keyboard "key-was-just-depressed" event (case-insensitive, not translated)
		case WM_KEYDOWN:
		{
			EventArgs args;
			args.SetValue("KeyCode", Stringf("%d", (unsigned char)wParam));
			FireEvent("KeyPressed", args);
			return 0;
		}

		// Raw physical keyboard "key-was-just-released" event (case-insensitive, not translated)
		case WM_KEYUP:
		{
			EventArgs args;
			args.SetValue("KeyCode", Stringf("%d", (unsigned char)wParam));
			FireEvent("KeyReleased", args);
			return 0;
		}
		case WM_CHAR:
		{
			EventArgs args;
			args.SetValue("CharCode", Stringf("%d", (unsigned char)wParam));
			FireEvent("CharInput", args);
			return 0;
		}
		// Mouse button down and up events, treated as fake keyboard keys
		case WM_LBUTTONDOWN:
		{
			if (input)
			{
				input->HandleKeyPressed(KEYCODE_LEFT_MOUSE);
			}
			return 0;
		}
		case WM_LBUTTONUP:
		{
			if (input)
			{
				input->HandleKeyReleased(KEYCODE_LEFT_MOUSE);
			}
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			if (input)
			{
				input->HandleKeyPressed(KEYCODE_RIGHT_MOUSE);
			}
			return 0;
		}
		case WM_RBUTTONUP:
		{
			if (input)
			{
				input->HandleKeyReleased(KEYCODE_RIGHT_MOUSE);
			}
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			if (input)
			{
				int delta = GET_WHEEL_DELTA_WPARAM(wParam);
				EventArgs args;
				args.SetValue("MouseDelta", Stringf("%d", static_cast<int>(delta)));
				FireEvent("MouseWheelScrolled", args);
			}
			return 0;
		}
	}

	// Send back to Windows any unhandled/unconsumed messages we want other apps to see (e.g. play/pause in music apps, etc.)
	return DefWindowProc(windowHandle, wmMessageCode, wParam, lParam);
}


Window::Window(WindowConfig const& config)
	:m_config(config)
{
	s_mainWindow = this;
}

Window::~Window()
{
}

void Window::Startup()
{
	CreateOSWindow();
}

void Window::BeginFrame()
{
	RunMessagePump();
}

void Window::EndFrame()
{
}

void Window::Shutdown()
{
}

WindowConfig const& Window::GetConfig() const
{
	return m_config;
}

void* Window::GetDisplayContext() const
{
	return m_displayContext;
}

void* Window::GetHwnd() const
{
	return static_cast<HWND>(m_windowHandle);
}

IntVec2 Window::GetClientDimensions() const
{
	HWND windowHandle = static_cast<HWND>(m_windowHandle);
	RECT clientRect;
	if (GetClientRect(windowHandle, &clientRect))
	{
		return IntVec2(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
	}
	return IntVec2::ZERO;
}

Vec2 Window::GetNormalizedMouseUV() const
{
	HWND windowHandle = static_cast<HWND>(m_windowHandle);
	POINT cursorCoords;
	RECT clientRect;
	::GetCursorPos(&cursorCoords);
	::ScreenToClient(windowHandle, &cursorCoords);
	::GetClientRect(windowHandle, &clientRect);
	float cursorX = static_cast<float>(cursorCoords.x) / static_cast<float>(clientRect.right);
	float cursorY = static_cast<float>(cursorCoords.y) / static_cast<float>(clientRect.bottom);
	return Vec2(cursorX, 1.f - cursorY);
}


void Window::CreateOSWindow()
{
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	HMODULE applicationInstanceHandle = ::GetModuleHandle(NULL);
	float clientAspect = m_config.m_aspectRatio;

	// Define a window style/class
	WNDCLASSEX windowClassDescription;
	memset(&windowClassDescription, 0, sizeof(windowClassDescription));
	windowClassDescription.cbSize = sizeof(windowClassDescription);
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast<WNDPROC>(WindowsMessageHandlingProcedure); // Register our Windows message-handling function
	windowClassDescription.hInstance = GetModuleHandle(NULL);
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT("Simple Window Class");
	RegisterClassEx(&windowClassDescription);

	// #SD1ToDo: Add support for fullscreen mode (requires different window style flags than windowed mode)
	DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_OVERLAPPED;
	DWORD windowStyleExFlags = WS_EX_APPWINDOW;
	RECT windowRect = {};

	// Get desktop rect, dimensions, aspect
	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect(desktopWindowHandle, &desktopRect);
	float desktopWidth = (float)(desktopRect.right - desktopRect.left);
	float desktopHeight = (float)(desktopRect.bottom - desktopRect.top);
	float desktopAspect = desktopWidth / desktopHeight;

	// Set to fullscreen if flag is true, if not set to windowed
	if (m_config.m_isWindowFullScreen)
	{
		windowStyleFlags = WS_POPUP;
		windowStyleExFlags = WS_EX_APPWINDOW;

		windowRect.left = 0;
		windowRect.top = 0;
		windowRect.right = (int)desktopWidth;
		windowRect.bottom = (int)desktopHeight;

		m_config.m_clientWidth = (int)desktopWidth;
		m_config.m_clientHeight = (int)desktopHeight;
		m_config.m_aspectRatio = desktopWidth / desktopHeight;
	}
	else
	{
		// Calculate maximum client size (as some % of desktop size)
		constexpr float maxClientFractionOfDesktop = 0.90f;
		float clientWidth = desktopWidth * maxClientFractionOfDesktop;
		float clientHeight = desktopHeight * maxClientFractionOfDesktop;
		if (clientAspect > desktopAspect)
		{
			// Client window has a wider aspect than desktop; shrink client height to match its width
			clientHeight = clientWidth / clientAspect;
		}
		else
		{
			// Client window has a taller aspect than desktop; shrink client width to match its height
			clientWidth = clientHeight * clientAspect;
		}

		// Calculate client rect bounds by centering the client area
		float clientMarginX = 0.5f * (desktopWidth - clientWidth);
		float clientMarginY = 0.5f * (desktopHeight - clientHeight);
		RECT clientRect;
		clientRect.left = (int)clientMarginX;
		clientRect.right = clientRect.left + (int)clientWidth;
		clientRect.top = (int)clientMarginY;
		clientRect.bottom = clientRect.top + (int)clientHeight;

		// Calculate the outer dimensions of the physical window, including frame et. al.
		windowRect = clientRect;
		AdjustWindowRectEx(&windowRect, windowStyleFlags, FALSE, windowStyleExFlags);

		m_config.m_clientWidth = (int)clientWidth;
		m_config.m_clientHeight = (int)clientHeight;
	}

	WCHAR windowTitle[1024];
	MultiByteToWideChar(GetACP(), 0, m_config.m_windowTitle.c_str(), -1, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
	m_windowHandle = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		(HINSTANCE)applicationInstanceHandle,
		NULL);

    ShowWindow(static_cast<HWND>(m_windowHandle), SW_SHOW);
    SetForegroundWindow(static_cast<HWND>(m_windowHandle));
    SetFocus(static_cast<HWND>(m_windowHandle));

    m_displayContext = GetDC(static_cast<HWND>(m_windowHandle));

    HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
    SetCursor(cursor);
}

void Window::RunMessagePump()
{
	MSG queuedMessage;
	for (;; )
	{
		BOOL const wasMessagePresent = PeekMessage(&queuedMessage, NULL, 0, 0, PM_REMOVE);
		if (!wasMessagePresent)
		{
			break;
		}

		TranslateMessage(&queuedMessage);
		DispatchMessage(&queuedMessage); // This tells Windows to call our "WindowsMessageHandlingProcedure" (a.k.a. "WinProc") function
	}
}

