#include "ThirdParty/stb/stb_image.h"
//-----------------------------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN		// Always #define this before #including <windows.h>
#include <windows.h>			// #include this (massive, platform-specific) header in VERY few places (and .CPPs only)
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <dxgi.h>

// Link the following libraries using pragmas.
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// -----------------------------------------------------------------------------
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/DefaultShader.hpp"
#include "Engine/Renderer/Camera.h"
#include "Engine/Window/Window.hpp"
#include "Engine/Math/MathUtils.h"
#include "Engine/Core/Rgba8.h"
#include "Engine/Core/Image.hpp"
#include "Engine/Core/Vertex_PCU.h"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/EngineCommon.h"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Time.hpp"
// -----------------------------------------------------------------------------
#if defined(ENGINE_DEBUG_RENDER)
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif
#if defined(ENGINE_DEBUG_RENDER)
void* m_dxgiDebug = nullptr;
void* m_dxgiDebugModule = nullptr;
#endif
// -----------------------------------------------------------------------------
Renderer::Renderer(RendererConfig const& config)
	: m_config(config)
{
}

Renderer::~Renderer()
{
}

void Renderer::Startup()
{
	// Create debug module
#if defined (ENGINE_DEBUG_RENDER)
	m_dxgiDebugModule = (void*)::LoadLibraryA("dxgidebug.dll");
	if (m_dxgiDebugModule == nullptr)
	{
		ERROR_AND_DIE("Could not load dxgidebug.dll.");
	}

	typedef HRESULT(WINAPI* GetDebugModuleCB)(REFIID, void**);
	((GetDebugModuleCB)::GetProcAddress((HMODULE)m_dxgiDebugModule, "DXGIGetDebugInterface"))
		(__uuidof(IDXGIDebug), &m_dxgiDebug);

	if (m_dxgiDebug == nullptr)
	{
		ERROR_AND_DIE("Could not load debug module.");
	}
#endif

	// Create a local DXGI_SWAP_CHAIN_DESC variable and set its values as follows.
	unsigned int deviceFlags = 0;
#if defined(ENGINE_DEBUG_RENDER)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Create device and swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferDesc.Width = Window::s_mainWindow->GetClientDimensions().x;
	swapChainDesc.BufferDesc.Height = Window::s_mainWindow->GetClientDimensions().y;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = (HWND)Window::s_mainWindow->GetHwnd();
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// Call D3D11CreateDeviceAndSwapChain.
	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceFlags,
		nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc,
		&m_swapChain, &m_device, nullptr, &m_deviceContext);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create D3D 11 device and swap chain.");
	}

	// Save Back Buffer View
	// Get back buffer texture
	ID3D11Texture2D* backBuffer;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not get swap chain buffer.");
	}

	hr = m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create render target view for swap buffer chain.");
	}
	backBuffer->Release();

	// Create Rasterizer State SOLID_CULL_NONE
	D3D11_RASTERIZER_DESC rasterizerDesc = { };
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = true;
	hr = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerStates[(int)(RasterizerMode::SOLID_CULL_NONE)]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateRasterizerState for RasterizerMode::SOLID_CULL_NONE failed.");
	}

	// Create Rasterizer State SOLID_CULL_BACK
	D3D11_RASTERIZER_DESC solidCullrasterizerDesc = { };
	solidCullrasterizerDesc.FillMode = D3D11_FILL_SOLID;
	solidCullrasterizerDesc.CullMode = D3D11_CULL_BACK;
	solidCullrasterizerDesc.FrontCounterClockwise = true;
	solidCullrasterizerDesc.DepthBias = 0;
	solidCullrasterizerDesc.DepthBiasClamp = 0.0f;
	solidCullrasterizerDesc.SlopeScaledDepthBias = 0.0f;
	solidCullrasterizerDesc.DepthClipEnable = true;
	solidCullrasterizerDesc.ScissorEnable = false;
	solidCullrasterizerDesc.MultisampleEnable = false;
	solidCullrasterizerDesc.AntialiasedLineEnable = true;
	hr = m_device->CreateRasterizerState(&solidCullrasterizerDesc, &m_rasterizerStates[(int)(RasterizerMode::SOLID_CULL_BACK)]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateRasterizerState for RasterizerMode::SOLID_CULL_BACK failed.");
	}

	// Create Rasterizer State WIREFRAME_CULL_NONE
	D3D11_RASTERIZER_DESC wireframeCullNonerasterizerDesc = { };
	wireframeCullNonerasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeCullNonerasterizerDesc.CullMode = D3D11_CULL_NONE;
	wireframeCullNonerasterizerDesc.FrontCounterClockwise = true;
	wireframeCullNonerasterizerDesc.DepthBias = 0;
	wireframeCullNonerasterizerDesc.DepthBiasClamp = 0.0f;
	wireframeCullNonerasterizerDesc.SlopeScaledDepthBias = 0.0f;
	wireframeCullNonerasterizerDesc.DepthClipEnable = true;
	wireframeCullNonerasterizerDesc.ScissorEnable = false;
	wireframeCullNonerasterizerDesc.MultisampleEnable = false;
	wireframeCullNonerasterizerDesc.AntialiasedLineEnable = true;
	hr = m_device->CreateRasterizerState(&wireframeCullNonerasterizerDesc, &m_rasterizerStates[(int)(RasterizerMode::WIREFRAME_CULL_NONE)]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateRasterizerState for RasterizerMode::WIREFRAME_CULL_NONE failed.");
	}

	// Crate Rasterizer State WIREFRAME_CULL_BACK
	D3D11_RASTERIZER_DESC wireframeCullBackrasterizerDesc = { };
	wireframeCullBackrasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeCullBackrasterizerDesc.CullMode = D3D11_CULL_BACK;
	wireframeCullBackrasterizerDesc.FrontCounterClockwise = true;
	wireframeCullBackrasterizerDesc.DepthBias = 0;
	wireframeCullBackrasterizerDesc.DepthBiasClamp = 0.0f;
	wireframeCullBackrasterizerDesc.SlopeScaledDepthBias = 0.0f;
	wireframeCullBackrasterizerDesc.DepthClipEnable = true;
	wireframeCullBackrasterizerDesc.ScissorEnable = false;
	wireframeCullBackrasterizerDesc.MultisampleEnable = false;
	wireframeCullBackrasterizerDesc.AntialiasedLineEnable = true;
	hr = m_device->CreateRasterizerState(&wireframeCullBackrasterizerDesc, &m_rasterizerStates[(int)(RasterizerMode::WIREFRAME_CULL_BACK)]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateRasterizerState for RasterizerMode::WIREFRAME_CULL_BACK failed.");
	}

	// Set Default Shader
	m_defaultShader = CreateShader("Default", g_shaderSource);
	BindShader(m_defaultShader);

	// Set default texture
	Image defaultWhiteImage(IntVec2::ONE, Rgba8::WHITE);
	m_defaultTexture = CreateTextureFromImage(defaultWhiteImage);
	BindTexture(m_defaultTexture, 0);

	// Set default normal map
	Image defaultNormalMapImage(IntVec2::ONE, Rgba8(127, 127, 255));
	m_defaultNormalMap = CreateTextureFromImage(defaultNormalMapImage);
	BindTexture(m_defaultNormalMap, 1);

	// Set default specGlosEmit
	Image defaultSpecGlosEmitImage(IntVec2::ONE, Rgba8(127, 127, 0));
	m_defaultSpecGlosEmit = CreateTextureFromImage(defaultSpecGlosEmitImage);
	BindTexture(m_defaultSpecGlosEmit, 2);

	// Create immediate vertex buffer for Vertex_PCU
	m_immediateVBO = new VertexBuffer(m_device, sizeof(Vertex_PCU), sizeof(Vertex_PCU));
	m_immediateVBO->m_size = sizeof(Vertex_PCU);

	// Create immediate vertex buffer for Vertex_PCUTBN
	m_immediateVBOTBN = new VertexBuffer(m_device, sizeof(Vertex_PCUTBN), sizeof(Vertex_PCUTBN));
	m_immediateVBOTBN->m_size = sizeof(Vertex_PCUTBN);

	// Create constant buffer large enough to fit perFrame structure
	m_perFrameCBO = new ConstantBuffer(m_device, sizeof(PerFrameConstants));
	
	// Create constant buffer large enough to fit camera structure
	m_cameraCBO = new ConstantBuffer(m_device, sizeof(CameraConstants));

	// Create constant buffer large enough to fit model structure
	m_modelCBO = new ConstantBuffer(m_device, sizeof(ModelConstants));

	// Create constant buffer large enough to fit lighting structure
	m_lightCBO = new ConstantBuffer(m_device, sizeof(LightingConstants));

	// Create Opaque blend state
	D3D11_BLEND_DESC blendDesc = { };
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = blendDesc.RenderTarget[0].SrcBlend;
	blendDesc.RenderTarget[0].DestBlendAlpha = blendDesc.RenderTarget[0].DestBlend;
	blendDesc.RenderTarget[0].BlendOpAlpha = blendDesc.RenderTarget[0].BlendOp;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = m_device->CreateBlendState(&blendDesc, &m_blendStates[(int)(BlendMode::OPAQUE)]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateBlendState for BlendMode::OPAQUE failed.");
	}

	// Create Alpha blend state
	D3D11_BLEND_DESC alphaBlendDesc = { };
	alphaBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	alphaBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	alphaBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	alphaBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	alphaBlendDesc.RenderTarget[0].SrcBlendAlpha = alphaBlendDesc.RenderTarget[0].SrcBlend;
	alphaBlendDesc.RenderTarget[0].DestBlendAlpha = alphaBlendDesc.RenderTarget[0].DestBlend;
	alphaBlendDesc.RenderTarget[0].BlendOpAlpha = alphaBlendDesc.RenderTarget[0].BlendOp;
	alphaBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = m_device->CreateBlendState(&alphaBlendDesc, &m_blendStates[(int)(BlendMode::ALPHA)]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateBlendState for BlendMode::ALPHA failed.");
	}

	// Create Additive blend state
	D3D11_BLEND_DESC additiveBlendDesc = { };
	additiveBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	additiveBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	additiveBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	additiveBlendDesc.RenderTarget[0].SrcBlendAlpha = additiveBlendDesc.RenderTarget[0].SrcBlend;
	additiveBlendDesc.RenderTarget[0].DestBlendAlpha = additiveBlendDesc.RenderTarget[0].DestBlend;
	additiveBlendDesc.RenderTarget[0].BlendOpAlpha = additiveBlendDesc.RenderTarget[0].BlendOp;
	additiveBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = m_device->CreateBlendState(&additiveBlendDesc, &m_blendStates[(int)(BlendMode::ADDITIVE)]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateBlendState for BlendMode::ADDITIVE failed.");
	}

	// Create Point Clamp sampler state
	D3D11_SAMPLER_DESC samplerDesc = { };
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = m_device->CreateSamplerState(&samplerDesc, &m_samplerStates[(int)SamplerMode::POINT_CLAMP]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateSamplerState for SamplerMode::POINT_CLAMP failed.");
	}

	// Create Bilinear Wrap sampler state
	D3D11_SAMPLER_DESC bilinearSamplerDesc = { };
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = m_device->CreateSamplerState(&samplerDesc, &m_samplerStates[(int)SamplerMode::BILINEAR_WRAP]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateSamplerState for SamplerMode::BILINEAR_WRAP failed.");
	}

	// Query the interface to create user defined annotations
	hr = m_deviceContext->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), reinterpret_cast<void**>(&m_userDefinedAnnotations));
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create user defined annotations interface!");
	}

	// Create Depth Stencil Texture and View
	D3D11_TEXTURE2D_DESC depthTextureDesc = { };
	depthTextureDesc.Width = m_config.m_window->GetClientDimensions().x;
	depthTextureDesc.Height = m_config.m_window->GetClientDimensions().y;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.SampleDesc.Count = 1;

	hr = m_device->CreateTexture2D(&depthTextureDesc, nullptr, &m_depthStencilTexture);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create texture for depth stencil.");
	}

	hr = m_device->CreateDepthStencilView(m_depthStencilTexture, nullptr, &m_depthStencilDSV);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create depth stencil view.");
	}

	// Create DepthMode DISABLED
	D3D11_DEPTH_STENCIL_DESC disabledDepthStencilDesc = { };
	hr = m_device->CreateDepthStencilState(&disabledDepthStencilDesc, &m_depthStates[(int)DepthMode::DISABLED]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateDepthStencilState for DepthMode::DISABLED failed.");
	}

	// Create DepthMode READ_ONLY_ALWAYS
	D3D11_DEPTH_STENCIL_DESC readOnlyAlwaysDepthStencilDesc = { };
	readOnlyAlwaysDepthStencilDesc.DepthEnable = TRUE;
	readOnlyAlwaysDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	readOnlyAlwaysDepthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	hr = m_device->CreateDepthStencilState(&readOnlyAlwaysDepthStencilDesc, &m_depthStates[(int)DepthMode::READ_ONLY_ALWAYS]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateDepthStencilState for DepthMode::READ_ONLY_ALWAYS failed.");
	}

	// Create DepthMode READ_ONLY_LESS_EQUAL
	D3D11_DEPTH_STENCIL_DESC readOnlyLessEqualDepthStencilDesc = { };
	readOnlyLessEqualDepthStencilDesc.DepthEnable = TRUE;
	readOnlyLessEqualDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	readOnlyLessEqualDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = m_device->CreateDepthStencilState(&readOnlyLessEqualDepthStencilDesc, &m_depthStates[(int)DepthMode::READ_ONLY_LESS_EQUAL]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateDepthStencilState for DepthMode::READ_ONLY_LESS_EQUAL failed.");
	}

	// Create DepthMode READ_WRITE_LESS_EQUAL
	D3D11_DEPTH_STENCIL_DESC readWriteLessEqualDepthStencilDesc = { };
	readWriteLessEqualDepthStencilDesc.DepthEnable = TRUE;
	readWriteLessEqualDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	readWriteLessEqualDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = m_device->CreateDepthStencilState(&readWriteLessEqualDepthStencilDesc, &m_depthStates[(int)DepthMode::READ_WRITE_LESS_EQUAL]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateDepthStencilState for DepthMode::READ_WRITE_LESS_EQUAL failed.");
	}
}

