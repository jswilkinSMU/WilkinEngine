#pragma once
#include "Engine/AI/BehaviorNode.hpp"
// -----------------------------------------------------------------------------
class BehaviorTree
{
public:
	BehaviorTree(BehaviorNodePtr root);
	void Update(float deltaSeconds);

private:
	BehaviorNodePtr m_root = nullptr;
};
// -----------------------------------------------------------------------------