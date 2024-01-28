#pragma once

#include <glm/glm.hpp>

#include "Texture.h"
#include "Shader.h"
#include "Camera2D.h"

namespace Night
{
	// Holds all state information relevant to a character as loaded using FreeType
	struct Character
	{
		unsigned int TextureID; //ID handler of glyph texture
		glm::ivec2 Size;		//size of glyph
		glm::ivec2 Bearing;		//offset from baseline to left/top of glyph
		unsigned int Advance;	//horizontal offset to next glyph
	};
	// A renderer class for rendering text displayed by a font loaded using the 
	// FreeType library. A single font is loaded, processed into a list of Character
	// items for later rendering.
	class Font
	{
	public:
		std::map<char, Character> Characters;

		Font() = default;
		void Init(); // load the path of the font we taking

		void Load(const std::string& fontpath, unsigned int fontsize = 50);

		void RenderText(const std::string& text, glm::vec2 position, float scale, Camera2D& camera, glm::vec3 color = glm::vec3(1.0f));

	private:
		Ref<Shader> TextShader;
		unsigned int mVao{ 0 }, mVbo{ 0 };

	};
}