#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Core/Timer.hpp"
#include "Engine/Core/EngineCommon.h"
#include "Engine/Input/InputSystem.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Math/MathUtils.h"
// -----------------------------------------------------------------------------
DevConsole* g_theDevConsole = nullptr;
// -----------------------------------------------------------------------------
const Rgba8 DevConsole::ERROR_MAJOR(Rgba8(255, 0, 0, 255));
const Rgba8 DevConsole::WARNING(Rgba8(255, 255, 0, 255));
const Rgba8 DevConsole::INFO_MAJOR(Rgba8(255, 165, 0, 255));
const Rgba8 DevConsole::INFO_MINOR(Rgba8(50, 205, 50, 255));
const Rgba8 DevConsole::INPUT_TEXT(Rgba8(255, 0, 255));
const Rgba8 DevConsole::INPUT_INSERTION_POINT(Rgba8(255, 255, 255, 255));
// -----------------------------------------------------------------------------
DevConsole::DevConsole(DevConsoleConfig const& config)
	:m_config(config)
{
}

DevConsole::~DevConsole()
{
}

void DevConsole::Startup()
{
	m_lines.reserve(10000);

	SubscribeEventCallbackFunction("KeyPressed", Event_KeyPressed);
	SubscribeEventCallbackFunction("CharInput", Event_CharInput);
	SubscribeEventCallbackFunction("EchoCommand", Event_EchoCommand);
	SubscribeEventCallbackFunction("Help", Command_Help);
	SubscribeEventCallbackFunction("Clear", Command_Clear);
	m_insertionPointBlinkTimer = new Timer(0.5);

	m_insertionPointBlinkTimer->Start();
}

void DevConsole::Shutdown()
{
	delete m_insertionPointBlinkTimer;
	m_insertionPointBlinkTimer = nullptr;
}

void DevConsole::BeginFrame()
{
	++m_frameNumber;

	while (m_insertionPointBlinkTimer->DecrementPeriodIfElapsed())
	{
		m_insertionPointVisible = !m_insertionPointVisible;
	}
}

void DevConsole::EndFrame()
{
}

void DevConsole::Execute(std::string const& consoleCommandText, bool echoCommand)
{
	// Check command history
	if (echoCommand)
	{
		AddLine(DevConsole::INFO_MINOR, consoleCommandText);
	}
	m_commandHistory.push_back(consoleCommandText);
	m_historyIndex = static_cast<int>(m_inputText.size()) - 1;

	Strings commandParts = SplitStringOnDelimiter(consoleCommandText, ' ');
	if (commandParts.empty())
	{
		return;
	}

	std::string eventName = commandParts[0];
	
	EventArgs args;
	for (int i = 1; i < (int)commandParts.size(); ++i) // Skip zero command and start at eventargs
	{
		std::string const& argKeyEqualsValue = commandParts[i];
		Strings keyValue = SplitStringOnDelimiter(argKeyEqualsValue, '=');

		if (keyValue.size() != 2) 
		{
			g_theDevConsole->AddLine(DevConsole::ERROR_MAJOR, "Error: Arguments must be key=value!");
			return;
		}

		args.SetValue(keyValue[0], keyValue[1]);
	}
	g_theEventSystem->FireEvent(eventName, args);
}

void DevConsole::AddLine(Rgba8 const& color, std::string const& text)
{
	std::scoped_lock<std::mutex> lock(m_devConsoleMutex);

	Strings separateLines = SplitStringOnDelimiter(text, '\n');
	for (int i = 0; i < (int)separateLines.size(); ++i)
	{
		DevConsoleLine newLine;
		newLine.m_text = separateLines[i];
		newLine.m_color = color;
		newLine.m_frameNumberPrinted = m_frameNumber;
		newLine.m_timePrinted = GetCurrentTimeSeconds();
		m_lines.push_back(newLine);
	}
}

void DevConsole::Render(AABB2 const& bounds, Renderer* rendererOverride) const
{
	// Renderer validation
	Renderer* renderer = rendererOverride ? rendererOverride : m_config.m_renderer;
	if (!renderer)
	{
		static bool hasShownError = false;
		if (!hasShownError)
		{
			ERROR_RECOVERABLE("Call to DevConsole::Render() without a Renderer pointer");
			hasShownError = true;
		}
		return;
	}

	// Font validation
	BitmapFont* fontFileName = renderer->CreateOrGetBitmapFont(m_config.m_fontName.c_str());
	if (!fontFileName)
	{
		ERROR_RECOVERABLE("Could not find Bitmap font!");
		return;
	}

	// DevConsoleMode dispatcher
	switch (m_mode)
	{
		case DevConsoleMode::OPEN_FULL:
		{
			Render_OpenFull(bounds, *renderer, *fontFileName, m_config.m_fontAspect);
			break;
		}
		case DevConsoleMode::HIDDEN:
		{
			return;
		}
	}
}

DevConsoleMode DevConsole::GetMode() const
{
	return m_mode;
}

void DevConsole::SetMode(DevConsoleMode mode)
{
	m_mode = mode;
}

