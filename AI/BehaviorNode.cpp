#include "Engine/AI/BehaviorNode.hpp"
// -----------------------------------------------------------------------------
SelectorNode::SelectorNode(std::vector<BehaviorNodePtr> children)
	:m_children(children)
{
}

BehaviorResult SelectorNode::Update(float deltaSeconds)
{
	for (int childIndex = 0; childIndex < static_cast<int>(m_children.size()); ++childIndex)
	{
		BehaviorNodePtr child = m_children[childIndex];
		BehaviorResult result = child->Update(deltaSeconds);

		if (result != BehaviorResult::FAILURE)
		{
			return result;
		}
	}
	return BehaviorResult::FAILURE;
}
// -----------------------------------------------------------------------------
SequenceNode::SequenceNode(std::vector<BehaviorNodePtr> children)
	:m_children(children)
{
}

BehaviorResult SequenceNode::Update(float deltaSeconds)
{
	for (int childIndex = 0; childIndex < static_cast<int>(m_children.size()); ++childIndex)
	{
		BehaviorNodePtr child = m_children[childIndex];
		BehaviorResult result = child->Update(deltaSeconds);

		if (result != BehaviorResult::SUCCESS)
		{
			return result;
		}
	}
	return BehaviorResult::SUCCESS;
}
// -----------------------------------------------------------------------------