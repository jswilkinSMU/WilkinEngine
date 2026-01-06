#include "Engine/Core/DebugRender.hpp"
#include "Engine/Renderer/Camera.h"
#include "Engine/Window/Window.hpp"
#include "Engine/Core/EngineCommon.h"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/Timer.hpp"
#include "Engine/Math/MathUtils.h"
// -----------------------------------------------------------------------------
// Struct for Debug Render Geometry
struct DebugRenderGeometry
{
	DebugRenderGeometry() = default;

	std::vector<Vertex_PCU> m_geometryVerts;
	DebugRenderMode m_debugRenderMode = DebugRenderMode::USE_DEPTH;
	RasterizerMode m_rasterizerMode = RasterizerMode::SOLID_CULL_NONE;
	BillboardType m_billBoardType = BillboardType::NONE;
	Timer* m_timer = nullptr;
	Texture* m_texture = nullptr;
	float  m_timeDuration = 0.f;
	Vec3   m_startPosition = Vec3::ZERO;
	Vec3   m_endPosition = Vec3::ONE;
	float  m_radius = 0.f;
	Mat44  m_matrixTransform = Mat44();
	Rgba8  m_setColor = Rgba8::WHITE;
	Rgba8  m_geometryStartColor = Rgba8::WHITE;
	Rgba8  m_geometryEndColor = Rgba8::WHITE;
};
// -----------------------------------------------------------------------------
// Struct for Debug Render Text
struct DebugRenderText
{
	DebugRenderText() = default;

	std::vector<Vertex_PCU> m_textVerts;
	std::string m_text;
	DebugRenderMode m_debugRenderMode = DebugRenderMode::USE_DEPTH;
	RasterizerMode m_rasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	Timer* m_timer = nullptr;
	Mat44  m_matrixTransform = Mat44();
	float m_timeDuration = 0.f;
	Vec3 m_worldTextPosition = Vec3::ZERO;
	Vec2 m_screenTextPosition = Vec2::ZERO;
	Vec2 m_alignment = Vec2::ZERO;
	float m_textHeight = 20.f;
	int   m_startText = -1;
	Rgba8 m_startColor = Rgba8::WHITE;
	Rgba8 m_endColor = Rgba8::WHITE;
};
// -----------------------------------------------------------------------------
// Global variables (Only viewed by Debug Render)
static DebugRenderConfig s_debugRenderConfig;
static DebugRenderGeometry s_debugGeometry;
static std::vector<DebugRenderGeometry> s_debugShapes;
static std::vector<DebugRenderText> s_debugTexts;
static std::vector<DebugRenderText> s_messageTexts;
static DebugRenderText s_debugText;
static BitmapFont* s_font;
static bool s_debugRenderHidden;
static int  s_firstMessage = 0;
Timer* SetupTimer(float timeDuration);
std::mutex s_debugWorldMutex;
std::mutex s_debugScreenMutex;
// -----------------------------------------------------------------------------
void DebugRenderSystemStartup(DebugRenderConfig const& config)
{
	s_debugRenderConfig = config;
	s_font = s_debugRenderConfig.m_renderer->CreateOrGetBitmapFont(s_debugRenderConfig.m_fontName.c_str());

	SubscribeEventCallbackFunction("DebugRenderClear", Command_DebugRenderClear);
	SubscribeEventCallbackFunction("DebugRenderToggle", Command_DebugRenderToggle);
}

void DebugRenderSystemShutdown()
{
	DebugRenderClear();

	for (size_t geometryIndex = 0; geometryIndex < s_debugShapes.size(); ++geometryIndex)
	{
		delete &s_debugShapes[geometryIndex];
	}

	for (size_t textIndex = 0; textIndex < s_debugTexts.size(); ++textIndex)
	{
		delete &s_debugTexts[textIndex];
	}

	for (size_t textIndex = 0; textIndex < s_messageTexts.size(); ++textIndex)
	{
		delete& s_messageTexts[textIndex];
	}
}

void DebugRenderSetVisible()
{
	s_debugRenderHidden = false;
}

void DebugRenderSetHidden()
{
	s_debugRenderHidden = true;
}

