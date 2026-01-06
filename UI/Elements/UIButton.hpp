#pragma once
#include "Engine/UI/UIElement.hpp"
#include "Engine/Core/Rgba8.h"
#include <functional>
// -----------------------------------------------------------------------------
class BitmapFont;
// -----------------------------------------------------------------------------
class UIButton : public UIElement
{
public:
	UIButton(std::string name, AABB2 bounds, std::string text, BitmapFont* font);
	void Update(float deltaSeconds) override;
	void Render(Renderer* renderer) const override;
	void OnClick() override;
	void SetOnClickCallback(std::function<void()> callback);
	
	void SetButtonBackgroundColor(Rgba8 buttonColor);
	void SetButtonHoverColor(Rgba8 hoverColor);
	void SetButtonTextColor(Rgba8 textColor);
// -----------------------------------------------------------------------------
private:
	std::string m_text;
	BitmapFont* m_font = nullptr;
	std::function<void()> m_onClick;
	Rgba8 m_buttonColor  = Rgba8::DARKGRAY;
	Rgba8 m_hoverColor = Rgba8(0, 0, 0, 125);
	Rgba8 m_textColor  = Rgba8(255, 255, 255, 255);
};