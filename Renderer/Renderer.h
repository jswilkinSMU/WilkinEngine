#pragma once
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Renderer/VertexBuffer.hpp"
#include "Engine/Renderer/ConstantBuffer.hpp"
#include "Engine/Renderer/IndexBuffer.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Game/EngineBuildPreferences.hpp"
#include <vector>
// -----------------------------------------------------------------------------
struct Rgba8;
struct Vertex_PCU;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11RasterizerState;
struct ID3DUserDefinedAnnotation;
class  Camera;
class  Window;
class  Image;
// -----------------------------------------------------------------------------
#define DX_SAFE_RELEASE(dxObject)	\
{									\
	if ((dxObject) != nullptr)		\
	{								\
		(dxObject)->Release();		\
		(dxObject) = nullptr;		\
	}								\
}
// -----------------------------------------------------------------------------
#if defined(OPAQUE)
#undef OPAQUE
#endif
//--------------------------------------------------------------------------------------------------------
struct RendererConfig
{
	Window* m_window = nullptr;
};
//--------------------------------------------------------------------------------------------------------
struct PerFrameConstants
{
	float m_time = 0.0f;
	int   m_debugInt = -1;
	float m_debugFloat = -1.0f;
	int   padding0 = 0;
};
static const int k_perFrameConstantsSlot = 1;
// -------------------------------------------------------------------------------------------------------
struct CameraConstants
{
	Mat44 WorldToCameraTransform;
	Mat44 CameraToRenderTransform;
	Mat44 RenderToClipTransform;
	Vec3  CameraPosition = Vec3::ZERO;
	float Padding = 0.0f;
};
static const int k_cameraConstantsSlot = 2;
// -------------------------------------------------------------------------------------------------------
struct ModelConstants
{
	Mat44 ModelToWorldTransform;
	float ModelColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
};
static const int k_modelConstantsSlot = 3;
// -------------------------------------------------------------------------------------------------------
struct PointLight
{
	Vec4  Position = Vec4(0.f, 0.f, 0.f, 0.f);
	float Color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
};
static_assert(sizeof(PointLight) == 32, "PointLight must be 32 bytes");
#define MAX_POINTLIGHTS 64
// -----------------------------------------------------------------------------
struct SpotLight
{
	Vec4  m_position = Vec4(0.f, 0.f, 0.f, 0.f); 
	Vec4  m_color	 = Vec4(0.f, 0.f, 0.f, 0.f); 

	Vec3  m_direction = Vec3::ZERO; 
	float m_innerRadius = 0.f;      

	float m_outerRadius = 0.f;                  
	float m_innerPenumbraDotThreshold = 0.f;  
	float m_outerPenumbraDotThreshold = 0.f;
	float PADDINGTON = 0.f;
};
static_assert(sizeof(SpotLight) == 64, "SpotLight must be 64 bytes");
#define MAX_SPOTLIGHTS 8
// -------------------------------------------------------------------------------------------------------
struct LightingConstants
{
	Vec3 m_sunDirection = Vec3::ZERO;
	float m_sunIntensity = 0.f;

	float m_ambientIntensity = 0.f;
	Vec3  padders = Vec3::ZERO;

	int NumPointLights = 0;
	Vec3 pointPadders = Vec3::ZERO;
	PointLight PointLights[MAX_POINTLIGHTS];

	int NumSpotLights = 0;
	Vec3 spotPadders = Vec3::ZERO;
	SpotLight SpotLights[MAX_SPOTLIGHTS];
};
static_assert(sizeof(LightingConstants) % 16 == 0, "LightingConstants must be 16-byte aligned");
static const int k_lightingConstantsSlot = 4;
// -------------------------------------------------------------------------------------------------------
enum class BlendMode
{
	ALPHA,
	ADDITIVE,
	OPAQUE,
	COUNT
};
// -------------------------------------------------------------------------------------------------------
enum class SamplerMode
{
	POINT_CLAMP,
	BILINEAR_WRAP,
	COUNT
};
// -------------------------------------------------------------------------------------------------------
enum class RasterizerMode
{
	SOLID_CULL_NONE,
	SOLID_CULL_BACK,
	WIREFRAME_CULL_NONE,
	WIREFRAME_CULL_BACK,
	COUNT
};
// -------------------------------------------------------------------------------------------------------
enum class DepthMode
{
	DISABLED,
	READ_ONLY_ALWAYS,
	READ_ONLY_LESS_EQUAL,
	READ_WRITE_LESS_EQUAL,
	COUNT
};
// -------------------------------------------------------------------------------------------------------
enum class VertexType
{
	VERTEX_PCU,
	VERTEX_PCUTBN,
	COUNT
};
// -------------------------------------------------------------------------------------------------------
class Renderer
{
public:
	Renderer(RendererConfig const& config);
	~Renderer();
	void Startup();
	void BeginFrame();
	void EndFrame();
	void Shutdown();

	void ClearScreen(const Rgba8& clearColor);
	void BeginCamera(const Camera& camera);
	void EndCamera(const Camera& camera);
	void DrawVertexArray(int numVertexes, const Vertex_PCU* vertexes);
	void DrawVertexArray(std::vector<Vertex_PCU> const& verts);
	void DrawVertexArray(std::vector<Vertex_PCUTBN> const& verts);