void Renderer::BeginFrame()
{
	// Set PerFrame Constant Buffer
	SetPerFrameConstants(0, 0.f);

	// Set Render Target
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilDSV);
}

void Renderer::EndFrame()
{
	// Present the swap chain
	HRESULT hr;
	hr = m_swapChain->Present(0, 0);
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		ERROR_AND_DIE("Device has been lost, application will now terminate.");
	}
}

void Renderer::Shutdown()
{
	// Delete all textures in the cache
	for (size_t textureIndex = 0; textureIndex < m_loadedTextures.size(); ++textureIndex)
	{
		if (m_loadedTextures[textureIndex] != nullptr)
		{
			delete m_loadedTextures[textureIndex];
			m_loadedTextures[textureIndex] = nullptr;
		}
	}
	m_loadedTextures.clear();

	// Delete all fonts in the cache
	for (size_t fontIndex = 0; fontIndex < m_loadedFonts.size(); ++fontIndex)
	{
		if (m_loadedFonts[fontIndex] != nullptr)
		{
			delete m_loadedFonts[fontIndex];
			m_loadedFonts[fontIndex] = nullptr;
		}
	}
	m_loadedFonts.clear();

	// Delete all shaders in the cache
	for (size_t shaderIndex = 0; shaderIndex < m_loadedShaders.size(); ++shaderIndex)
	{
		if (m_loadedShaders[shaderIndex] != nullptr)
		{
			delete m_loadedShaders[shaderIndex];
			m_loadedShaders[shaderIndex] = nullptr;
		}
	}
	m_loadedShaders.clear();

	// Set defaults to nullptr
	m_defaultTexture = nullptr;
	m_defaultNormalMap = nullptr;
	m_defaultSpecGlosEmit = nullptr;
	m_defaultShader  = nullptr;
	m_currentShader  = nullptr;

	// Delete the immediate vertex buffer
	delete m_immediateVBO;
	m_immediateVBO = nullptr;
	delete m_immediateVBOTBN;
	m_immediateVBOTBN = nullptr;

	// Delete the perFrame constant buffer
	delete m_perFrameCBO;
	m_perFrameCBO = nullptr;

	// Delete the camera constant buffer
	delete m_cameraCBO;
	m_cameraCBO = nullptr;

	// Delete the model constant buffer
	delete m_modelCBO;
	m_modelCBO = nullptr;

	// Delete the lighting constant buffer
	delete m_lightCBO;
	m_lightCBO = nullptr;

	// Releasing the blend states
	for (int blendStateIndex = 0; blendStateIndex < (int)(BlendMode::COUNT); ++blendStateIndex)
	{
		DX_SAFE_RELEASE(m_blendStates[blendStateIndex]);
	}

	// Releasing the sampler states
	for (int samplerStateIndex = 0; samplerStateIndex < (int)(SamplerMode::COUNT); ++samplerStateIndex)
	{
		DX_SAFE_RELEASE(m_samplerStates[samplerStateIndex]);
	}

	// Releasing the rasterizer states
	for (int rasterizerStateIndex = 0; rasterizerStateIndex < (int)(RasterizerMode::COUNT); ++rasterizerStateIndex)
	{
		DX_SAFE_RELEASE(m_rasterizerStates[rasterizerStateIndex]);
	}

	// Releasing the depth stencil states
	for (int depthStateIndex = 0; depthStateIndex < (int)(DepthMode::COUNT); ++depthStateIndex)
	{
		DX_SAFE_RELEASE(m_depthStates[depthStateIndex]);
	}

	DX_SAFE_RELEASE(m_userDefinedAnnotations);
	DX_SAFE_RELEASE(m_depthStencilTexture);
	DX_SAFE_RELEASE(m_depthStencilDSV);
	DX_SAFE_RELEASE(m_renderTargetView);
	DX_SAFE_RELEASE(m_swapChain);
	DX_SAFE_RELEASE(m_deviceContext);
	DX_SAFE_RELEASE(m_device);

	// Report error leaks and release debug module
#if defined(ENGINE_DEBUG_RENDER)
	((IDXGIDebug*)m_dxgiDebug)->ReportLiveObjects(
		DXGI_DEBUG_ALL,
		(DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL)
	);

	((IDXGIDebug*)m_dxgiDebug)->Release();
	m_dxgiDebug = nullptr;

	::FreeLibrary((HMODULE)m_dxgiDebugModule);
	m_dxgiDebugModule = nullptr;
#endif
}


