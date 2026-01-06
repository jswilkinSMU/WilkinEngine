#include "Engine/UI/UIElement.hpp"

UIElement::UIElement(std::string widgetName, AABB2 bounds)
	:m_name(widgetName),
	 m_bounds(bounds)
{
}

bool UIElement::IsPointInside(Vec2 const& point) const
{
	return m_bounds.IsPointInside(point);
}

void UIElement::SetHovered(bool isHovered)
{
	m_isHovered = isHovered;
}

bool UIElement::IsHovered() const
{
	return m_isHovered;
}

std::string UIElement::GetElementName() const
{
	return m_name;
}

AABB2 UIElement::GetElementBounds() const
{
	return m_bounds;
}
