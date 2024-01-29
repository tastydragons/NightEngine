#pragma once

#include "NightEngine/Renderer/Buffer.h"

namespace Night
{
	class VBuffer : public VertexBuffer
	{
	public:
		VBuffer(uint32_t size);
		VBuffer(float* data, unsigned int size);
		virtual ~VBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void SetBufferData(const void* data, unsigned int size);

		virtual const BufferLayout& GetLayout() const override { return mLayout; }
		virtual void  SetLayout(const BufferLayout& layout) override { mLayout = layout; }
	private:
		unsigned int mRenderID;
		BufferLayout mLayout;
	};


	class IBuffer : public IndexBuffer
	{
	public:
		IBuffer(unsigned int* indices, unsigned int count);
		virtual ~IBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual unsigned int GetCount() const { return mCount; }
	private:

		unsigned int mRenderID;
		unsigned int mCount;
	};
}