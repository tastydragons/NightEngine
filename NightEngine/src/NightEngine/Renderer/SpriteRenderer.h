#pragma once

#include <glm.hpp>
#include "../Engine/Core.h"
#include "Texture.h"

namespace Night {
	class SpriteRenderer
	{
	public:
		SpriteRenderer(const Ref<Texture>& texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture> GetTexture()const { return m_Tex; }
		const glm::vec2* GetTexCoords()const { return m_TexCoords; }

		static Ref<SpriteRenderer> CreateSprite(const Ref<Texture>& texture, const glm::vec2& coords, const glm::vec2& spriteSize);
	private:
		Ref<Texture> m_Tex;
		glm::vec2 m_TexCoords[4];
	};
}