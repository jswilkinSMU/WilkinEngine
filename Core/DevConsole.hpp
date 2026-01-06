#pragma once
#include "Engine/Renderer/Renderer.h"
#include "Engine/Core/EventSystem.hpp"
#include <mutex>
// -----------------------------------------------------------------------------
class Renderer;
class Camera;
class BitmapFont;
class Timer;
struct AABB2;
// -----------------------------------------------------------------------------
struct DevConsoleConfig
{
	Renderer* m_renderer = nullptr;
	Camera* m_camera = nullptr;
	std::string  m_fontName = "Unnamed Font";
	float m_fontAspect = 0.7f;
	float m_linesOnScreen = 34.5f;
	int m_maxCommandHistory = 128;
};
// -----------------------------------------------------------------------------
enum class DevConsoleMode
{
	HIDDEN,
	OPEN_FULL,
	NUM_MODES
};
// -----------------------------------------------------------------------------
struct DevConsoleLine
{
	Rgba8 m_color;
	std::string m_text;
	int m_frameNumberPrinted;
	double m_timePrinted;
};
// -----------------------------------------------------------------------------
class DevConsole
{
public:
	DevConsole(DevConsoleConfig const& config);
	~DevConsole();
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();

	void Execute(std::string const& consoleCommandText, bool echoCommand = true);
	void AddLine(Rgba8 const& color, std::string const& text); // Set and push back
	void Render(AABB2 const& bounds, Renderer* rendererOverride = nullptr) const; // Render devConsole with BitmapFont

	DevConsoleMode GetMode() const;
	void SetMode(DevConsoleMode mode);
	void ToggleMode(DevConsoleMode mode);

	static const Rgba8 ERROR_MAJOR;
	static const Rgba8 WARNING;
	static const Rgba8 INFO_MAJOR;
	static const Rgba8 INFO_MINOR;
	static const Rgba8 INPUT_TEXT;
	static const Rgba8 INPUT_INSERTION_POINT;

	// Handle text input and printing
	static bool Event_EchoCommand(EventArgs& args);

	// Handle key input, typing and insertion point is handled here.
	static bool Event_KeyPressed(EventArgs& args);

	// Handle char input by appending valid characters to our current input line.
	static bool Event_CharInput(EventArgs& args);

	// Clear all lines of text.
	static bool Command_Clear(EventArgs& args);

	// Display all currently registered commands in the event system.
	static bool Command_Help(EventArgs& args);

protected:
	void Render_OpenFull(AABB2 const& bounds, Renderer& renderer, BitmapFont& font, float fontAspect = 1.f) const;

protected:
	DevConsoleConfig			m_config;
	std::vector<DevConsoleLine> m_lines; //#ToDo: Support a max limited number of lines (e.g. fixed circular buffer)
	std::atomic<int>			m_frameNumber = 0;
	std::atomic<DevConsoleMode>	m_mode = DevConsoleMode::HIDDEN;

	// Our current line of input text.
	std::string m_inputText;

	// Index of the Insertion point in our current input text.
	int m_insertionPointPosition = 0;

	// True if our insertion point is currently in the visible phase of blinking.
	bool m_insertionPointVisible = true;

	// Timer for controlling insertion point visibility.
	Timer* m_insertionPointBlinkTimer = nullptr;

	// History of all commands executed.
	std::vector<std::string> m_commandHistory;

	// Our current index in our history of commands as we are scrolling.
	std::atomic<int> m_historyIndex = -1;

	// DevConsole's stored internal mutex
	mutable std::mutex m_devConsoleMutex;
};