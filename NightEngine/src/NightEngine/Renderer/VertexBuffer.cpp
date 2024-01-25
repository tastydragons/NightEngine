/*!***********************************************************************
 * @file VertexBuffer.cpp
 * @author Aloysius Liong Yu Xuan
 * @par DP email: a.liong@digipen.edu
 * @par Course: CSD2401F23-B
 * @par Milestone 1
 * @date 27-9-2023
 * @brief Implementation of the VertexBuffer and IndexBuffer classes.
 *
 * This source file contains the implementation of the VertexBuffer and
 * IndexBuffer classes, which are responsible for managing vertex and
 * index data and interacting with OpenGL to create and manage GPU buffers.
*************************************************************************/

#include "VertexBuffer.h"
#include "../Precompile/Precompiled.h"

namespace Night {
	/*!***********************************************************************
	\brief Constructor for a Vertex Buffer object.

	This constructor creates a Vertex Buffer with a specified size and
	initializes it with null vertex data. It creates a GPU buffer, binds it,
	and allocates GPU memory for the buffer with the provided size.

	\param size
	The size in bytes of the Vertex Buffer.
	*************************************************************************/
	VBuffer::VBuffer(uint32_t size)
		: m_rendererID(0) 
	{
		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	/*!***********************************************************************
	\brief
	Constructor for a Vertex Buffer object.

	This constructor creates a Vertex Buffer and initializes it with the
	given vertex data. It creates a GPU buffer, binds it, and uploads
	the vertex data to the GPU.

	\param vertices
	A pointer to the vertex data.

	\param size
	The size in bytes of the vertex data.
	*************************************************************************/
	VBuffer::VBuffer(float* vertices, uint32_t size)
		: m_rendererID(0)
	{
		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	/*!***********************************************************************
	\brief
	Destructor for a Vertex Buffer object.

	This destructor releases the GPU resources associated with the Vertex
	Buffer object by deleting the buffer.
	*************************************************************************/
	VBuffer::~VBuffer() {
		glDeleteBuffers(1, &m_rendererID);
	}

	/*!***********************************************************************
	\brief
	Binds the Vertex Buffer object to the OpenGL context.

	This function binds the Vertex Buffer to the OpenGL context, allowing
	other OpenGL commands to operate on the currently bound buffer.
	*************************************************************************/
	void VBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
	}

	/*!***********************************************************************
	\brief
	Unbinds the currently bound Vertex Buffer object.

	This function unbinds the currently bound Vertex Buffer object from the
	OpenGL context, making it safe to perform other operations without
	accidentally affecting this buffer.
	*************************************************************************/
	void VBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	/*____________________INDEX BUFFER____________________*/
	/*!***********************************************************************
	\brief
	Constructor for an Index Buffer object.

	This constructor creates an Index Buffer and initializes it with the
	given index data. It creates a GPU buffer, binds it as an element
	array buffer, and uploads the index data to the GPU.

	\param indices
	A pointer to the index data.

	\param count
	The number of indices in the buffer.
	*************************************************************************/
	IBuffer::IBuffer(uint32_t* indices, uint32_t count) :m_rendererID(0), m_Count(count) {
		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);

	}

	/*!***********************************************************************
	\brief
	Destructor for an Index Buffer object.

	This destructor releases the GPU resources associated with the Index
	Buffer object by deleting the buffer.
	*************************************************************************/
	IBuffer::~IBuffer() {
		glDeleteBuffers(1, &m_rendererID);
	}

	/*!***********************************************************************
	\brief
	Binds the Index Buffer object to the OpenGL context.

	This function binds the Index Buffer to the OpenGL context as an
	element array buffer, allowing it to be used for rendering.
	*************************************************************************/
	void IBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
	}

	/*!***********************************************************************
	\brief
	Unbinds the currently bound Index Buffer object.

	This function unbinds the currently bound Index Buffer object from the
	OpenGL context, making it safe to perform other operations without
	accidentally affecting this buffer.
	*************************************************************************/
	void IBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}