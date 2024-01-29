#include "hzpch.h"

#include "NightEngine/Renderer/Buffer.h"
#include "NightEngine/Renderer/VertexBuffer.h"

namespace Night
{
	/*!*************************************************************************
	Dynamic draw buffer
	****************************************************************************/
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		return CreateRef<VBuffer>(size);
	}

	/*!*************************************************************************
	Static draw buffer
	****************************************************************************/
	Ref<VertexBuffer> VertexBuffer::Create(float* data, unsigned int size)
	{
		return CreateRef<VBuffer>(data, size);
	}

	/*!*************************************************************************
	draw buffer
	****************************************************************************/
	Ref<IndexBuffer> IndexBuffer::Create(unsigned int* indices, unsigned int count)
	{
		return CreateRef<IBuffer>(indices, count);
	}
}