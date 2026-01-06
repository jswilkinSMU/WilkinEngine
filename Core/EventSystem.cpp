#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EngineCommon.h"
EventSystem* g_theEventSystem = nullptr;

EventSystem::EventSystem(EventSystemConfig const& config)
	:m_config(config)
{
}

EventSystem::~EventSystem()
{
}

void EventSystem::Startup()
{
}

void EventSystem::Shutdown()
{
	m_subscriptionListsByEventName.clear();
}

void EventSystem::BeginFrame()
{
}

void EventSystem::EndFrame()
{
}

void EventSystem::SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	std::scoped_lock<std::mutex> lock(m_eventSystemMutex);

	SubscriptionList& subscriptionList = m_subscriptionListsByEventName[eventName];
	EventSubscription newSubscription = { functionPtr };
	subscriptionList.push_back(newSubscription);
}

void EventSystem::UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	std::scoped_lock<std::mutex> lock(m_eventSystemMutex);

	std::map<std::string, SubscriptionList>::iterator found = m_subscriptionListsByEventName.find(eventName);
	if (found == m_subscriptionListsByEventName.end())
	{
		return;
	}
	SubscriptionList& subscriptionList = found->second;
	for (int subIndex = 0; subIndex < (int)subscriptionList.size(); ++subIndex)
	{
		if (subscriptionList[subIndex].callbackFunction == functionPtr)
		{
			subscriptionList.erase(subscriptionList.begin() + subIndex);
			break;
		}
	}
}

void EventSystem::FireEvent(std::string const& eventName, EventArgs& args)
{
	std::map<std::string, SubscriptionList> subscriptionListByEventName;

	{
		std::scoped_lock<std::mutex> lock(m_eventSystemMutex);
		subscriptionListByEventName = m_subscriptionListsByEventName;
	}

	std::map<std::string, SubscriptionList>::iterator found = subscriptionListByEventName.find(eventName);
	if (found == subscriptionListByEventName.end())
	{
		if (g_theDevConsole != nullptr)
		{
			g_theDevConsole->AddLine(DevConsole::ERROR_MAJOR, "Unknown Command: " + eventName);
		}
		return;
	}
	SubscriptionList& subscriptionList = found->second;
	for (int subIndex = 0; subIndex < (int)subscriptionList.size(); ++subIndex)
	{
		if (subscriptionList[subIndex].callbackFunction(args))
		{
			break;
		}
	}
}

void EventSystem::FireEvent(std::string const& eventName)
{
	EventArgs args;
	FireEvent(eventName, args);
}

std::vector<std::string> EventSystem::GetAllRegisteredCommands() const
{
	std::vector<std::string> registeredCommands;
	for (auto found = m_subscriptionListsByEventName.begin(); found != m_subscriptionListsByEventName.end(); ++found)
	{
		registeredCommands.push_back(found->first);
	}
	return registeredCommands;
}

void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	if (g_theEventSystem)
	{
		g_theEventSystem->SubscribeEventCallbackFunction(eventName, functionPtr);
	}
}

void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	if (g_theEventSystem)
	{
		g_theEventSystem->UnsubscribeEventCallbackFunction(eventName, functionPtr);
	}
}

void FireEvent(std::string const& eventName, EventArgs& args)
{
	if (g_theEventSystem)
	{
		g_theEventSystem->FireEvent(eventName, args);
	}
}

void FireEvent(std::string const& eventName)
{
	if (g_theEventSystem)
	{
		g_theEventSystem->FireEvent(eventName);
	}
}
