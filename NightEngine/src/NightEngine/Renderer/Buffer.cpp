/*!***********************************************************************
 * @file Buffer.cpp
 * @author Aloysius Liong Yu Xuan
 * @par DP email: a.liong@digipen.edu
 * @par Course: CSD2401F23-B
 * @par Milestone 1
 * @date 27-9-2023
 * @brief Implementation file for buffer-related classes and definitions.
 *
 * This file contains the implementation for several classes and
 * enumerations related to buffers, including VertexBuffer and IndexBuffer.
 * It also includes factory functions and structures for handling buffer
 * layouts and data types.
*************************************************************************/

#include "Buffer.h"
#include "VertexBuffer.h"
#include "hzpch.h"

namespace Night {
	/*!***********************************************************************
	\brief Creates a new VertexBuffer with the specified size.
	
	This static method creates a new VertexBuffer with the provided size
	in bytes. It is typically used to create dynamic VertexBuffers that can
	be updated with data later.
	
	\param size 
	The size in bytes for the VertexBuffer.

	\return 
	A pointer to the newly created VertexBuffer.
	*************************************************************************/
	VertexBuffer* VertexBuffer::Create(uint32_t size)
	{
		return new VBuffer(size);
	}

	/*!***********************************************************************
	\brief
	Factory function to create a Vertex Buffer.

	This factory function creates a new Vertex Buffer and initializes it
	with the provided vertex data. It returns a pointer to the newly
	created Vertex Buffer.

	\param vertices
	A pointer to the vertex data.

	\param size
	The size in bytes of the vertex data.

	\return
	A pointer to the newly created Vertex Buffer.
	*************************************************************************/
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		return new VBuffer(vertices, size);
	}

	/*!***********************************************************************
	\brief
	Factory function to create an Index Buffer.

	This factory function creates a new Index Buffer and initializes it
	with the provided index data. It returns a pointer to the newly
	created Index Buffer.

	\param indices
	A pointer to the index data.

	\param count
	The size in bytes of the index data.

	\return
	A pointer to the newly created Index Buffer.
	*************************************************************************/
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		return new IBuffer(indices, count);
	}
}