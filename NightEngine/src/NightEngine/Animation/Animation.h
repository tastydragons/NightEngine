#pragma once

#include "NightEngine/Renderer/Renderer.h"
#include "NightEngine/Renderer/SpriteRendering.h"

#include "NightEngine/Math/Vector2D.h"
#include <NightEngine/ECS/Components/Sprite.h>

namespace Night
{
	class Animation
	{
	public:
		static std::unordered_map<std::string, std::vector<float>> spriteContainer;// to store all animation info
		Animation();
		void UpdateAnimation(float deltatime, Sprite& sprite);
	private:
	};
}