void DebugRenderClear()
{
	// Clearing geometry shapes
	s_debugWorldMutex.lock();
	s_debugShapes.clear();
	s_debugWorldMutex.unlock();

	// Clearing screen text
	s_debugScreenMutex.lock();
	s_debugTexts.clear();
	s_debugScreenMutex.unlock();

	// Clear messages
	s_debugScreenMutex.lock();
	s_messageTexts.clear();
	s_debugScreenMutex.unlock();
}

void DebugRenderBeginFrame()
{
	// Geometry time loop
	s_debugWorldMutex.lock();
	for (std::vector<DebugRenderGeometry>::iterator found = s_debugShapes.begin(); found != s_debugShapes.end();)
	{
		DebugRenderGeometry& geometry = *found;
		if (geometry.m_timer)
		{
			if (geometry.m_timer->HasPeriodElapsed())
			{
				// If the geometry duration period has ended, erase it
				found = s_debugShapes.erase(found);
			}
			else
			{
				// If the geometry duration period is ongoing, interpolate the start and end colors and increment
				geometry.m_setColor = geometry.m_setColor.Rgba8Interpolate(geometry.m_geometryStartColor, geometry.m_geometryEndColor, static_cast<float>(geometry.m_timer->GetElapsedFraction()));
				found += 1;
			}
		}
		else
		{
			// Increment through DebugRenderGeometry if no timer is found
			found += 1;
		}
	}
	s_debugWorldMutex.unlock();

	// Text time loop
	s_debugScreenMutex.lock();
	for (std::vector<DebugRenderText>::iterator found = s_debugTexts.begin(); found != s_debugTexts.end();)
	{
		DebugRenderText& text = *found;
		if (text.m_timer)
		{
			if (text.m_timer->HasPeriodElapsed())
			{
				// If the text duration period has ended, erase it
				found = s_debugTexts.erase(found);
			}
			else
			{
				// If the text duration period is ongoing, interpolate the start and end colors and increment
				text.m_startColor = text.m_startColor.Rgba8Interpolate(text.m_startColor, text.m_endColor, static_cast<float>(text.m_timer->GetElapsedFraction()));
				found += 1;
			}
		}
		else
		{
			// Increment through DebugRenderText if no timer is found
			found += 1;
		}
	}
	s_debugScreenMutex.unlock();

	// Message time loop
	s_debugScreenMutex.lock();
	for (std::vector<DebugRenderText>::iterator found = s_messageTexts.begin(); found != s_messageTexts.end();)
	{
		DebugRenderText& text = *found;
		if (text.m_timer)
		{
			if (text.m_timer->HasPeriodElapsed())
			{
				// If the text duration period has ended, erase it
				found = s_messageTexts.erase(found);
			}
			else
			{
				// If the text duration period is ongoing, interpolate the start and end colors and increment
				text.m_startColor = text.m_startColor.Rgba8Interpolate(text.m_startColor, text.m_endColor, static_cast<float>(text.m_timer->GetElapsedFraction()));
				found += 1;
			}
		}
		else
		{
			// Increment through DebugRenderText if no timer is found
			found += 1;
		}
	}
	s_debugScreenMutex.unlock();
}

