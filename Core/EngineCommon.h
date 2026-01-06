#pragma once
// ------------------------------------
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
// ------------------------------------
#define UNUSED(x) (void)(x);
// -----------------------------------------------------------------------------
class InputSystem;
class NamedStrings;
class EventSystem;
class DevConsole;
// -----------------------------------------------------------------------------
extern NamedStrings  g_gameConfigBlackboard; // declared in EngineCommon.hpp, defined in EngineCommon.cpp
extern InputSystem*  g_theInput;
extern EventSystem*  g_theEventSystem;
extern DevConsole*   g_theDevConsole;