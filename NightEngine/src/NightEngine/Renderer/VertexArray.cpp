/*!***********************************************************************
 * @file VertexArray.cpp
 * @author Aloysius Liong Yu Xuan
 * @par DP email: a.liong@digipen.edu
 * @par Course: CSD2401F23-B
 * @par Milestone 1
 * @date 27-9-2023
 * @brief Implementation of the VertexArray class for managing vertex data.
 *
 * This source file contains the implementation of the VertexArray class.
 * The VertexArray class is responsible for managing vertex data by
 * adding vertex buffers and index buffers to the OpenGL vertex array
 * object and configuring vertex attributes for rendering.
*************************************************************************/

#include "../Precompile/Precompiled.h"
#include "VertexArray.h"

namespace Night {
	/*!***********************************************************************
	\brief 
	Adds a vertex buffer to the VertexArray object.

	This function binds the VertexArray, binds the provided vertexBuffer,
	enables and configures vertex attributes based on the layout of the
	vertexBuffer, and stores the vertexBuffer in the VertexArray.

	\param vertexBuffer 
	A shared pointer to the VertexBuffer to be added.
	*************************************************************************/
	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
		glBindVertexArray(m_rendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				GL_FLOAT,
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				reinterpret_cast<const void*>(static_cast<uintptr_t>(element.offset)));

			index++;
		}

		m_vertexBuffer.push_back(vertexBuffer);
	}

	/*!***********************************************************************
	\brief 
	Sets the index buffer for the VertexArray object.

	This function binds the VertexArray and the provided indexBuffer and
	associates it with the VertexArray. The indexBuffer specifies the
	indices used to render the geometry.

	\param indexBuffer 
	A shared pointer to the IndexBuffer to be set.
	*************************************************************************/
	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
		glBindVertexArray(m_rendererID);
		indexBuffer->Bind();

		m_indexBuffer = indexBuffer;
	}
}