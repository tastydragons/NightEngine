/*!***********************************************************************
 * @file Renderer.cpp
 * @author Aloysius Liong Yu Xuan
 * @par DP email: a.liong@digipen.edu
 * @par Course: CSD2401F23-B
 * @par Milestone 2
 * @date 2-11-2023
 * @brief Implementation of the FrameBuffer class.
 *
 * This file contains the implementation of the FrameBuffer class, which is responsible
 * for managing frame buffers used in rendering operations. It provides functionality for
 * creating, resizing, and reading pixel data from frame buffers.
*************************************************************************/

#include "hzpch.h"
#include "FrameBuffer.h"

namespace Night {
	/*!***********************************************************************
	\brief 
	Construct a FrameBuffer object with the provided specifications.

	\param specs 
	The specifications for the framebuffer.
	*************************************************************************/
	FrameBuffer::FrameBuffer(const FrameBufferSpecification& specs) : m_Specification(specs)
	{
		Invalidate();
	}

	/*!***********************************************************************
	\brief
	Destroy the FrameBuffer object.
	*************************************************************************/
	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	/*!***********************************************************************
	\brief
	Bind the FrameBuffer for rendering.
	*************************************************************************/
	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	/*!***********************************************************************
	\brief
	Unbind the FrameBuffer for rendering.
	*************************************************************************/
	void FrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	/*!***********************************************************************
	\brief
	(Re)Initialize the FrameBuffer.

	This function should be called when creating the FrameBuffer or when its dimensions change.
	*************************************************************************/
	void FrameBuffer::Invalidate()
	{
		if (m_RendererID) {
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_DepthAttachment);
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	/*!***********************************************************************
	\brief
	Resize the FrameBuffer.

	\param width 
	The new width for the frame buffer.

	\param height 
	The new height for the frame buffer.
	*************************************************************************/
	void FrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

	/*!***********************************************************************
	\brief
	Read the pixel data at a specific attachment index, x, and y.

	\param attachmentIndex 
	The attachment index for which to read the pixel data.

	\param x 
	The x-coordinate of the pixel to read.

	\param y 
	The y-coordinate of the pixel to read.
	
	\return 
	The integer value of the pixel at the specified coordinates and attachment index.
	*************************************************************************/
	int FrameBuffer::ReadPixel(uint32_t attachementIndex, int x, int y)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachementIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

		return pixelData;
	}
}