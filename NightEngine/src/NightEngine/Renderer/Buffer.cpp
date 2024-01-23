#include "hzpch.h"
#include "NightEngine/Renderer/Buffer.h"

#include "NightEngine/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Night
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		NIGHT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(size);
		}

		NIGHT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		NIGHT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		NIGHT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		NIGHT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLIndexBuffer>(indices, size);
		}

		NIGHT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}