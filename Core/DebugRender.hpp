#pragma once
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Renderer/Renderer.h"
// -----------------------------------------------------------------------------
enum class DebugRenderMode
{
	ALWAYS,
	USE_DEPTH,
	X_RAY
};
// -----------------------------------------------------------------------------
struct DebugRenderConfig
{
	Renderer* m_renderer = nullptr;
	std::string m_fontName = "SquirrelFixedFont";
};
// -----------------------------------------------------------------------------
// Setup
void DebugRenderSystemStartup(DebugRenderConfig const& config);
void DebugRenderSystemShutdown();
// -----------------------------------------------------------------------------
// Control
void DebugRenderSetVisible();
void DebugRenderSetHidden();
void DebugRenderClear();
// -----------------------------------------------------------------------------
// Output
void DebugRenderBeginFrame();
void DebugRenderWorld(Camera const& camera);
void DebugRenderScreen(Camera const& camera);
void DebugRenderEndFrame();
// -----------------------------------------------------------------------------
// Geometry
void DebugAddWorldSphere(Vec3 const& pos, float radius, float duration, Rgba8 const& startColor = Rgba8::WHITE,
	Rgba8 const& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldCylinder(Vec3 const& start, Vec3 const& end, float radius, float duration, Rgba8 const& startColor = Rgba8::WHITE,
	Rgba8 const& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldWireSphere(Vec3 const& center, float radius, float duration, Rgba8 const& startColor = Rgba8::WHITE,
	Rgba8 const& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldWireCylinder(Vec3 const& base, Vec3 const& top, float radius, float duration, Rgba8 const& startColor = Rgba8::WHITE,
	Rgba8 const& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldCone(Vec3 const& start, Vec3 const& end, float radius, float duration, Rgba8 const& startColor = Rgba8::WHITE,
	Rgba8 const& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldWireCone(Vec3 const& start, Vec3 const& end, float radius, float duration, Rgba8 const& startColor = Rgba8::WHITE,
	Rgba8 const& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldQuad(Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, float duration, Rgba8 const& startColor = Rgba8::WHITE,
	Rgba8 const& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldWireQuad(Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, float duration, Rgba8 const& startColor = Rgba8::WHITE,
	Rgba8 const& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldAABB3(AABB3 const& alignedBox, float duration, Rgba8 const& startColor = Rgba8::WHITE,
	Rgba8 const& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldWireAABB3(AABB3 const& alignedBox, float duration, Rgba8 const& startColor = Rgba8::WHITE,
	Rgba8 const& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldArrow(Vec3 const& start, Vec3 const& end, float radius, float duration, Rgba8 const& startColor = Rgba8::WHITE,
	Rgba8 const& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldText(std::string const& text, Mat44 const& transform, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor = Rgba8::WHITE,
	Rgba8 const& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldBillboardText(std::string const& text, Vec3 const& origin, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor = Rgba8::WHITE,
	Rgba8 const& endColor = Rgba8::WHITE, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
void DebugAddWorldBasis(Mat44 const& transform, float duration, DebugRenderMode mode = DebugRenderMode::USE_DEPTH);
// -----------------------------------------------------------------------------
// Screen view
void DebugAddScreenText(std::string const& text, AABB2 const& box, float size, Vec2 const& alignment, float duration,
	Rgba8 const& startColor = Rgba8::WHITE, Rgba8 const& endColor = Rgba8::WHITE);
void DebugAddMessage(std::string const& text, float duration, Rgba8 const& startColor = Rgba8::WHITE, Rgba8 const& endColor = Rgba8::WHITE);
// -----------------------------------------------------------------------------
// Console Commands
bool Command_DebugRenderClear(EventArgs& args);
bool Command_DebugRenderToggle(EventArgs& args);
// -----------------------------------------------------------------------------