void DebugRenderWorld(Camera const& camera)
{
	if (!s_debugRenderHidden)
	{
		s_debugRenderConfig.m_renderer->BeginCamera(camera);
		s_debugRenderConfig.m_renderer->BindTexture(nullptr);
		s_debugRenderConfig.m_renderer->BindShader(nullptr);
		s_debugWorldMutex.lock();

		for (size_t geometryIndex = 0; geometryIndex < s_debugShapes.size(); ++geometryIndex)
		{
			s_debugRenderConfig.m_renderer->SetRasterizerMode(s_debugShapes[geometryIndex].m_rasterizerMode);

			if (s_debugShapes[geometryIndex].m_billBoardType == BillboardType::FULL_OPPOSING)
			{
				s_debugRenderConfig.m_renderer->SetModelConstants(GetBillboardMatrix(BillboardType::FULL_OPPOSING, camera.GetOrientation().GetAsMatrix_IFwd_JLeft_KUp(),
					s_debugShapes[geometryIndex].m_startPosition), s_debugShapes[geometryIndex].m_setColor);
				s_debugRenderConfig.m_renderer->SetBlendMode(BlendMode::ALPHA);
				s_debugRenderConfig.m_renderer->SetDepthMode(DepthMode::READ_WRITE_LESS_EQUAL);
				s_debugRenderConfig.m_renderer->BindTexture(s_debugShapes[geometryIndex].m_texture);
				s_debugRenderConfig.m_renderer->DrawVertexArray(s_debugShapes[geometryIndex].m_geometryVerts);
			}

			if (s_debugShapes[geometryIndex].m_debugRenderMode == DebugRenderMode::ALWAYS)
			{
				s_debugRenderConfig.m_renderer->SetModelConstants(Mat44(), s_debugShapes[geometryIndex].m_setColor);
				s_debugRenderConfig.m_renderer->SetDepthMode(DepthMode::DISABLED);
				s_debugRenderConfig.m_renderer->BindTexture(s_debugShapes[geometryIndex].m_texture);
				s_debugRenderConfig.m_renderer->DrawVertexArray(s_debugShapes[geometryIndex].m_geometryVerts);
			}
			else if (s_debugShapes[geometryIndex].m_debugRenderMode == DebugRenderMode::USE_DEPTH)
			{
				s_debugRenderConfig.m_renderer->SetModelConstants(Mat44(), s_debugShapes[geometryIndex].m_setColor);
				s_debugRenderConfig.m_renderer->SetBlendMode(BlendMode::OPAQUE);
				s_debugRenderConfig.m_renderer->SetDepthMode(DepthMode::READ_WRITE_LESS_EQUAL);
				s_debugRenderConfig.m_renderer->BindTexture(s_debugShapes[geometryIndex].m_texture);
				s_debugRenderConfig.m_renderer->DrawVertexArray(s_debugShapes[geometryIndex].m_geometryVerts);
			}
			else if (s_debugShapes[geometryIndex].m_debugRenderMode == DebugRenderMode::X_RAY)
			{
				// First draw call
				Rgba8 xrayColor = s_debugShapes[geometryIndex].m_setColor;
				xrayColor.a = 130;
				s_debugRenderConfig.m_renderer->SetModelConstants(Mat44(), xrayColor);
				s_debugRenderConfig.m_renderer->SetBlendMode(BlendMode::ALPHA);
				s_debugRenderConfig.m_renderer->SetDepthMode(DepthMode::DISABLED);
				s_debugRenderConfig.m_renderer->BindTexture(s_debugShapes[geometryIndex].m_texture);
				s_debugRenderConfig.m_renderer->DrawVertexArray(s_debugShapes[geometryIndex].m_geometryVerts);

				// Second draw call
				s_debugRenderConfig.m_renderer->SetModelConstants(Mat44(), s_debugShapes[geometryIndex].m_setColor);
				s_debugRenderConfig.m_renderer->SetBlendMode(BlendMode::OPAQUE);
				s_debugRenderConfig.m_renderer->SetDepthMode(DepthMode::READ_WRITE_LESS_EQUAL);
				s_debugRenderConfig.m_renderer->BindTexture(s_debugShapes[geometryIndex].m_texture);
				s_debugRenderConfig.m_renderer->DrawVertexArray(s_debugShapes[geometryIndex].m_geometryVerts);
			}
		}
		s_debugWorldMutex.unlock();
		s_debugRenderConfig.m_renderer->EndCamera(camera);
	}
}

