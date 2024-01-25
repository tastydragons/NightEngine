/*!***********************************************************************
 * @file			Sprite.h
 * @author			Tan Wei Jie Clement
 * @par DP email:	weijieclement.tan@digipen.edu
 * @par Course:		CSD2401F23-B
 * @par				Milestone 1
 * @date			28-9-2023
 * @brief
 *
 * This file provides the declaration of class Sprite
*************************************************************************/

#pragma once

#include "../Composition/Composition.h"
#include "../Others/ObjectLinkedList.h"
#include "../Math/Vector2D.h"
#include "../Math/Vector4D.h"
#include "Texture.h"

namespace Night
{
	class Sprite : public GameComponent
	{
	public:
		Sprite* Next;
		Sprite* Prev;

		Sprite();
		~Sprite();

		virtual void Serialize(NightSerializer& stream);
		virtual void Initialize();

		void SerializeToJson(GOC& Object, const std::string& filename_);
		void JsonDeserialize(const nlohmann::json& json);

		void Draw();
		void noDraw();
		void ChangeTexture(std::string new_pTex);

		Vec2 pos;
		Vec2 size;
		Vec4 color;

		float rotation;
		std::string pTex;

		Ref<Texture> texture;

		bool isSelected = false;

	};
}
