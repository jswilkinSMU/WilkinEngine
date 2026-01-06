#include "Engine/AI/BehaviorTree.hpp"

BehaviorTree::BehaviorTree(BehaviorNodePtr root)
	:m_root(root)
{
}

void BehaviorTree::Update(float deltaSeconds)
{
	if (m_root)
	{
		m_root->Update(deltaSeconds);
	}
}
