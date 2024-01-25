/*!***********************************************************************
 * @file Texture.cpp
 * @author Aloysius Liong Yu Xuan
 * @par DP email: a.liong@digipen.edu
 * @par Course: CSD2401F23-B
 * @par Milestone 1
 * @date 27-9-2023
 * @brief Implementation of the Texture class for loading and managing textures in OpenGL.
 *
 * This file contains the implementation of the Texture class, which is used
 * for loading textures from image files and managing them in OpenGL. It provides
 * constructors for creating textures from image files or generating empty textures,
 * setting texture data, and binding textures to specific texture units.
*************************************************************************/

#include "Texture.h"
#include "../Precompile/Precompiled.h"
#include <stb_image.h>
#include <iostream>

namespace Night {
	/*!***********************************************************************
	\brief 
	Construct a texture from an image file.

	This constructor loads an image file from the specified filepath and creates
	a texture from it. It sets the texture's properties such as width, height,
	internal format, data format, and storage.

	\param[in] filepath 
	The file path to the image.
	*************************************************************************/
	Texture::Texture(const std::string& filepath)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

		m_width = width;
		m_height = height;

		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}

		else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		else {
			//std::cout << "Format not supported!";
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, internalFormat, m_width, m_height);

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	/*!***********************************************************************
	\brief 
	Construct an empty texture with a specified width and height.

	This constructor creates an empty texture with the given width and height.
	It sets the texture's internal format, data format, storage, and filtering
	parameters.

	\param[in] width  
	The width of the empty texture.

	\param[in] height 
	The height of the empty texture.
	*************************************************************************/
	Texture::Texture(uint32_t width, uint32_t height)
	{
		(void)height;
		(void)width;
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, m_InternalFormat, m_width, m_height);

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	/*!***********************************************************************
	\brief Destructor for the Texture class.

	This destructor releases the OpenGL texture resource associated with
	the Texture object when it is no longer needed.
	*************************************************************************/
	Texture::~Texture()
	{
		glDeleteTextures(1, &m_rendererID);
	}

	/*!***********************************************************************
	\brief 
	Set texture data.

	This function updates the texture's data with the provided pixel data.
	The data should match the dimensions and format of the texture.

	\param[in] data 
	A pointer to the pixel data.
	*************************************************************************/
	void Texture::TextureData(void* data)
	{
		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	/*!***********************************************************************
	\brief 
	Bind the texture to a texture unit slot.

	This function binds the texture to a specified texture unit slot, allowing
	it to be used in shaders.

	\param[in] slot 
	The texture unit slot to which the texture will be bound.
	*************************************************************************/
	void Texture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_rendererID);
	}
}