void Renderer::ClearScreen(const Rgba8& clearColor)
{
	float colorAsFloats[4];
	clearColor.GetAsFloats(colorAsFloats);
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, colorAsFloats);
	m_deviceContext->ClearDepthStencilView(m_depthStencilDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::BeginCamera(const Camera& camera)
{
	// Setting model constants to their defaults
	SetModelConstants();

	float viewportWidth = camera.m_normalizedViewport.m_maxs.x - camera.m_normalizedViewport.m_mins.x;
	float viewportHeight = camera.m_normalizedViewport.m_maxs.y - camera.m_normalizedViewport.m_mins.y;

	// Set Viewport
	D3D11_VIEWPORT viewport = { };
	viewport.TopLeftX = RangeMap(camera.m_normalizedViewport.m_mins.x, 0.f, 1.f, 0.f, (float)Window::s_mainWindow->GetClientDimensions().x);
	viewport.TopLeftY = RangeMap(1.f - camera.m_normalizedViewport.m_maxs.y, 0.f, 1.f, 0.f, (float)Window::s_mainWindow->GetClientDimensions().y);
	viewport.Width    = RangeMap(viewportWidth, 0.f, 1.f, 0.f, (float)Window::s_mainWindow->GetClientDimensions().x);
	viewport.Height   = RangeMap(viewportHeight, 0.f, 1.f, 0.f, (float)Window::s_mainWindow->GetClientDimensions().y);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_deviceContext->RSSetViewports(1, &viewport);

	// Create a local CameraConstants structure
	CameraConstants localCamera = { };
	localCamera.WorldToCameraTransform = camera.GetWorldToCameraTransform();
	localCamera.CameraToRenderTransform = camera.GetCameraToRenderTransform();
	localCamera.RenderToClipTransform = camera.GetRenderToClipTransform();
	localCamera.CameraPosition = camera.GetPosition();

	// Copy data from local structure to constant buffer and then bind
	CopyCPUToGPU(&localCamera, sizeof(localCamera), m_cameraCBO);
	BindConstantBuffer(k_cameraConstantsSlot, m_cameraCBO);
}

void Renderer::EndCamera(const Camera& camera)
{
	UNUSED(camera);
}

void Renderer::DrawVertexArray(int numVertexes, const Vertex_PCU* vertexes) 
{
	m_immediateVBO->m_stride = sizeof(Vertex_PCU);
	CopyCPUToGPU(vertexes, numVertexes * sizeof(Vertex_PCU), m_immediateVBO);
	DrawVertexBuffer(m_immediateVBO, numVertexes);
}

void Renderer::DrawVertexArray(std::vector<Vertex_PCU> const& verts)
{
	DrawVertexArray(static_cast<int>(verts.size()), verts.data());
}

void Renderer::DrawVertexArray(std::vector<Vertex_PCUTBN> const& verts)
{
	m_immediateVBOTBN->m_stride = sizeof(Vertex_PCUTBN);
	CopyCPUToGPU(verts.data(), static_cast<int>(verts.size()) * sizeof(Vertex_PCUTBN), m_immediateVBOTBN);
	DrawVertexBuffer(m_immediateVBOTBN, static_cast<int>(verts.size()));
}

Texture* Renderer::GetTextureForFileName(char const* imageFilePath)
{
	for (int textureIndex = 0; textureIndex < static_cast<int>(m_loadedTextures.size()); ++textureIndex)
	{
		if (m_loadedTextures[textureIndex]->m_name == static_cast<std::string>(imageFilePath))
		{
			return m_loadedTextures[textureIndex];
		}
	}
	return nullptr;
}

Shader* Renderer::GetShaderForFileName(char const* shaderName)
{
	for (int shaderIndex = 0; shaderIndex < static_cast<int>(m_loadedShaders.size()); ++shaderIndex)
	{
		if (m_loadedShaders[shaderIndex]->GetName() == static_cast<std::string>(shaderName))
		{
			return m_loadedShaders[shaderIndex];
		}
	}
	return nullptr;
}

BitmapFont* Renderer::GetBitmapFontForFileName(char const* bitmapFontFilePathWithNoExtension)
{
	for (int fontIndex = 0; fontIndex < static_cast<int>(m_loadedFonts.size()); ++fontIndex)
	{
		if (m_loadedFonts[fontIndex]->m_fontFilePathNameWithNoExtension == static_cast<std::string>(bitmapFontFilePathWithNoExtension))
		{
			return m_loadedFonts[fontIndex];
		}
	}
	return nullptr;
}

void Renderer::SetBlendMode(BlendMode blendMode)
{
	m_desiredBlendMode = blendMode;
}

void Renderer::SetSamplerMode(SamplerMode samplerMode)
{
	m_desiredSamplerMode = samplerMode;
}

void Renderer::BindSampler(SamplerMode samplerMode, int slot)
{
	ID3D11SamplerState* sampler = m_samplerStates[static_cast<int>(samplerMode)];
	m_deviceContext->PSSetSamplers(slot, 1, &sampler);
}

void Renderer::SetRasterizerMode(RasterizerMode rasterizerMode)
{
	m_desiredRasterizerMode = rasterizerMode;
}

void Renderer::SetDepthMode(DepthMode depthMode)
{
	m_desiredDepthMode = depthMode;
}

void Renderer::SetModelConstants(Mat44 const& modelToWorldTransform, Rgba8 const& modelColor)
{
	ModelConstants modelConstants;

	modelConstants.ModelToWorldTransform = modelToWorldTransform;
	modelColor.GetAsFloats(modelConstants.ModelColor);

	CopyCPUToGPU(&modelConstants, static_cast<unsigned char>(m_modelCBO->m_size), m_modelCBO);
	BindConstantBuffer(k_modelConstantsSlot, m_modelCBO);
}

void Renderer::SetLightingConstants(LightingConstants const& lightingConstants)
{
	CopyCPUToGPU((void*) &lightingConstants, sizeof(lightingConstants), m_lightCBO);
	BindConstantBuffer(k_lightingConstantsSlot, m_lightCBO);
}

void Renderer::SetLightingConstants(Vec3 sunDirection, float sunIntensity, float ambientIntensity)
{
	LightingConstants lighting;

	lighting.m_sunDirection = sunDirection;
	lighting.m_sunIntensity = GetClamped(sunIntensity, 0.f, 1.f);
	lighting.m_ambientIntensity = GetClamped(ambientIntensity, 0.f, 1.f);

	CopyCPUToGPU(&lighting, sizeof(LightingConstants), m_lightCBO);
	BindConstantBuffer(k_lightingConstantsSlot, m_lightCBO);
}

void Renderer::SetPerFrameConstants(int debugInt, float debugFloat)
{
	PerFrameConstants perFrame;

	perFrame.m_time = static_cast<float>(GetCurrentTimeSeconds());
	perFrame.m_debugInt = debugInt;
	perFrame.m_debugFloat = debugFloat;

	CopyCPUToGPU(&perFrame, sizeof(PerFrameConstants), m_perFrameCBO);
	BindConstantBuffer(k_perFrameConstantsSlot, m_perFrameCBO);
}

Texture* Renderer::CreateTextureFromImage(Image const& image, unsigned int mipMapLevels)
{
	Texture* newTexture = new Texture();

	IntVec2 dims = image.GetDimensions();
	unsigned int width = dims.x;
	unsigned int height = dims.y;

	D3D11_TEXTURE2D_DESC textureDesc = { };
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = mipMapLevels;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	ID3D11Texture2D* textureHandle = nullptr;
	HRESULT hr = m_device->CreateTexture2D(&textureDesc, nullptr, &textureHandle);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("CreateTextureFromImage failed for image file \"&s\".", image.GetImageFilePath().c_str()));
	}

	// Uploading base level
	m_deviceContext->UpdateSubresource(textureHandle, 0, nullptr, image.GetRawData(), width * 4, 0);

	// Creating the srv
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = mipMapLevels;

	ID3D11ShaderResourceView* srv = nullptr;
	hr = m_device->CreateShaderResourceView(textureHandle, &srvDesc, &srv);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("CreateShaderResourceView failed for image file \"&s\".", image.GetImageFilePath().c_str()));
	}

	if (mipMapLevels > 1)
	{
		m_deviceContext->GenerateMips(srv);
	}
	
	newTexture->m_texture = textureHandle;
	newTexture->m_shaderResourceView = srv;
	newTexture->m_dimensions = dims;
	newTexture->m_mipMapLevels = mipMapLevels;

	m_loadedTextures.push_back(newTexture);
	return newTexture;
}

