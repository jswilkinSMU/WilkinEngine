#include "Engine/UI/Elements/UIBorder.hpp"

UIBorder::UIBorder(std::string borderName, AABB2 bounds, Rgba8 borderColor, float borderThickness)
	:UIElement(borderName, bounds),
	 m_color(borderColor),
	 m_thickness(borderThickness)
{
}

void UIBorder::Update(float deltaSeconds)
{
	UNUSED(deltaSeconds);
}

void UIBorder::Render(Renderer* renderer) const
{
	switch (m_style)
	{
		case BorderStyle::SOLID:
		{
			RenderSolidBorder(renderer);
			break;
		}
		case BorderStyle::DASHED:
		{
			RenderDashedBorder(renderer);
			break;
		}
		case BorderStyle::ROUNDED:
		{
			RenderRoundedBorder(renderer);
			break;
		}
	}
}

void UIBorder::SetBorderColor(Rgba8 borderColor)
{
	m_color = borderColor;
}

void UIBorder::SetBorderThickness(float thickness)
{
	m_thickness = thickness;
}

void UIBorder::SetBorderStyle(BorderStyle style)
{
	m_style = style;
}

void UIBorder::SetDashLength(float dashLength)
{
	m_dashLength = dashLength;
}

void UIBorder::SetGapLength(float gapLength)
{
	m_gapLength = gapLength;
}

void UIBorder::RenderSolidBorder(Renderer* renderer) const
{
	Vec2 borderMins = m_bounds.m_mins;
	Vec2 borderMaxs = m_bounds.m_maxs;

	std::vector<Vertex_PCU> borderVerts;

	// Draw bottom border line
	AddVertsForLineSegment2D(borderVerts, borderMins, Vec2(borderMaxs.x, borderMins.y), m_thickness, m_color);

	// Draw left border line
	AddVertsForLineSegment2D(borderVerts, borderMins, Vec2(borderMins.x, borderMaxs.y), m_thickness, m_color);

	// Draw right border line
	AddVertsForLineSegment2D(borderVerts, Vec2(borderMaxs.x, borderMins.y), borderMaxs, m_thickness, m_color);

	// Draw top border line
	AddVertsForLineSegment2D(borderVerts, Vec2(borderMins.x, borderMaxs.y), borderMaxs, m_thickness, m_color);

	renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	renderer->BindShader(nullptr);
	renderer->BindTexture(nullptr);
	renderer->DrawVertexArray(borderVerts);
}

void UIBorder::RenderDashedBorder(Renderer* renderer) const
{
	Vec2 borderMins = m_bounds.m_mins;
	Vec2 borderMaxs = m_bounds.m_maxs;

	std::vector<Vertex_PCU> borderVerts;

	// Draw bottom border line
	AddVertsForDashedLine2D(borderVerts, borderMins, Vec2(borderMaxs.x, borderMins.y), m_dashLength, m_gapLength, m_thickness, m_color);

	// Draw left border line
	AddVertsForDashedLine2D(borderVerts, borderMins, Vec2(borderMins.x, borderMaxs.y), m_dashLength, m_gapLength, m_thickness, m_color);

	// Draw right border line
	AddVertsForDashedLine2D(borderVerts, Vec2(borderMaxs.x, borderMins.y), borderMaxs, m_dashLength, m_gapLength, m_thickness, m_color);

	// Draw top border line
	AddVertsForDashedLine2D(borderVerts, Vec2(borderMins.x, borderMaxs.y), borderMaxs, m_dashLength, m_gapLength, m_thickness, m_color);

	renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	renderer->BindShader(nullptr);
	renderer->BindTexture(nullptr);
	renderer->DrawVertexArray(borderVerts);
}

void UIBorder::RenderRoundedBorder(Renderer* renderer) const
{
	float borderRadius = m_thickness * 0.5f;
	float arcDegrees = 90.f;
	Vec2  bottomLeft = m_bounds.m_mins;
	Vec2  bottomRight = Vec2(m_bounds.m_maxs.x, m_bounds.m_mins.y);
	Vec2  topRight = m_bounds.m_maxs;
	Vec2  topLeft = Vec2(m_bounds.m_mins.x, m_bounds.m_maxs.y);

	std::vector<Vertex_PCU> borderVerts;

	AddVertsForArc2D(borderVerts, bottomLeft, borderRadius, 90.f, arcDegrees, m_color);
	AddVertsForArc2D(borderVerts, bottomRight, borderRadius, 0.f, arcDegrees, m_color);
	AddVertsForArc2D(borderVerts, topRight, borderRadius, 270.f, arcDegrees, m_color);
	AddVertsForArc2D(borderVerts, topLeft, borderRadius, 180.f, arcDegrees, m_color);

	RenderSolidBorder(renderer);

	renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	renderer->BindShader(nullptr);
	renderer->BindTexture(nullptr);
	renderer->DrawVertexArray(borderVerts);
}
