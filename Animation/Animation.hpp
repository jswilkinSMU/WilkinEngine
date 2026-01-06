#pragma once
#include <string>
#include <functional>
// -----------------------------------------------------------------------------
class Skeleton;
class Pose;
// -----------------------------------------------------------------------------
class Animation
{
public:
	Animation(std::string const& animName, float duration, bool isAnimLooping, std::function<void(Skeleton&, float)> animCallback);
	void ApplyAnimation(Skeleton& skeleton, float time) const;
	Pose GetPoseAtTime(Skeleton const& skeleton, float time) const;

public:
	std::string m_animName = "None";
	float       m_animDuration = 0.f;
	bool		m_animIsLooping = false;
	std::function<void(Skeleton&, float)> m_animCallback;
};