//-----------------------------------------------------------------------------------------------
void Renderer::BindTexture(Texture* texture, int slot)
{
	if (!texture)
	{
		m_deviceContext->PSSetShaderResources(0, 1, &m_defaultTexture->m_shaderResourceView);
		m_deviceContext->PSSetShaderResources(1, 1, &m_defaultNormalMap->m_shaderResourceView);
		m_deviceContext->PSSetShaderResources(2, 1, &m_defaultSpecGlosEmit->m_shaderResourceView);
	}
	else
	{
		m_deviceContext->PSSetShaderResources(slot, 1, &texture->m_shaderResourceView);
	}
}

//------------------------------------------------------------------------------------------------
Texture* Renderer::CreateOrGetTextureFromFile(char const* imageFilePath, unsigned int mipMapLevels)
{
	// See if we already have this texture previously loaded
	Texture* existingTexture = GetTextureForFileName(imageFilePath);
	if (existingTexture)
	{
		return existingTexture;
	}

	// Never seen this texture before!  Let's load it.
	Texture* newTexture = CreateTextureFromFile(imageFilePath, mipMapLevels);
	return newTexture;
}

BitmapFont* Renderer::CreateOrGetBitmapFont(char const* bitmapFontFilePathWithNoExtension)
{
	BitmapFont* existingBitmapFont = GetBitmapFontForFileName(bitmapFontFilePathWithNoExtension);
	if (existingBitmapFont)
	{
		return existingBitmapFont;
	}
	std::string textureFilePath = Stringf("%s.png", bitmapFontFilePathWithNoExtension);
	Texture* newTexture = CreateTextureFromFile(textureFilePath.c_str());
	BitmapFont* newFont = new BitmapFont(bitmapFontFilePathWithNoExtension, *newTexture);
	m_loadedFonts.push_back(newFont);
	return newFont;
}