void DebugRenderScreen(Camera const& camera)
{
	if (!s_debugRenderHidden)
	{
		s_debugRenderConfig.m_renderer->BeginCamera(camera);
		s_debugRenderConfig.m_renderer->BindShader(nullptr);
		s_debugScreenMutex.lock();

		for (size_t textIndex = 0; textIndex < s_debugTexts.size(); ++textIndex)
		{
			s_debugRenderConfig.m_renderer->SetModelConstants();
			s_debugRenderConfig.m_renderer->SetBlendMode(BlendMode::ALPHA);
			s_debugRenderConfig.m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
			s_debugRenderConfig.m_renderer->SetDepthMode(DepthMode::DISABLED);
			s_debugRenderConfig.m_renderer->BindTexture(&s_font->GetTexture());
			s_debugRenderConfig.m_renderer->DrawVertexArray(s_debugTexts[textIndex].m_textVerts);
		}

		for (size_t textIndex = 0; textIndex < s_messageTexts.size(); ++textIndex)
		{
			Mat44 translationMatrix;
			int clientDimensionsY = Window::s_mainWindow->GetClientDimensions().y;
			float translateDownYDuration = static_cast<float>(clientDimensionsY) - 380.f - (textIndex * s_messageTexts[textIndex].m_textHeight) - (s_firstMessage * s_messageTexts[textIndex].m_textHeight);
			float translateDownYStart = static_cast<float>(clientDimensionsY) - 380.f - (s_messageTexts[textIndex].m_startText * s_messageTexts[textIndex].m_textHeight);

			if (s_messageTexts[textIndex].m_timer == nullptr || s_messageTexts[textIndex].m_timer->m_period == 0)
			{
				translationMatrix.SetTranslation2D(Vec2(0.f, translateDownYStart));
			}
			else
			{
				translationMatrix.SetTranslation2D(Vec2(0.f, translateDownYDuration));
			}

			s_debugRenderConfig.m_renderer->SetModelConstants(translationMatrix, s_messageTexts[textIndex].m_startColor);
			s_debugRenderConfig.m_renderer->SetBlendMode(BlendMode::ALPHA);
			s_debugRenderConfig.m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
			s_debugRenderConfig.m_renderer->SetDepthMode(DepthMode::DISABLED);
			s_debugRenderConfig.m_renderer->BindTexture(&s_font->GetTexture());
			s_debugRenderConfig.m_renderer->DrawVertexArray(s_messageTexts[textIndex].m_textVerts);
		}

		s_debugScreenMutex.unlock();
		s_debugRenderConfig.m_renderer->EndCamera(camera);
	}
}

void DebugRenderEndFrame()
{

}

void DebugAddWorldSphere(Vec3 const& pos, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderGeometry debugSphere;

	// Setting sphere variables to geometry struct members
	debugSphere.m_debugRenderMode = mode;
	debugSphere.m_setColor = startColor;
	debugSphere.m_geometryStartColor = startColor;
	debugSphere.m_geometryEndColor = endColor;
	debugSphere.m_rasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	debugSphere.m_timer = SetupTimer(duration);
	AddVertsForSphere3D(debugSphere.m_geometryVerts, pos, radius, Rgba8::WHITE, AABB2::ZERO_TO_ONE, 32, 16);

	s_debugWorldMutex.lock();
	s_debugShapes.push_back(debugSphere);
	s_debugWorldMutex.unlock();
}

void DebugAddWorldWireSphere(Vec3 const& center, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderGeometry debugWireSphere;

	// Setting sphere variables to geometry struct members
	debugWireSphere.m_debugRenderMode = mode;
	debugWireSphere.m_setColor = startColor;
	debugWireSphere.m_geometryStartColor = startColor;
	debugWireSphere.m_geometryEndColor = endColor;
	debugWireSphere.m_rasterizerMode = RasterizerMode::WIREFRAME_CULL_BACK;
	debugWireSphere.m_timer = SetupTimer(duration);
	AddVertsForSphere3D(debugWireSphere.m_geometryVerts, center, radius, Rgba8::WHITE, AABB2(Vec2::ZERO, Vec2::ONE), 32, 16);

	s_debugWorldMutex.lock();
	s_debugShapes.push_back(debugWireSphere);
	s_debugWorldMutex.unlock();
}

void DebugAddWorldCylinder(Vec3 const& start, Vec3 const& end, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderGeometry debugCylinder;

	// Setting line variables to geometry struct members
	debugCylinder.m_debugRenderMode = mode;
	debugCylinder.m_setColor = startColor;
	debugCylinder.m_geometryStartColor = startColor;
	debugCylinder.m_geometryEndColor = endColor;
	debugCylinder.m_rasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	debugCylinder.m_timer = SetupTimer(duration);
	AddVertsForCylinder3D(debugCylinder.m_geometryVerts, start, end, radius, Rgba8::WHITE, AABB2::ZERO_TO_ONE, 16);

	s_debugWorldMutex.lock();
	s_debugShapes.push_back(debugCylinder);
	s_debugWorldMutex.unlock();
}