void DevConsole::ToggleMode(DevConsoleMode mode)
{
	if (m_mode == mode)
	{
		SetMode(DevConsoleMode::HIDDEN); // Toggle off
	}
	else
	{
		SetMode(mode); // Toggle on (switch different mode on)
	}
}

bool DevConsole::Event_EchoCommand(EventArgs& args)
{
	std::string message = args.GetValue("Echo", "");
	if (message.empty())
	{
		g_theDevConsole->AddLine(DevConsole::ERROR_MAJOR, "EchoCommand is Echo=text");
		return false;
	}

	g_theDevConsole->AddLine(Rgba8::WHITE, message);
	return true;
}

bool DevConsole::Event_KeyPressed(EventArgs& args)
{
	unsigned char keyCode = static_cast<unsigned char>(args.GetValue("KeyCode", -1));
	if (keyCode == KEYCODE_TILDE)
	{
		return false;
	}
	if (g_theDevConsole->m_mode == DevConsoleMode::OPEN_FULL)
	{
		g_theDevConsole->m_insertionPointVisible = true;

		if (keyCode == KEYCODE_ESC)
		{
			g_theDevConsole->m_insertionPointPosition = 0;
			if (g_theDevConsole->m_inputText == "")
			{
				g_theDevConsole->m_mode = DevConsoleMode::HIDDEN;
			}
			else
			{
				g_theDevConsole->m_inputText = "";
			}
		}

		if (keyCode == KEYCODE_ENTER)
		{
			g_theDevConsole->m_insertionPointPosition = 0;
			if (g_theDevConsole->m_inputText == "")
			{
				g_theDevConsole->m_mode = DevConsoleMode::HIDDEN;
			}
			else
			{
				g_theDevConsole->Execute(g_theDevConsole->m_inputText);
				g_theDevConsole->m_inputText = "";
			}
		}

		if (keyCode == KEYCODE_UPARROW)
		{
			if (g_theDevConsole->m_inputText != "")
			{
				g_theDevConsole->m_historyIndex--;
			}
			if (g_theDevConsole->m_historyIndex < 0)
			{
				g_theDevConsole->m_historyIndex = 0;
			}
			if (g_theDevConsole->m_historyIndex >= static_cast<int>(g_theDevConsole->m_commandHistory.size()))
			{
				g_theDevConsole->m_historyIndex = static_cast<int>(g_theDevConsole->m_commandHistory.size()) - 1;
			}
			else
			{
				g_theDevConsole->m_inputText = g_theDevConsole->m_commandHistory[g_theDevConsole->m_historyIndex];
			}
		}

		if (keyCode == KEYCODE_DOWNARROW)
		{
			g_theDevConsole->m_historyIndex++;

			if (g_theDevConsole->m_historyIndex < 0)
			{
				g_theDevConsole->m_historyIndex = 0;
			}
			if (g_theDevConsole->m_historyIndex >= static_cast<int>(g_theDevConsole->m_commandHistory.size()))
			{
				g_theDevConsole->m_historyIndex = static_cast<int>(g_theDevConsole->m_commandHistory.size() - 1);
			}
			else
			{
				g_theDevConsole->m_inputText = g_theDevConsole->m_commandHistory[g_theDevConsole->m_historyIndex];
			}
		}

		if (keyCode == KEYCODE_RIGHTARROW)
		{
			if (g_theDevConsole->m_insertionPointPosition < static_cast<int>(g_theDevConsole->m_inputText.length()))
			{
				g_theDevConsole->m_insertionPointPosition++;
			}
		}

		if (keyCode == KEYCODE_LEFTARROW)
		{
			if (g_theDevConsole->m_insertionPointPosition > 0)
			{
				g_theDevConsole->m_insertionPointPosition--;
			}
		}

		if (keyCode == KEYCODE_HOME)
		{
			g_theDevConsole->m_insertionPointPosition = 0;
		}
		if (keyCode == KEYCODE_END)
		{
			g_theDevConsole->m_insertionPointPosition = static_cast<int>(g_theDevConsole->m_inputText.length());
		}

		if (keyCode == KEYCODE_BACKSPACE)
		{
			if (g_theDevConsole->m_inputText != "" && g_theDevConsole->m_insertionPointPosition > 0)
			{
				g_theDevConsole->m_insertionPointPosition--;
				g_theDevConsole->m_inputText.erase(static_cast<size_t>(g_theDevConsole->m_insertionPointPosition), 1);
			}
		}
		if (keyCode == KEYCODE_DELETE)
		{
			if (g_theDevConsole->m_inputText != "")
			{
				g_theDevConsole->m_inputText.erase(static_cast<size_t>(g_theDevConsole->m_insertionPointPosition), 1);
			}
		}
		return true;
	}
	return false;
}

