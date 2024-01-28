#pragma once

#include "Buffer.h"

namespace Night
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();
		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexbuffer);
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return mVertexBuffers; }
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return mIndexBuffers; }
		static Ref<VertexArray> Create() { return CreateRef<VertexArray>(); }
	private:
		unsigned int mRenderID;
		unsigned int mVBIndex = 0;
		std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffers;
		std::shared_ptr<IndexBuffer> mIndexBuffers;
	};
}