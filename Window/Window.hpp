#pragma once
#include "Engine/Math/IntVec2.h"
#include "Engine/Math/Vec2.hpp"
#include <string>
// -------------------------------------------------------------------------------------------------------
class InputSystem;
//--------------------------------------------------------------------------------------------------------
struct WindowConfig
{
	float		 m_aspectRatio = (16.f / 9.f);
	int			 m_clientWidth = 1280;
	int			 m_clientHeight = 720;
	InputSystem* m_inputSystem = nullptr;
	std::string  m_windowTitle = "Unnamed Application";
	bool		 m_isWindowFullScreen = false;
};
//--------------------------------------------------------------------------------------------------------
class Window
{
public:
	Window(WindowConfig const& config);
	~Window();
	void Startup();
	void BeginFrame();
	void EndFrame();
	void Shutdown();

	WindowConfig const& GetConfig() const;
	void* GetDisplayContext() const;
	void* GetHwnd() const;
	IntVec2 GetClientDimensions() const;
	static Window* s_mainWindow;
	Vec2 GetNormalizedMouseUV() const;
private:
	//Private (internal) member functions will go here
	//void CreateOSWindow(void* applicationInstanceHandle, float clientAspect);
	void CreateOSWindow();
	void RunMessagePump();

private:
	//Private (internal) member functions will go here
	WindowConfig m_config;
	void* m_windowHandle = nullptr; // Actually a Windows HWND
	void* m_displayContext = nullptr; // Actually a Windows HDC
};