Shader* Renderer::CreateOrGetShader(char const* shaderName, VertexType vertexType)
{
	// See if we already have this shader previously loaded
	Shader* existingShader = GetShaderForFileName(shaderName); 
	if (existingShader)
	{
		return existingShader;
	}

	// Never seen this shader before!  Let's load it.
	Shader* newShader = CreateShader(shaderName, vertexType);
	return  newShader;
}

Shader* Renderer::CreateShader(char const* shaderName, VertexType vertexType)
{
	std::string sourceFile;
	FileReadToString(sourceFile, (static_cast<std::string>(shaderName) + ".hlsl").c_str());

	char const* source = sourceFile.c_str();
	Shader* shader = CreateShader(shaderName, source, vertexType);

	return shader;
}

//------------------------------------------------------------------------------------------------
Texture* Renderer::CreateTextureFromFile(char const* imageFilePath, unsigned int mipLevels)
{
	Image fileImage(imageFilePath);
	return CreateTextureFromImage(fileImage, mipLevels);
}

Shader* Renderer::CreateShader(char const* shaderName, char const* shaderSource, VertexType vertexType)
{
	// Create a new Shader config and create a new shader with it
	ShaderConfig shaderConfig;
	shaderConfig.m_name = shaderName;

	// Compile Vertex Shader
	std::vector<unsigned char> vertexShaderBytes;
	if (!CompileShaderToByteCode(vertexShaderBytes, shaderName, shaderSource, "VertexMain", "vs_5_0"))
	{
		ERROR_AND_DIE(Stringf("Could not compile vertex shader."));
	}

	// Create Vertex Shader
	ID3D11VertexShader* vertexShader = nullptr;
	HRESULT hr = m_device->CreateVertexShader(vertexShaderBytes.data(), vertexShaderBytes.size(), nullptr, &vertexShader);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Could not create vertex shader."));
	}

	// Compile Pixel Shader
	std::vector<unsigned char> pixelShaderBytes;
	if (!CompileShaderToByteCode(pixelShaderBytes, shaderName, shaderSource, "PixelMain", "ps_5_0"))
	{
		ERROR_AND_DIE(Stringf("Could not compile pixel shader."));
	}

	// Create Pixel Shader
	ID3D11PixelShader* pixelShader = nullptr;
	hr = m_device->CreatePixelShader(pixelShaderBytes.data(), pixelShaderBytes.size(), nullptr, &pixelShader);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Could not create pixel shader."));
	}

	ID3D11InputLayout* inputLayout = nullptr;

	// Create local array of Input Elements
	if (vertexType == VertexType::VERTEX_PCU)
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
				0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM,
				0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
				0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		// Create Input Layout
		UINT numElements = ARRAYSIZE(inputElementDesc);
		hr = m_device->CreateInputLayout(inputElementDesc, numElements, vertexShaderBytes.data(), vertexShaderBytes.size(), &inputLayout);
		if (!SUCCEEDED(hr))
		{
			ERROR_AND_DIE("Could not create input layout");
		}
	}
	else if (vertexType == VertexType::VERTEX_PCUTBN)
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		// Create Input Layout
		UINT numElements = ARRAYSIZE(inputElementDesc);
		hr = m_device->CreateInputLayout(inputElementDesc, numElements, vertexShaderBytes.data(), vertexShaderBytes.size(), &inputLayout);
		if (!SUCCEEDED(hr))
		{
			ERROR_AND_DIE("Could not create input layout");
		}
	}

	// Adding the newly created shader to cache
	Shader* newShader = new Shader(shaderConfig);
	newShader->m_vertexShader = vertexShader;
	newShader->m_pixelShader = pixelShader;
	newShader->m_inputLayout = inputLayout;
	m_loadedShaders.push_back(newShader);
	return newShader;
}

