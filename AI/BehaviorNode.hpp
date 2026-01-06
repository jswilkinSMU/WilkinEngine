#pragma once
#include <vector>
#include <memory>
// -----------------------------------------------------------------------------
enum class BehaviorResult
{
	SUCCESS,
	RUNNING,
	FAILURE
};
// -----------------------------------------------------------------------------
class BehaviorNode
{
public:
	virtual ~BehaviorNode() = default;
	virtual BehaviorResult Update(float deltaSeconds) = 0;
};
// -----------------------------------------------------------------------------
typedef std::shared_ptr<BehaviorNode> BehaviorNodePtr;
// -----------------------------------------------------------------------------
class SelectorNode : public BehaviorNode
{
public:
	SelectorNode(std::vector<BehaviorNodePtr> children);
	BehaviorResult Update(float deltaSeconds) override;

private:
	std::vector<BehaviorNodePtr> m_children;
};
// -----------------------------------------------------------------------------
class SequenceNode : public BehaviorNode
{
public:
	SequenceNode(std::vector<BehaviorNodePtr> children);
	BehaviorResult Update(float deltaSeconds) override;

private:
	std::vector<BehaviorNodePtr> m_children;
};
// -----------------------------------------------------------------------------