/*!***********************************************************************
 * @file Renderer.cpp
 * @author Aloysius Liong Yu Xuan
 * @par DP email: a.liong@digipen.edu
 * @par Course: CSD2401F23-B
 * @par Milestone 2
 * @date 2-11-2023
 * @brief his file contains the implementation of the FrameBuffer class, which represents a graphics framebuffer for rendering operations.
 *
 * A FrameBuffer is a critical component in computer graphics used to render scenes and images off-screen before displaying them on the screen. 
 * It consists of a color attachment and depth attachment that stores rendered content. 
 * The FrameBuffer class allows creating, binding, resizing, and reading pixels from framebuffers.
 *
 * The class is used in conjunction with the rendering system to provide a flexible and efficient way to control rendering processes, manage framebuffers, 
 * and generate high-quality images for various graphics applications.
*************************************************************************/

#pragma once

#include "../Core/Core.h"
#include "../Renderer/FrameBuffer.h"

namespace Night {

	struct FrameBufferSpecification
	{
		uint32_t Width{ 0 }, Height{ 0 };
		uint32_t Samples = 1;
		bool SwapChainTarget = false;
	};

	class FrameBuffer {
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment, m_DepthAttachment = 0;
		FrameBufferSpecification m_Specification;

	public:
		FrameBuffer() = default;
		FrameBuffer(const FrameBufferSpecification& specs);
		~FrameBuffer();
		void Bind();
		void UnBind();

		void Invalidate();

		void Resize(uint32_t width, uint32_t height);
		int ReadPixel(uint32_t attachementIndex, int x, int y);

		uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }
		const FrameBufferSpecification& GetSpecification() const { return m_Specification; }

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec) { return CreateRef<FrameBuffer>(spec); }
	};

	extern FrameBufferSpecification* Spec;
}

