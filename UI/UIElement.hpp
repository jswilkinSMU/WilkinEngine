#pragma once
#include "Engine/Math/AABB2.h"
#include "Engine/Core/EngineCommon.h"
#include "Engine/Core/VertexUtils.h"
#include "Engine/Renderer/Renderer.h"
#include <string>
// -----------------------------------------------------------------------------
class UIElement
{
public:
	UIElement(std::string widgetName, AABB2 bounds);
	virtual ~UIElement() = default;

	virtual void Update(float deltaseconds) = 0;
	virtual void Render(Renderer* renderer) const = 0;
	virtual void OnClick() = 0;

	bool IsPointInside(Vec2 const& point) const;
	void SetHovered(bool isHovered);
	bool IsHovered() const;

	std::string GetElementName() const;
	AABB2 GetElementBounds() const;

protected:
	std::string m_name   = "default";
	AABB2		m_bounds = AABB2::ZERO_TO_ONE;
	bool		m_isHovered = false;
};