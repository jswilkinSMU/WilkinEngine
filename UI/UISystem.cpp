#include "Engine/UI/UISystem.hpp"
#include "Engine/UI/UIElement.hpp"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Input/InputSystem.h"
#include "Engine/Core/EngineCommon.h"

UISystem::UISystem(UISystemConfig const& config)
	:m_uiConfig(config)
{
}

UISystem::~UISystem()
{
}

void UISystem::Startup()
{
}

void UISystem::Shutdown()
{
	Clear();
}

void UISystem::BeginFrame()
{
}

void UISystem::EndFrame()
{
}

void UISystem::Update(float deltaseconds, Vec2 const& cursorWorldPos)
{
	for (int elementIndex = 0; elementIndex < static_cast<int>(m_elements.size()); ++elementIndex)
	{
		UIElement*& element = m_elements[elementIndex];
		if (element)
		{
			bool isHovered = element->IsPointInside(cursorWorldPos);
			element->SetHovered(isHovered);
			element->Update(deltaseconds);
		}
	}
}

void UISystem::Render() const
{
	for (int elementIndex = 0; elementIndex < static_cast<int>(m_elements.size()); ++elementIndex)
	{
		UIElement const* element = m_elements[elementIndex];
		if (element)
		{
			element->Render(m_uiConfig.m_renderer);
		}
	}
}

void UISystem::Clear()
{
	for (int elementIndex = 0; elementIndex < static_cast<int>(m_elements.size()); ++elementIndex)
	{
		UIElement*& element = m_elements[elementIndex];
		if (element)
		{
			delete element;
			element = nullptr;
		}
	}
	m_elements.clear();
}

void UISystem::AddElement(UIElement* widget)
{
	m_elements.push_back(widget);
}

void UISystem::HandleMouseClick(Vec2 const& mousePosition)
{
	for (int elementIndex = 0; elementIndex < static_cast<int>(m_elements.size()); ++elementIndex)
	{
		UIElement*& element = m_elements[elementIndex];
		if (element->IsPointInside(mousePosition))
		{
			element->OnClick();
			break;
		}
	}
}
