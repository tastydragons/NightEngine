/*!***********************************************************************
 * @file VertexArray.h
 * @author Aloysius Liong Yu Xuan
 * @par DP email: a.liong@digipen.edu
 * @par Course: CSD2401F23-B
 * @par Milestone 1
 * @date 27-9-2023
 * @brief Declaration of the VertexArray class for managing vertex data.
 *
 * This header file contains the declaration of the VertexArray class.
 * The VertexArray class is responsible for managing vertex data by
 * adding vertex buffers and index buffers to the OpenGL vertex array
 * object and configuring vertex attributes for rendering.
*************************************************************************/

#pragma once

#include "Buffer.h"

namespace Night {
	class VertexArray {
	public: 
		VertexArray() : m_rendererID(0) {
			glCreateVertexArrays(1, &m_rendererID);
		}
		~VertexArray() { glDeleteVertexArrays(0, &m_rendererID); }

		virtual void Bind() const { glBindVertexArray(m_rendererID); }
		virtual void Unbind() const { glBindVertexArray(0); }

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffer() const { return m_vertexBuffer; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_indexBuffer; }

	private:
		uint32_t m_rendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffer;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
	};
}