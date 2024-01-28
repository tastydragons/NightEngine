#include "nepch.h"

#include "FrameBuffer.h"

#include <gl/glew.h>

namespace Night
{
	std::unique_ptr<FrameBuffer> FrameBuffer::mInstance{ nullptr };

	std::unique_ptr<FrameBuffer>& FrameBuffer::GetInstance()
	{
		if (mInstance == nullptr)
		{
			mInstance = std::make_unique<FrameBuffer>();
		}
		return mInstance;
	}

	/*!*************************************************************************
	return pixelData
	****************************************************************************/
	int FrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}
	namespace Helper {

		/*!*************************************************************************
		Switch between multi-sampling and single sampling
		****************************************************************************/
		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		/*!*************************************************************************
		Check DepthFormat
		****************************************************************************/
		static bool IsDepthFormat(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				return true;
			}
			return false;
		}

		/*!*************************************************************************
		Create texture
		****************************************************************************/
		static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(multisampled), count, outID);
		}

		/*!*************************************************************************
		Bind texture
		****************************************************************************/
		static void BindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled), id);
		}

		/*!*************************************************************************
		ColorAttachment
		ie internalFormat - GL_RGBA8 format - GL_RGBA
		ie internalFormat - GL_R32I format - GL_RED_INTEGER (reading int from the shader file)
		****************************************************************************/
		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, size_t index)
		{
			bool multisampled = samples > 1;
			if (multisampled) // check whether its used for multisampling (currently no)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (GLenum)index, (GLenum)TextureTarget(multisampled), id, 0);

		}

		/*!*************************************************************************
		DepthBufferAttachment :
		currently not using
		****************************************************************************/
		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}
	}

	/*!*************************************************************************
	Find buffer specs
	****************************************************************************/
	const FrameBufferSpecification Night::FrameBuffer::GetSpecification() const
	{
		return mSpecification;
	}

	/*!*************************************************************************
	Find color attachment
	****************************************************************************/
	uint32_t FrameBuffer::GetColorAttachmentRendererID(uint32_t index) const
	{
		//GM_CORE_ASSERT(index < mColorAttachments.size(), "");
		return  mColorAttachments[index];
	}

	/*!*************************************************************************
	Set frame specifications
	****************************************************************************/
	void FrameBuffer::SetFrameBufferSpecification(const FrameBufferSpecification& specification)
	{
		mSpecification = specification;

		for (auto spec : mSpecification.Attachments.Attachments)
		{
			if (!Helper::IsDepthFormat(spec.TextureFormat))
			{
				mColorAttachmentSpecification.emplace_back(spec);
			}
			else
				mDepthAttachmentSpecifiication = spec;
		}

		Create();
	}

	/*!*************************************************************************
	Resize frame specification
	****************************************************************************/
	void FrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		// update the specification
		mSpecification.Width = width;
		mSpecification.Height = height;
		Create();
	}

	/*!*************************************************************************
	Destructor
	****************************************************************************/
	FrameBuffer::~FrameBuffer()
	{
		// delete framebuffer object
		glDeleteFramebuffers(1, &mRendererID);
		// delete color attachment textures
		glDeleteTextures((GLsizei)mColorAttachments.size(), mColorAttachments.data());

		glDeleteRenderbuffers(1, &mDepthAttachment);
	}

	/*!*************************************************************************
	Create frame buffer
	****************************************************************************/
	void FrameBuffer::Create()
	{
		if (mRendererID)
		{
			// delete framebuffer object
			glDeleteFramebuffers(1, &mRendererID);
			// delete color attachment textures
			glDeleteTextures((GLsizei)mColorAttachments.size(), mColorAttachments.data());
			// delete renderbuffer objects
			glDeleteRenderbuffers(1, &mDepthAttachment);
			// clear attachments
			mColorAttachments.clear();
			mDepthAttachment = 0;
		}

		// creating a framebuffer obj
		glGenFramebuffers(1, &mRendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);

		bool multisample = mSpecification.Samples > 1; // check the specification if its multi-sampling 

		// Attachments
		if (mColorAttachmentSpecification.size())
		{
			// resize the ColorAttachment ID vector size to fit the ColorSpecification vector size (contains data properties of color attachment) 
			mColorAttachments.resize(mColorAttachmentSpecification.size());

			// generates color attachments 
			Helper::CreateTextures(multisample, mColorAttachments.data(), (uint32_t)mColorAttachments.size());
			for (size_t i{ 0 }; i < mColorAttachments.size(); ++i)
			{
				Helper::BindTexture(multisample, mColorAttachments[i]);

				// switch between rendering RGBA8 for the game and RED_INTGER for shader 
				switch (mColorAttachmentSpecification[i].TextureFormat)
				{
					// Current Game (RGBA8)
				case FrameBufferTextureFormat::RGBA8:
					Helper::AttachColorTexture(mColorAttachments[i], mSpecification.Samples, GL_RGBA8, GL_RGBA, mSpecification.Width, mSpecification.Height, i);
					break;
					// Used to sample an int value from the fragment shader 
				case FrameBufferTextureFormat::RED_INTEGER:
					Helper::AttachColorTexture(mColorAttachments[i], mSpecification.Samples, GL_R32I, GL_RED_INTEGER, mSpecification.Width, mSpecification.Height, i);
					break;
				}
			}
		}


		// generates Depth stencil attachment (not using)
		if (mDepthAttachmentSpecifiication.TextureFormat != FrameBufferTextureFormat::NONE)
		{
			// create depth attachment 
			Helper::CreateTextures(multisample, &mDepthAttachment, 1);
			Helper::BindTexture(multisample, mDepthAttachment);
			switch (mDepthAttachmentSpecifiication.TextureFormat)
			{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				Helper::AttachDepthTexture(mDepthAttachment, mSpecification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, mSpecification.Width, mSpecification.Height);
			}
		}

		// draw (max of 4 color attachments)
		if (mColorAttachments.size() > 1)
		{
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((GLsizei)mColorAttachments.size(), buffers);
		}
		else if (mColorAttachments.empty())
		{
			// depth pass
			glDrawBuffer(GL_NONE);
		}

		// check framebuffer status
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		NIGHT_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "FrameBuffer is incomplete!");

		// render onto the main window (activate default framebuffer)	
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	/*!*************************************************************************
	Bind frambuffer
	****************************************************************************/
	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
		glViewport(0, 0, mSpecification.Width, mSpecification.Height);
	}

	/*!*************************************************************************
	Unbind frambuffer
	****************************************************************************/
	void FrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}