void DebugAddWorldWireCylinder(Vec3 const& base, Vec3 const& top, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderGeometry debugWireCylinder;

	// Setting cylinder variables to geometry struct members
	debugWireCylinder.m_debugRenderMode = mode;
	debugWireCylinder.m_setColor = startColor;
	debugWireCylinder.m_geometryStartColor = startColor;
	debugWireCylinder.m_geometryEndColor = endColor;
	debugWireCylinder.m_rasterizerMode = RasterizerMode::WIREFRAME_CULL_BACK;
	debugWireCylinder.m_timer = SetupTimer(duration);
	AddVertsForCylinder3D(debugWireCylinder.m_geometryVerts, base, top, radius, Rgba8::WHITE, AABB2(Vec2::ZERO, Vec2::ONE), 16);

	s_debugWorldMutex.lock();
	s_debugShapes.push_back(debugWireCylinder);
	s_debugWorldMutex.unlock();
}

void DebugAddWorldCone(Vec3 const& start, Vec3 const& end, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderGeometry debugCone;

	// Setting cone variables to geometry struct members
	debugCone.m_debugRenderMode = mode;
	debugCone.m_setColor = startColor;
	debugCone.m_geometryStartColor = startColor;
	debugCone.m_geometryEndColor = endColor;
	debugCone.m_rasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	debugCone.m_timer = SetupTimer(duration);
	AddVertsForCone3D(debugCone.m_geometryVerts, start, end, radius, Rgba8::WHITE, AABB2::ZERO_TO_ONE, 16);

	s_debugWorldMutex.lock();
	s_debugShapes.push_back(debugCone);
	s_debugWorldMutex.unlock();
}

void DebugAddWorldWireCone(Vec3 const& start, Vec3 const& end, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderGeometry debugWireCone;

	// Setting cone variables to geometry struct members
	debugWireCone.m_debugRenderMode = mode;
	debugWireCone.m_setColor = startColor;
	debugWireCone.m_geometryStartColor = startColor;
	debugWireCone.m_geometryEndColor = endColor;
	debugWireCone.m_rasterizerMode = RasterizerMode::WIREFRAME_CULL_BACK;
	debugWireCone.m_timer = SetupTimer(duration);
	AddVertsForCone3D(debugWireCone.m_geometryVerts, start, end, radius, Rgba8::WHITE, AABB2::ZERO_TO_ONE, 16);

	s_debugWorldMutex.lock();
	s_debugShapes.push_back(debugWireCone);
	s_debugWorldMutex.unlock();
}

void DebugAddWorldQuad(Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderGeometry debugQuad;

	// Setting quad variables to geometry struct members
	debugQuad.m_debugRenderMode = mode;
	debugQuad.m_setColor = startColor;
	debugQuad.m_geometryStartColor = startColor;
	debugQuad.m_geometryEndColor = endColor;
	debugQuad.m_rasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	debugQuad.m_timer = SetupTimer(duration);
	AddVertsForQuad3D(debugQuad.m_geometryVerts, bottomLeft, bottomRight, topRight, topLeft, Rgba8::WHITE, AABB2::ZERO_TO_ONE);

	s_debugWorldMutex.lock();
	s_debugShapes.push_back(debugQuad);
	s_debugWorldMutex.unlock();
}


void DebugAddWorldWireQuad(Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderGeometry debugWireQuad;

	// Setting quad variables to geometry struct members
	debugWireQuad.m_debugRenderMode = mode;
	debugWireQuad.m_setColor = startColor;
	debugWireQuad.m_geometryStartColor = startColor;
	debugWireQuad.m_geometryEndColor = endColor;
	debugWireQuad.m_rasterizerMode = RasterizerMode::WIREFRAME_CULL_BACK;
	debugWireQuad.m_timer = SetupTimer(duration);
	AddVertsForQuad3D(debugWireQuad.m_geometryVerts, bottomLeft, bottomRight, topRight, topLeft, Rgba8::WHITE, AABB2::ZERO_TO_ONE);

	s_debugWorldMutex.lock();
	s_debugShapes.push_back(debugWireQuad);
	s_debugWorldMutex.unlock();
}