	Texture* GetTextureForFileName(char const* imageFilePath);
	Shader*  GetShaderForFileName(char const* shaderName);
	BitmapFont* GetBitmapFontForFileName(char const* bitmapFontFilePathWithNoExtension);

	void SetBlendMode(BlendMode blendMode);
	void SetSamplerMode(SamplerMode samplerMode);
	void BindSampler(SamplerMode samplerMode, int slot = 0);
	void SetRasterizerMode(RasterizerMode rasterizerMode);
	void SetDepthMode(DepthMode depthMode);
	void SetModelConstants(Mat44 const& modelToWorldTransform = Mat44(), Rgba8 const& modelColor = Rgba8::WHITE);
	void SetLightingConstants(LightingConstants const& lightingConstants);
	void SetLightingConstants(Vec3 sunDirection = Vec3::ZERO, float sunIntensity = 0.0f, float ambientIntensity = 0.0f);
	void SetPerFrameConstants(int debugInt, float debugFloat);

public:
	Texture*		CreateTextureFromImage(Image const& image, unsigned int mipMapLevels = 1);
	void			BindTexture(Texture* texture, int slot = 0);
	Texture*		CreateOrGetTextureFromFile(char const* imageFilePath, unsigned int mipMapLevels = 1);
	BitmapFont*		CreateOrGetBitmapFont(char const* bitmapFontFilePathWithNoExtension);
	Shader*			CreateOrGetShader(char const* shaderName, VertexType vertexType = VertexType::VERTEX_PCU);
	Shader*			CreateShader(char const* shaderName, VertexType vertexType = VertexType::VERTEX_PCU);
	Shader*			CreateShader(char const* shaderName, char const* shaderSource, VertexType vertexType = VertexType::VERTEX_PCU);
	void			BindShader(Shader* shader);
	VertexBuffer*	CreateVertexBuffer(const unsigned int size, unsigned int stride);
	ConstantBuffer* CreateConstantBuffer(const unsigned int size);
	IndexBuffer*	CreateIndexBuffer(const unsigned int size, unsigned int stride);
	void			BindVertexBuffer(VertexBuffer* vbo);
	void			DrawVertexBuffer(VertexBuffer* vbo, unsigned int vertexCount);
	void			BindConstantBuffer(int slot, ConstantBuffer* cbo);
	void			DrawIndexedVertexBuffer(VertexBuffer* vbo, IndexBuffer* ibo, unsigned int indexCount);
	void			BindIndexBuffer(IndexBuffer* ibo);
	void			SetStatesIfChanged();

	void			BeginRenderEvent(char const* eventName);
	void			EndRenderEvent(char const* optional_eventName);

	bool CompileShaderToByteCode(std::vector<unsigned char>& outByteCode, char const* name,
		char const* source, char const* entryPoint, char const* target);
	void CopyCPUToGPU(const void* data, unsigned int size, VertexBuffer* vbo);
	void CopyCPUToGPU(const void* data, unsigned int size, ConstantBuffer* cbo);
	void CopyCPUToGPU(const void* data, unsigned int size, IndexBuffer* ibo);
private:
	Texture* CreateTextureFromFile(char const* imageFilePath, unsigned int mipLevels = 1);

private:
	std::vector<Texture*> m_loadedTextures;
	std::vector<BitmapFont*> m_loadedFonts;
	std::vector<Shader*> m_loadedShaders;
	Shader* m_currentShader = nullptr;
	Shader* m_defaultShader = nullptr;
	Texture* m_defaultTexture = nullptr;
	Texture* m_defaultNormalMap = nullptr;
	Texture* m_defaultSpecGlosEmit = nullptr;
	VertexBuffer* m_immediateVBO = nullptr;
	VertexBuffer* m_immediateVBOTBN = nullptr;
	ConstantBuffer* m_perFrameCBO = nullptr;
	ConstantBuffer* m_cameraCBO = nullptr;
	ConstantBuffer* m_modelCBO = nullptr;
	ConstantBuffer* m_lightCBO = nullptr;
	BlendMode m_desiredBlendMode = BlendMode::ALPHA;
	SamplerMode m_desiredSamplerMode = SamplerMode::POINT_CLAMP;
	RasterizerMode m_desiredRasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	DepthMode m_desiredDepthMode = DepthMode::READ_WRITE_LESS_EQUAL;
	RendererConfig m_config;

protected:
	// Create variables to store DirectX state.
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
	IDXGISwapChain* m_swapChain = nullptr;
	ID3D11RenderTargetView* m_renderTargetView = nullptr;

	ID3DUserDefinedAnnotation* m_userDefinedAnnotations = nullptr;

	ID3D11BlendState* m_blendState = nullptr;
	ID3D11BlendState* m_blendStates[(int)(BlendMode::COUNT)] = {};
	ID3D11SamplerState* m_samplerState = nullptr;
	ID3D11SamplerState* m_samplerStates[(int)(SamplerMode::COUNT)] = {};
	ID3D11RasterizerState* m_rasterizerState = nullptr;
	ID3D11RasterizerState* m_rasterizerStates[(int)(RasterizerMode::COUNT)] = {};
	ID3D11DepthStencilState* m_depthState = nullptr;
	ID3D11DepthStencilState* m_depthStates[(int)(DepthMode::COUNT)] = {};

	ID3D11Texture2D* m_depthStencilTexture = nullptr;
	ID3D11DepthStencilView* m_depthStencilDSV = nullptr;
};