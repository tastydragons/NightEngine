#include "nepch.h"

#include "Texture.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <stb_image.h>

namespace Night
{
	/*!*************************************************************************
	Constructor for texture
	****************************************************************************/
	Texture::Texture()
		:mWidth{ 0 }, mHeight{ 0 }, mWrapS{ GL_LINEAR }, mWrapT{ GL_LINEAR }, mFilterMin{ GL_CLAMP_TO_BORDER }, mFilterMax{ GL_CLAMP_TO_BORDER }, mInnerFormat{ GL_RGB8 }, mImageFormat{ GL_RGBA }
	{
	}

	/*!*************************************************************************
	Destructor for texture
	****************************************************************************/
	Texture::~Texture()
	{
		glDeleteTextures(1, &mRendererID);
	}

	/*!*************************************************************************
	Bind texture
	****************************************************************************/
	void Texture::Bind(unsigned int slot) const
	{
		glBindTextureUnit(slot, mRendererID);
	}

	/*!*************************************************************************
	Unbind texture
	****************************************************************************/
	void Texture::UnBind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	/*!*************************************************************************
	Generate texture from file
	****************************************************************************/
	void Texture::GenTexture(const std::string& path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		unsigned char* data = nullptr;
		{
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		if (data)
		{
			mWidth = width;
			mHeight = height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			mInnerFormat = internalFormat;
			mImageFormat = dataFormat;

			glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
			glTextureStorage2D(mRendererID, 1, mInnerFormat, mWidth, mHeight);

			glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, mImageFormat, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
	}

	/*!*************************************************************************
	Generate texture
	****************************************************************************/
	void Texture::GenTexture(unsigned int width, unsigned int height)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
		glTextureStorage2D(mRendererID, 1, mInnerFormat, width, height);

		glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	/*!*************************************************************************
	Set texture data
	****************************************************************************/
	void Texture::SetTextureData(void* data, unsigned int size)
	{
		uint32_t bpp = mImageFormat == GL_RGBA ? 4 : 3;
		NIGHT_CORE_ASSERT(size == mWidth * mHeight * bpp, "Data must be entire texture!");
		glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, mImageFormat, GL_UNSIGNED_BYTE, data);
	}
}