void DebugAddWorldAABB3(AABB3 const& alignedBox, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderGeometry debugBox;

	// Setting aabb3 variables to geometry struct members
	debugBox.m_debugRenderMode = mode;
	debugBox.m_setColor = startColor;
	debugBox.m_geometryStartColor = startColor;
	debugBox.m_geometryEndColor = endColor;
	debugBox.m_rasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	debugBox.m_timer = SetupTimer(duration);
	AddVertsForAABB3D(debugBox.m_geometryVerts, alignedBox, Rgba8::WHITE, AABB2::ZERO_TO_ONE);

	s_debugWorldMutex.lock();
	s_debugShapes.push_back(debugBox);
	s_debugWorldMutex.unlock();
}

void DebugAddWorldWireAABB3(AABB3 const& alignedBox, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderGeometry debugWireBox;

	// Setting aabb3 variables to geometry struct members
	debugWireBox.m_debugRenderMode = mode;
	debugWireBox.m_setColor = startColor;
	debugWireBox.m_geometryStartColor = startColor;
	debugWireBox.m_geometryEndColor = endColor;
	debugWireBox.m_rasterizerMode = RasterizerMode::WIREFRAME_CULL_BACK;
	debugWireBox.m_timer = SetupTimer(duration);
	AddVertsForAABB3D(debugWireBox.m_geometryVerts, alignedBox, Rgba8::WHITE, AABB2::ZERO_TO_ONE);

	s_debugWorldMutex.lock();
	s_debugShapes.push_back(debugWireBox);
	s_debugWorldMutex.unlock();
}

void DebugAddWorldArrow(Vec3 const& start, Vec3 const& end, float radius, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderGeometry debugArrow;

	// Setting arrow variables to geometry struct members
	debugArrow.m_debugRenderMode = mode;
	debugArrow.m_setColor = startColor;
	debugArrow.m_geometryStartColor = startColor;
	debugArrow.m_geometryEndColor = endColor;
	debugArrow.m_rasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	debugArrow.m_timer = SetupTimer(duration);
	AddVertsForArrow3D(debugArrow.m_geometryVerts, start, end, radius, Rgba8::WHITE, 32);

	s_debugWorldMutex.lock();
	s_debugShapes.push_back(debugArrow);
	s_debugWorldMutex.unlock();
}

void DebugAddWorldText(std::string const& text, Mat44 const& transform, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderGeometry debugWorldText;

	// Setting world text variables to text struct members
	debugWorldText.m_debugRenderMode = mode;
	debugWorldText.m_setColor = startColor;
	debugWorldText.m_geometryStartColor = startColor;
	debugWorldText.m_geometryEndColor = endColor;
	debugWorldText.m_rasterizerMode = RasterizerMode::SOLID_CULL_NONE;
	debugWorldText.m_timer = SetupTimer(duration);
	debugWorldText.m_texture = &s_font->GetTexture();
	s_font->AddVertsForText3DAtOriginXForward(debugWorldText.m_geometryVerts, textHeight, text, Rgba8::WHITE, 1.f, alignment);

	TransformVertexArray3D(debugWorldText.m_geometryVerts, transform);

	s_debugWorldMutex.lock();
	s_debugShapes.push_back(debugWorldText);
	s_debugWorldMutex.unlock();
}

void DebugAddWorldBillboardText(std::string const& text, Vec3 const& origin, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor, Rgba8 const& endColor, DebugRenderMode mode)
{
	DebugRenderGeometry debugWorldBillboardText;

	// Setting world billboard text to text struct members
	debugWorldBillboardText.m_debugRenderMode = mode;
	debugWorldBillboardText.m_startPosition = origin;
	debugWorldBillboardText.m_setColor = startColor;
	debugWorldBillboardText.m_geometryStartColor = startColor;
	debugWorldBillboardText.m_geometryEndColor = endColor;
	debugWorldBillboardText.m_rasterizerMode = RasterizerMode::SOLID_CULL_NONE;
	debugWorldBillboardText.m_billBoardType = BillboardType::FULL_OPPOSING;
	debugWorldBillboardText.m_timer = SetupTimer(duration);
	debugWorldBillboardText.m_texture = &s_font->GetTexture();

	s_font->AddVertsForText3DAtOriginXForward(debugWorldBillboardText.m_geometryVerts, textHeight, text, Rgba8::WHITE, 1.f, alignment);

	Mat44 modelMatrix;
	modelMatrix = EulerAngles(90.f, 0.f, 90.f).GetAsMatrix_IFwd_JLeft_KUp();
	TransformVertexArray3D(debugWorldBillboardText.m_geometryVerts, modelMatrix);

	s_debugWorldMutex.lock();
	s_debugShapes.push_back(debugWorldBillboardText);
	s_debugWorldMutex.unlock();
}