bool Renderer::CompileShaderToByteCode(std::vector<unsigned char>& outByteCode, char const* name, char const* source, char const* entryPoint, char const* target)
{
	DWORD shaderFlags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#if defined(ENGINE_DEBUG_RENDER)
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	shaderFlags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
#endif
	ID3DBlob* shaderBlob = NULL;
	ID3DBlob* errorBlob = NULL;

	// Call D3DCompile
	HRESULT hr = D3DCompile(
		source, strlen(source),
		name, nullptr, nullptr,
		entryPoint, target, shaderFlags,
		0, &shaderBlob, &errorBlob);
	if (!SUCCEEDED(hr))
	{
		if (errorBlob)
		{
			DebuggerPrintf((char*)errorBlob->GetBufferPointer());
		}
		return false;
	}

	// Copy shader byte code to outByte code
	outByteCode.resize(shaderBlob->GetBufferSize());
	memcpy(outByteCode.data(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());
	shaderBlob->Release();
	if (errorBlob)
	{
		errorBlob->Release();
	}
	return true;
}

void Renderer::CopyCPUToGPU(const void* data, unsigned int size, VertexBuffer* vbo)
{
	if (size > vbo->GetSize())
	{
		vbo->Resize(size);
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	m_deviceContext->Map(vbo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, data, size);
	m_deviceContext->Unmap(vbo->m_buffer, 0);
}

void Renderer::CopyCPUToGPU(const void* data, unsigned int size, ConstantBuffer* cbo)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	m_deviceContext->Map(cbo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, data, size);
	m_deviceContext->Unmap(cbo->m_buffer, 0);
}

void Renderer::CopyCPUToGPU(const void* data, unsigned int size, IndexBuffer* ibo)
{
	if (ibo->GetSize() < size)
	{
		ibo->Resize(size);
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	m_deviceContext->Map(ibo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, data, size);
	m_deviceContext->Unmap(ibo->m_buffer, 0);
}

void Renderer::BindShader(Shader* shader)
{
	if (shader == nullptr)
	{
		m_deviceContext->VSSetShader(m_defaultShader->m_vertexShader, nullptr, 0);
		m_deviceContext->PSSetShader(m_defaultShader->m_pixelShader, nullptr, 0);
		m_deviceContext->IASetInputLayout(m_defaultShader->m_inputLayout);
		return;
	}
	m_deviceContext->VSSetShader(shader->m_vertexShader, nullptr, 0);
	m_deviceContext->PSSetShader(shader->m_pixelShader, nullptr, 0);
	m_deviceContext->IASetInputLayout(shader->m_inputLayout);
}

VertexBuffer* Renderer::CreateVertexBuffer(const unsigned int size, unsigned int stride)
{
	VertexBuffer* vertexBuffer = new VertexBuffer(m_device, size, stride);
	return vertexBuffer;
}

ConstantBuffer* Renderer::CreateConstantBuffer(const unsigned int size)
{
	ConstantBuffer* constantBuffer = new ConstantBuffer(m_device, size);
	return constantBuffer;
}

IndexBuffer* Renderer::CreateIndexBuffer(const unsigned int size, unsigned int stride)
{
	IndexBuffer* indexBuffer = new IndexBuffer(m_device, size, stride);
	return indexBuffer;
}

void Renderer::BindVertexBuffer(VertexBuffer* vbo)
{
	UINT stride = vbo->GetStride();
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &vbo->m_buffer, &stride, &offset);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::DrawVertexBuffer(VertexBuffer* vbo, unsigned int vertexCount)
{
	// Bind
	BindVertexBuffer(vbo);

	// Draw
	SetStatesIfChanged();
	m_deviceContext->Draw(vertexCount, 0);
}

void Renderer::BindConstantBuffer(int slot, ConstantBuffer* cbo)
{
	m_deviceContext->VSSetConstantBuffers(slot, 1, &cbo->m_buffer);
	m_deviceContext->PSSetConstantBuffers(slot, 1, &cbo->m_buffer);
}

void Renderer::DrawIndexedVertexBuffer(VertexBuffer* vbo, IndexBuffer* ibo, unsigned int indexCount)
{
	// Bind
	BindVertexBuffer(vbo);
	BindIndexBuffer(ibo);

	// Draw
	SetStatesIfChanged();
	m_deviceContext->DrawIndexed(indexCount, 0, 0);
}

void Renderer::BindIndexBuffer(IndexBuffer* ibo)
{
	UINT offset = 0;
	m_deviceContext->IASetIndexBuffer(ibo->m_buffer, DXGI_FORMAT_R32_UINT, offset);
}

void Renderer::SetStatesIfChanged()
{
	// Setting blend state
	if (m_blendState != m_blendStates[(int)m_desiredBlendMode])
	{
		m_blendState = m_blendStates[(int)m_desiredBlendMode];

		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		UINT sampleMask = 0xffffffff;
		m_deviceContext->OMSetBlendState(m_blendState, blendFactor, sampleMask);
	}

	// Setting sampler state
	if (m_samplerState != m_samplerStates[(int)m_desiredSamplerMode])
	{
		m_samplerState = m_samplerStates[(int)m_desiredSamplerMode];

		// Set sampler state
		m_deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	}

	// Setting rasterizer state
	if (m_rasterizerState != m_rasterizerStates[(int)m_desiredRasterizerMode])
	{
		m_rasterizerState = m_rasterizerStates[(int)m_desiredRasterizerMode];
		m_deviceContext->RSSetState(m_rasterizerState);
	}

	// Setting depth state
	if (m_depthState != m_depthStates[(int)m_desiredDepthMode])
	{
		m_depthState = m_depthStates[(int)m_desiredDepthMode];
		m_deviceContext->OMSetDepthStencilState(m_depthState, 0);
	}
}

void Renderer::BeginRenderEvent(char const* eventName)
{
	int eventNameLength = (int)strlen(eventName) + 1;
	int eventNameWideCharLength = MultiByteToWideChar(CP_UTF8, 0, eventName, eventNameLength, NULL, 0);

	wchar_t* eventNameWideCharStr = new wchar_t[eventNameWideCharLength];
	MultiByteToWideChar(CP_UTF8, 0, eventName, eventNameLength, eventNameWideCharStr, eventNameWideCharLength);

	m_userDefinedAnnotations->BeginEvent(eventNameWideCharStr);
}

void Renderer::EndRenderEvent(char const* optional_eventName)
{
	UNUSED(optional_eventName);
	m_userDefinedAnnotations->EndEvent();
}
