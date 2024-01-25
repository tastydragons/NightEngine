/*!***********************************************************************
 * @file VertexBuffer.h
 * @author Aloysius Liong Yu Xuan
 * @par DP email: a.liong@digipen.edu
 * @par Course: CSD2401F23-B
 * @par Milestone 1
 * @date 27-9-2023
 * @brief Declaration of the VertexBuffer and IndexBuffer classes.
 *
 * This header file contains the declarations of the VertexBuffer and
 * IndexBuffer classes, which are responsible for managing vertex and
 * index data and interacting with OpenGL to create and manage GPU buffers.
*************************************************************************/

#pragma once

#include "Buffer.h"

namespace Night {
	class VBuffer : public Night::VertexBuffer
	{
	public:
		VBuffer(uint32_t size);
		VBuffer(float* vertices, uint32_t size);
		virtual ~VBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const BufferLayout& GetLayout() const override { return m_layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_layout = layout; }

	private:
		uint32_t m_rendererID;
		BufferLayout m_layout;
	};

	class IBuffer : public IndexBuffer
	{
	public:
		IBuffer(uint32_t* indices, uint32_t count);
		virtual ~IBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_rendererID;
		uint32_t m_Count;

	};
}

