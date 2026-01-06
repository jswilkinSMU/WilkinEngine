#pragma once
#include "Engine/UI/UIElement.hpp"
#include "Engine/Core/Rgba8.h"
// -----------------------------------------------------------------------------
enum class BorderStyle
{
	SOLID,
	DASHED,
	ROUNDED
};
// -----------------------------------------------------------------------------
class UIBorder : public UIElement
{
public:
	UIBorder(std::string borderName, AABB2 bounds, Rgba8 borderColor = Rgba8::WHITE, float borderThickness = 1.f);
	void Update(float deltaSeconds) override;
	void Render(Renderer* renderer) const override;
	void OnClick() override {}

	void SetBorderColor(Rgba8 borderColor);
	void SetBorderThickness(float thickness);
	void SetBorderStyle(BorderStyle style);
	void SetDashLength(float dashLength);
	void SetGapLength(float gapLength);
// -----------------------------------------------------------------------------
private:
	void RenderSolidBorder(Renderer* renderer) const;
	void RenderDashedBorder(Renderer* renderer) const;
	void RenderRoundedBorder(Renderer* renderer) const;
// -----------------------------------------------------------------------------
private:
	BorderStyle m_style = BorderStyle::SOLID;
	Rgba8 m_color = Rgba8::WHITE;
	float m_thickness = 1.f;
	float m_dashLength = 2.f;
	float m_gapLength = 3.f;
};
// -----------------------------------------------------------------------------