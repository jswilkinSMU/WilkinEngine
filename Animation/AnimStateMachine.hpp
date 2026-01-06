#pragma once
#include <string>
#include <unordered_map>
// -----------------------------------------------------------------------------
class Animation;
class Skeleton;
// -----------------------------------------------------------------------------
struct AnimationState
{
	std::string m_animStateName = "None";
	Animation*  m_animation = nullptr;
	std::string m_nextAnimStateName = "None";
	float       m_timeInState = 0.f;

	AnimationState() = default;
	AnimationState(std::string const& animStateName, Animation* animation, std::string const& nextAnimStateName = "")
		: m_animStateName(animStateName), m_animation(animation), m_nextAnimStateName(nextAnimStateName) {}
};
// -----------------------------------------------------------------------------
struct AnimationBlend
{
	Animation* m_fromAnimation = nullptr;
	Animation* m_toAnimation = nullptr;
	float      m_blendDuration = 0.f;
	float      m_blendTime = 0.f;
	bool	   IsBlending() const { return m_blendTime < m_blendDuration; }
};
// -----------------------------------------------------------------------------
class AnimStateMachine
{
public:
	void AddAnimationState(std::string const& animStateName, Animation* animation, std::string const& nextAnimStateName = "");
	void AddTransition(std::string const& fromState, std::string const& condition, std::string const& toState);
	void TriggerTransition(std::string const& condition);
	void BlendToNextState(std::string const& nextStateName, float blendDuration);

	void            Update(Skeleton& skeleton, float deltaSeconds);
	void            SetAnimationState(std::string const& animStateName);
	AnimationState* GetAnimationState(std::string const& stateName);

	// Accessors
	std::string     GetCurrentAnimStateName() const;
	std::string     GetPreviousAnimStateName() const;
	AnimationState* GetCurrentAnimState() const;

private:
	std::unordered_map<std::string, AnimationState> m_animStates;
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_transitions;
	std::string m_triggeredCondition = "";
	std::string m_currentAnimStateName = "";
	std::string m_previousAnimStateName = "";
	AnimationState* m_currentAnimState = nullptr;
	AnimationBlend  m_blendState;
};
// -----------------------------------------------------------------------------