void DebugAddWorldBasis(Mat44 const& transform, float duration, DebugRenderMode mode)
{
	DebugRenderGeometry debugWorldBasis;
	float arrowRadius = 0.15f;

	// Setting arrow basis
	debugWorldBasis.m_debugRenderMode = mode;
	debugWorldBasis.m_rasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	debugWorldBasis.m_timer = SetupTimer(duration);

	AddVertsForArrow3D(debugWorldBasis.m_geometryVerts, Vec3::ZERO, Vec3::XAXE, arrowRadius, Rgba8::RED, 32);
	AddVertsForArrow3D(debugWorldBasis.m_geometryVerts, Vec3::ZERO, Vec3::YAXE, arrowRadius, Rgba8::GREEN, 32);
	AddVertsForArrow3D(debugWorldBasis.m_geometryVerts, Vec3::ZERO, Vec3::ZAXE, arrowRadius, Rgba8::BLUE, 32);

	TransformVertexArray3D(debugWorldBasis.m_geometryVerts, transform);

	s_debugWorldMutex.lock();
	s_debugShapes.push_back(debugWorldBasis);
	s_debugWorldMutex.unlock();
}

void DebugAddScreenText(std::string const& text, AABB2 const& box, float size, Vec2 const& alignment, float duration, Rgba8 const& startColor, Rgba8 const& endColor)
{
	DebugRenderText debugScreenText;

	// Setting screen text to text struct members
	debugScreenText.m_timer = SetupTimer(duration);
	debugScreenText.m_startColor = startColor;
	debugScreenText.m_endColor = endColor;
	s_font->AddVertsForTextInBox2D(debugScreenText.m_textVerts, text, box, size, Rgba8::WHITE, 1.f, alignment);

	s_debugScreenMutex.lock();
	s_debugTexts.push_back(debugScreenText);
	s_debugScreenMutex.unlock();
}

void DebugAddMessage(std::string const& text, float duration, Rgba8 const& startColor, Rgba8 const& endColor)
{
	DebugRenderText debugMessageText;

	// Setting messages to the text struct members
	debugMessageText.m_timer = SetupTimer(duration);
	debugMessageText.m_startColor = startColor;
	debugMessageText.m_endColor = endColor;
	s_font->AddVertsForText2D(debugMessageText.m_textVerts, Vec2::ZERO, 10.f, text);

	s_debugScreenMutex.lock();
	s_messageTexts.push_back(debugMessageText);
	s_debugScreenMutex.unlock();
}

bool Command_DebugRenderClear(EventArgs& args)
{
	UNUSED(args);
	if (g_theDevConsole->GetMode() == DevConsoleMode::OPEN_FULL)
	{
		DebugRenderClear();
		return true;
	}
	return false;
}

bool Command_DebugRenderToggle(EventArgs& args)
{
	UNUSED(args);
	if (g_theDevConsole->GetMode() == DevConsoleMode::OPEN_FULL)
	{
		s_debugRenderHidden = !s_debugRenderHidden;
		return true;
	}
	return false;
}

Timer* SetupTimer(float timeDuration)
{
	Timer* newTimer = nullptr;

	if (timeDuration == 0)
	{
		newTimer = new Timer(Clock::GetSystemClock().GetDeltaSeconds());
		newTimer->Start();
	}
	else if (timeDuration != -1)
	{
		newTimer = new Timer(timeDuration);
		newTimer->Start();
	}

	return newTimer;
}
