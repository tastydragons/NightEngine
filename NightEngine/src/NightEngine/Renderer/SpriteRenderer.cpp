#include "SpriteRenderer.h"

namespace Night {
	Night::SpriteRenderer::SpriteRenderer(const Ref<Texture>& texture, const glm::vec2& min, const glm::vec2& max) : m_Tex(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Ref<SpriteRenderer> SpriteRenderer::CreateSprite(const Ref<Texture>& texture, const glm::vec2& coords, const glm::vec2& spriteSize)
	{
		glm::vec2 min = { (coords.x * spriteSize.x) / texture->GetWidth(), (coords.y * spriteSize.y) / texture->GetHeight() };
		glm::vec2 max = { ((coords.x + 1) * spriteSize.x) / texture->GetWidth(), ((coords.y + 1) * spriteSize.y) / texture->GetHeight() };

		return CreateRef<SpriteRenderer>(texture, min, max);
	}


}