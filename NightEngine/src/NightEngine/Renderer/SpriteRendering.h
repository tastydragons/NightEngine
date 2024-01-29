#pragma once

#include <glm/glm.hpp>
#include "Texture.h"

namespace Night
{
	class SpriteRender
	{
	public:
		SpriteRender(const Ref<Texture>& texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture> GetTexture()const { return mTexture; }// retrive Texture	
		const glm::vec2* GetTexCoords()const { return mTexCoords; }	// retrieve texture coordinates

		//helper function to spilt the spritesheet 
		static Ref<SpriteRender> CreateSprite(const Ref<Texture>& texture, const glm::vec2& Coordinates, const glm::vec2& SpriteSize = { 512.0f, 512.0f });// our sprite sheet is standard 512 by 512


	private:
		Ref<Texture> mTexture;
		glm::vec2 mTexCoords[4];
	};
}