bool DevConsole::Event_CharInput(EventArgs& args)
{
	unsigned char keyCode = static_cast<unsigned char>(args.GetValue("CharCode", -1));
	std::string inputChar(1, keyCode);

	if (inputChar == "~" || inputChar == "`" || keyCode < 32 || keyCode > 126)
	{
		return false;
	}

	if (g_theDevConsole->m_mode == DevConsoleMode::OPEN_FULL)
	{
		g_theDevConsole->m_insertionPointVisible = true;
		g_theDevConsole->m_inputText = g_theDevConsole->m_inputText.insert(g_theDevConsole->m_insertionPointPosition, 1, keyCode);
		g_theDevConsole->m_insertionPointPosition++;
		return true;
	}

	return false;
}

bool DevConsole::Command_Clear(EventArgs& args)
{
	UNUSED(args);

	std::scoped_lock<std::mutex> lock(g_theDevConsole->m_devConsoleMutex);

	if (g_theDevConsole != nullptr)
	{
		g_theDevConsole->m_lines.clear();
	}
	return true;
}

bool DevConsole::Command_Help(EventArgs& args)
{
	UNUSED(args);

	// Check if there is an eventsystem and get all registered commands if there is.
	if (g_theEventSystem != nullptr)
	{
		std::vector<std::string> registeredCommands = g_theEventSystem->GetAllRegisteredCommands();
		for (std::vector<std::string>::iterator found = registeredCommands.begin(); found != registeredCommands.end(); ++found)
		{
			g_theDevConsole->AddLine(Rgba8::CYAN, *found);
		}
	}
	return true;
}

void DevConsole::Render_OpenFull(AABB2 const& bounds, Renderer& renderer, BitmapFont& font, float fontAspect) const
{
	// Begin camera
	renderer.BeginCamera(*m_config.m_camera);

	renderer.SetModelConstants();
	renderer.SetBlendMode(BlendMode::ALPHA);
	renderer.SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	renderer.SetDepthMode(DepthMode::DISABLED);

	// First draw call: Draw console box
	std::vector<Vertex_PCU> consoleBoxVerts;
	Rgba8 consoleColor = Rgba8(0, 0, 0, 200);
	AddVertsForAABB2D(consoleBoxVerts, bounds, consoleColor);
	renderer.BindTexture(nullptr);
	renderer.DrawVertexArray(consoleBoxVerts);
	
	float fontHeight = bounds.GetDimensions().y / m_config.m_linesOnScreen;
	float consoleBoxWidth = bounds.GetDimensions().x;
	float textStart = bounds.m_mins.y + fontHeight;
	float inputTextStart = bounds.m_mins.y;

	std::scoped_lock<std::mutex> lock(g_theDevConsole->m_devConsoleMutex);

	int startLineIndex = 0;
	if (static_cast<int>(m_lines.size()) > static_cast<int>(m_config.m_linesOnScreen))
	{
		startLineIndex = static_cast<int>(m_lines.size()) - static_cast<int>(m_config.m_linesOnScreen);
	}

	std::vector<Vertex_PCU> consoleLineVerts;
	// For loop through the lines
	for (int lineIndex = (int)m_lines.size() - 1; lineIndex >= 0; --lineIndex)
	{
		DevConsoleLine const& text = m_lines[lineIndex];

		// Out of bounds check to reduce draw calls.
		float lineBottom = textStart + fontHeight;
		if (lineBottom < bounds.m_mins.y || textStart > bounds.m_maxs.y)
		{
			continue;
		}

		Rgba8 textColor = text.m_color;
		AABB2 lineBounds = AABB2(Vec2(bounds.m_mins.x, textStart), Vec2(bounds.m_maxs.x, textStart + fontHeight));

		// Each line should have its own textbox
		font.AddVertsForTextInBox2D(consoleLineVerts, text.m_text, lineBounds, fontHeight, textColor, fontAspect, Vec2(0.f, 0.f));
		textStart += fontHeight;
	}

	// Adding text box for input line.
	AABB2 inputTextBounds = AABB2(Vec2(bounds.m_mins.x, inputTextStart), Vec2(bounds.m_maxs.x, inputTextStart + fontHeight));
	font.AddVertsForTextInBox2D(consoleLineVerts, m_inputText, inputTextBounds, fontHeight, INPUT_TEXT, fontAspect, Vec2(0.f, 0.f));

	// Second draw call: Draw textboxes
	renderer.BindTexture(&font.GetTexture());
	renderer.DrawVertexArray(consoleLineVerts);

	// Draw and clamp Insertion Point
	if (m_insertionPointVisible)
	{
		// Clamp to console bounds
		float clampedInsertionPointX = static_cast<float>(m_insertionPointPosition) * fontHeight * fontAspect;
		clampedInsertionPointX = GetClamped(clampedInsertionPointX, bounds.m_mins.x, consoleBoxWidth);

		// Draw insertion point
		std::vector<Vertex_PCU> insertionVerts;
		AddVertsForLineSegment2D(insertionVerts, Vec2(clampedInsertionPointX, 0.f), Vec2(clampedInsertionPointX, fontHeight), 2.f, INPUT_INSERTION_POINT);
		renderer.BindTexture(nullptr);
		renderer.DrawVertexArray(insertionVerts);
	}
	// End camera
	renderer.EndCamera(*m_config.m_camera);
}
