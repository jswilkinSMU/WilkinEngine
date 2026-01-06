#pragma once
#include "Engine/Math/Vec2.hpp"
#include <string>
#include <vector>
// -----------------------------------------------------------------------------
class InputSystem;
class Renderer;
class BitmapFont;
class UIElement;
// -----------------------------------------------------------------------------
struct UISystemConfig
{
	InputSystem* m_inputSystem = nullptr;
	Renderer* m_renderer = nullptr;
	std::string m_fontName = "SquirrelFixedFont";
};
// -----------------------------------------------------------------------------
class UISystem
{
public:
	UISystem(UISystemConfig const& config);
	~UISystem();
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();

	void Update(float deltaseconds, Vec2 const& cursorWorldPos);
	void Render() const;
	void Clear();

	void AddElement(UIElement* widget);
	void HandleMouseClick(Vec2 const& mousePosition);

private:
	UISystemConfig m_uiConfig;
	std::vector <UIElement*> m_elements;
};