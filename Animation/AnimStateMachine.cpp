#include "Engine/Animation/AnimStateMachine.hpp"
#include "Engine/Skeleton/Skeleton.hpp"
#include "Engine/Skeleton/Pose.hpp"
#include "Engine/Animation/Animation.hpp"
#include "Engine/Math/MathUtils.h"

void AnimStateMachine::AddAnimationState(std::string const& animStateName, Animation* animation, std::string const& nextAnimStateName)
{
	m_animStates[animStateName] = AnimationState(animStateName, animation, nextAnimStateName);
}

void AnimStateMachine::AddTransition(std::string const& fromState, std::string const& condition, std::string const& toState)
{
	m_transitions[fromState][condition] = toState;
}

void AnimStateMachine::TriggerTransition(std::string const& condition)
{
	m_triggeredCondition = condition;
}

void AnimStateMachine::BlendToNextState(std::string const& nextStateName, float blendDuration)
{
	if (nextStateName == m_currentAnimStateName)
	{
		return;
	}

	AnimationState* nextAnimState = GetAnimationState(nextStateName);
	if (!nextAnimState)
	{
		return;
	}

	// Set Animation Blend
	m_blendState.m_fromAnimation = m_currentAnimState->m_animation;
	m_blendState.m_toAnimation = nextAnimState->m_animation;
	m_blendState.m_blendDuration = blendDuration;
	m_blendState.m_blendTime = 0.f;

	// Set States
	m_previousAnimStateName = m_currentAnimStateName;
	m_currentAnimState = nextAnimState;
	m_currentAnimStateName = nextStateName;
	m_currentAnimState->m_timeInState = 0.f;
}

void AnimStateMachine::SetAnimationState(std::string const& animStateName)
{
	// Save current state as previous state
	m_previousAnimStateName = m_currentAnimStateName;

	// Setting the new state
	m_currentAnimStateName = animStateName;

	auto foundAnimState = m_animStates.find(animStateName);
	if (foundAnimState != m_animStates.end())
	{
		m_currentAnimState = &foundAnimState->second;
		m_currentAnimState->m_timeInState = 0.f;
	}
	else
	{
		m_currentAnimState = nullptr;
	}
}

void AnimStateMachine::Update(Skeleton& skeleton, float deltaSeconds)
{
	// Check if we have an anim state
	if (m_currentAnimState == nullptr)
	{
		return;
	}

	// Check for a triggered condition
	if (!m_triggeredCondition.empty())
	{
		std::string currentStateName = m_currentAnimState->m_animStateName;
		auto foundState = m_transitions.find(currentStateName);

		if (foundState != m_transitions.end())
		{
			auto conditionState = foundState->second.find(m_triggeredCondition);
			if (conditionState != foundState->second.end())
			{
				BlendToNextState(conditionState->second, 0.3f);
			}
		}
		m_triggeredCondition.clear();
	}

	// Update Timers
	m_currentAnimState->m_timeInState += deltaSeconds;

	// If blending, lerp the two animations
	if (!m_blendState.IsBlending() && m_blendState.m_fromAnimation != nullptr && m_blendState.m_toAnimation != nullptr)
	{
		m_blendState.m_blendTime += deltaSeconds;

		float blendFactor = 0.f;
		if (m_blendState.m_blendDuration > 0.f)
		{
			blendFactor = GetClampedZeroToOne(m_blendState.m_blendTime / m_blendState.m_blendDuration);
		}
		else
		{
			blendFactor = 1.f;
		}

		// Get poses for both animations
		float fromTime = fmodf(m_currentAnimState->m_timeInState, m_blendState.m_fromAnimation->m_animDuration);
		float toTime = fmodf(m_currentAnimState->m_timeInState, m_blendState.m_toAnimation->m_animDuration);
		Pose  fromPose = m_blendState.m_fromAnimation->GetPoseAtTime(skeleton, fromTime);
		Pose  toPose = m_blendState.m_toAnimation->GetPoseAtTime(skeleton, toTime);

		// Blend poses together
		Pose blendedPose = Pose::BlendPoses(fromPose, toPose, blendFactor);

		// Apply blended pose to skeleton
		int numBones = static_cast<int>(skeleton.m_bones.size());
		for (int boneIndex = 0; boneIndex < numBones; ++boneIndex)
		{
			BoneTransform const& boneTransform = blendedPose.GetBoneTransform(boneIndex);
			skeleton.m_bones[boneIndex].SetLocalBoneTransform(boneTransform.m_bonePosition, boneTransform.m_boneRotation, boneTransform.m_boneScale);
		}

		// Stop blending when done
		if (blendFactor >= 1.f)
		{
			// Resetting blend
			m_blendState = AnimationBlend();
			m_currentAnimState->m_animation->ApplyAnimation(skeleton, toTime);
			return;
		}

		return;
	}

	// Play current animation normally
	float animTime = m_currentAnimState->m_timeInState;

	// Check if looping
	if (m_currentAnimState->m_animation->m_animDuration)
	{
		animTime = fmodf(animTime, m_currentAnimState->m_animation->m_animDuration);
	}
	else
	{
		animTime = GetClamped(animTime, 0.f, m_currentAnimState->m_animation->m_animDuration);
	}

	m_currentAnimState->m_animation->ApplyAnimation(skeleton, animTime);

	// Check for next state set
	if (!m_currentAnimState->m_animation->m_animIsLooping 
		&& m_currentAnimState->m_timeInState >= m_currentAnimState->m_animation->m_animDuration 
		&& !m_currentAnimState->m_nextAnimStateName.empty())
	{
		BlendToNextState(m_currentAnimState->m_nextAnimStateName, 0.25f);
	}
}

std::string AnimStateMachine::GetCurrentAnimStateName() const
{
	return m_currentAnimStateName;
}

std::string AnimStateMachine::GetPreviousAnimStateName() const
{
	return m_previousAnimStateName;
}

AnimationState* AnimStateMachine::GetAnimationState(std::string const& stateName)
{
	auto foundAnimState = m_animStates.find(stateName);
	if (foundAnimState != m_animStates.end())
	{
		return &foundAnimState->second;
	}
	else
	{
		return nullptr;
	}
}

AnimationState* AnimStateMachine::GetCurrentAnimState() const
{
	return m_currentAnimState;
}
