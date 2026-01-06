#include "Engine/Renderer/SpriteAnimDefinition.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Core/EngineCommon.h"
#include "Engine/Math/MathUtils.h"

SpriteAnimDefinition::SpriteAnimDefinition(SpriteSheet const& sheet, int startSpriteIndex, int endSpriteIndex, float framesPerSecond, SpriteAnimPlaybackType playbackType)
	:m_spriteSheet(sheet), 
	 m_startSpriteIndex(startSpriteIndex), 
	 m_endSpriteIndex(endSpriteIndex), 
	 m_framesPerSecond(framesPerSecond), 
	 m_playbackType(playbackType)
{
}

SpriteDefinition const& SpriteAnimDefinition::GetSpriteDefAtTime(float seconds) const
{
	int totalFrames = m_endSpriteIndex - m_startSpriteIndex + 1;
	float frameDuration = (seconds / m_framesPerSecond);
	int animationDuration = RoundDownToInt(static_cast<float>(totalFrames) * frameDuration);
	float startToEnd = static_cast<float>(m_endSpriteIndex - m_startSpriteIndex);

	if (m_playbackType == SpriteAnimPlaybackType::ONCE)
	{
		animationDuration = static_cast<int>(GetClamped(static_cast<float>(animationDuration), 0.f, startToEnd));
	}
	else if (m_playbackType == SpriteAnimPlaybackType::LOOP)
	{
		animationDuration = animationDuration % totalFrames;
	}
	else if (m_playbackType == SpriteAnimPlaybackType::PINGPONG)
	{
		animationDuration = animationDuration % (totalFrames * 2 - 2);
		if (animationDuration >= totalFrames)
		{
			animationDuration = (totalFrames - 1) * 2 - animationDuration;
		}
	}
	return m_spriteSheet.GetSpriteDef(m_startSpriteIndex + animationDuration);
}

bool SpriteAnimDefinition::LoadFromXmlElement(XmlElement const& element)
{
	m_startSpriteIndex = ParseXmlAttribute(element, "startFrame", -1);
	m_endSpriteIndex = ParseXmlAttribute(element, "endFrame", -1);
	return true;
}

void SpriteAnimDefinition::LoadFromXmlElement(XmlElement const* element)
{
	m_startSpriteIndex = ParseXmlAttribute(*element, "startFrame", m_startSpriteIndex);
	m_endSpriteIndex = ParseXmlAttribute(*element, "endFrame", m_endSpriteIndex);
	m_framesPerSecond *= static_cast<float>((m_endSpriteIndex - m_startSpriteIndex + 1));
}

float SpriteAnimDefinition::GetDuration() const
{
	return m_framesPerSecond;
}
