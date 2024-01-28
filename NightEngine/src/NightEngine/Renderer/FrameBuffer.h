#pragma once

#define p_FrameBuffer Night::FrameBuffer::GetInstance()

namespace Night
{
	// user defined format of Opengl framebuffer format types 
	enum class FrameBufferTextureFormat {
		NONE = 0,
		RGBA8,					// Color
		RED_INTEGER,
		DEPTH24STENCIL8,		// Depth stencil
		DEPTH = DEPTH24STENCIL8 // default
	};

	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format) : TextureFormat(format) {}
		FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::NONE;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list< FrameBufferTextureSpecification> attachments) : Attachments(attachments) {}
		std::vector<FrameBufferTextureSpecification> Attachments;
	};

	struct FrameBufferSpecification
	{
		uint32_t Width{ 0 }, Height{ 0 };
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;
		bool SwapChainTarget = false;
	};
	class FrameBuffer
	{
	public:
		FrameBuffer() = default;
		~FrameBuffer();


		uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const;
		void SetFrameBufferSpecification(const FrameBufferSpecification&);
		const FrameBufferSpecification GetSpecification() const;

		void Resize(uint32_t width, uint32_t height);

		int ReadPixel(uint32_t attachmentIndex, int x, int y);
		void Create();
		void Bind();
		void UnBind();
		static std::unique_ptr<FrameBuffer>& GetInstance();
	private:
		// framebuffer
		uint32_t mRendererID = 0;
		FrameBufferSpecification mSpecification;
		// framebuffer instance 
		static std::unique_ptr<FrameBuffer> mInstance;

		// contains all the ColorAttachmentSpecification (user has defined)
		std::vector<FrameBufferTextureSpecification> mColorAttachmentSpecification;
		// depth attachment not using currently 
		FrameBufferTextureSpecification mDepthAttachmentSpecifiication = FrameBufferTextureFormat::NONE;

		// contains all the ColorAttachments renderID 
		std::vector<uint32_t> mColorAttachments;
		uint32_t mDepthAttachment = 0;
	};
}