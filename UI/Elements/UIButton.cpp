#include "Engine/UI/Elements/UIButton.hpp"
#include "Engine/Core/Vertex_PCU.h"
#include "Engine/Renderer/BitmapFont.hpp"

UIButton::UIButton(std::string name, AABB2 bounds, std::string text, BitmapFont* font)
	:UIElement(name, bounds),
	 m_text(text),
	 m_font(font)
{
}

void UIButton::Update(float deltaSeconds)
{
	UNUSED(deltaSeconds);
}

void UIButton::Render(Renderer* renderer) const
{
	// Draw box
	std::vector<Vertex_PCU> shapeVerts;
	Rgba8 buttonColor = m_isHovered ? m_hoverColor : m_buttonColor;
	AddVertsForAABB2D(shapeVerts, m_bounds, buttonColor);
	renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	renderer->BindShader(nullptr);
	renderer->BindTexture(nullptr);
	renderer->DrawVertexArray(shapeVerts);

	// Draw text
	std::vector<Vertex_PCU> textVerts;
	if (m_font)
	{
		m_font->AddVertsForTextInBox2D(textVerts, m_text, m_bounds, 16.f, m_textColor, 1.f, Vec2(0.5f, 0.3f));
	}
	renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	renderer->BindShader(nullptr);
	renderer->BindTexture(&m_font->GetTexture());
	renderer->DrawVertexArray(textVerts);
}

void UIButton::OnClick()
{
	if (m_onClick)
	{
		m_onClick();
	}
}

void UIButton::SetOnClickCallback(std::function<void()> callback)
{
	m_onClick = callback;
}

void UIButton::SetButtonBackgroundColor(Rgba8 buttonColor)
{
	m_buttonColor = buttonColor;
}

void UIButton::SetButtonHoverColor(Rgba8 hoverColor)
{
	m_hoverColor = hoverColor;
}

void UIButton::SetButtonTextColor(Rgba8 textColor)
{
	m_textColor = textColor;
}


