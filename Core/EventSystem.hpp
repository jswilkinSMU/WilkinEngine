#pragma once
#include "Engine/Core/NamedStrings.hpp"
#include <vector>
#include <map>
#include <string>
#include <mutex>
// -----------------------------------------------------------------------------
class DevConsole;
// -----------------------------------------------------------------------------
typedef NamedStrings EventArgs;
typedef bool (*EventCallbackFunction)(EventArgs& args);
// -----------------------------------------------------------------------------
struct EventSystemConfig
{
};
// -----------------------------------------------------------------------------
struct EventSubscription
{
	EventCallbackFunction callbackFunction;
};
// -----------------------------------------------------------------------------
typedef std::vector<EventSubscription> SubscriptionList;
class EventSystem
{
public:
	EventSystem(EventSystemConfig const& config);
	~EventSystem();
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();

	void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
	void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
	void FireEvent(std::string const& eventName, EventArgs& args);
	void FireEvent(std::string const& eventName);

	std::vector<std::string> GetAllRegisteredCommands() const;

protected:
	EventSystemConfig m_config;
	std::map<std::string, SubscriptionList> m_subscriptionListsByEventName;
	
	// EventSystem's stored internal mutex
	mutable std::mutex m_eventSystemMutex;
};

// -----------------------------------------------------------------------------
// Standalone global-namespace helper functions; these forward to "the" event system, if it exists
//
void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void FireEvent(std::string const& eventName, EventArgs& args);
void FireEvent(std::string const& eventName);