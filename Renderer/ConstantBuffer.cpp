#include "Engine/Renderer/ConstantBuffer.hpp"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Core/ErrorWarningAssert.hpp"
// -----------------------------------------------------------------------------
ConstantBuffer::ConstantBuffer(ID3D11Device* device, size_t size)
	:m_device(device), m_size(size)
{
	Create();
}

ConstantBuffer::~ConstantBuffer()
{
	DX_SAFE_RELEASE(m_buffer);
}

void ConstantBuffer::Create()
{
	D3D11_BUFFER_DESC bufferDesc = { };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)m_size;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = m_device->CreateBuffer(&bufferDesc, nullptr, &m_buffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create constant buffer.");
	}
}

