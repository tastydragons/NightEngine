/*!***********************************************************************
 * @file Texture.h
 * @author Aloysius Liong Yu Xuan
 * @par DP email: a.liong@digipen.edu
 * @par Course: CSD2401F23-B
 * @par Milestone 1
 * @date 27-9-2023
 * @brief Declaration of the Texture class for handling texture data.
 *
 * This header file defines the Texture class, which is responsible for
 * managing texture data in OpenGL. It provides constructors for loading
 * textures from image files or creating empty textures, as well as functions
 * for setting texture data and binding textures for rendering.
*************************************************************************/

#pragma once

#include <string>
#include <GL/glew.h>

namespace Night {
	class Texture
	{
	public:
		Texture(const std::string& filepath);
		Texture(uint32_t width, uint32_t height);
		~Texture();

		void TextureData(void* data);

		virtual bool operator== (const Texture& other) const {
			return m_rendererID == other.GetRendererID();
		}

		inline uint32_t GetWidth() const { return m_width; }
		inline uint32_t GetHeight() const { return m_height; }
		inline uint32_t GetRendererID() const { return m_rendererID; }

		void Bind(uint32_t slot = 0) const;

	private:
		uint32_t m_width = 0, m_height = 0;
		uint32_t m_rendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}

