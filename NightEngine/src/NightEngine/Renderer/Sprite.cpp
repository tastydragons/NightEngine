/*!***********************************************************************
 * @file			Sprite.cpp
 * @author			Tan Wei Jie Clement
 * @par DP email:	weijieclement.tan@digipen.edu
 * @par Course:		CSD2401F23-B
 * @par				Milestone 1
 * @date			28-9-2023
 * @brief
 *
 * This file provides the definition for member functions in 
 * class Sprite
*************************************************************************/

#include "../Precompile/Precompiled.h"
#include "Sprite.h"
#include "../Graphics/Renderer.h"
#include "../Game/GameLogic.h"

namespace Night
{
	Sprite::Sprite() : Next(nullptr), Prev(nullptr)
	{
		pos = { 0.0f, 0.0f };
		size = { 0.0f, 0.0f };
		color = { 0.0f, 0.0f, 0.0f, 1.0f };
		rotation = 0.0f;
		texture = NULL;
	}

	Sprite::~Sprite()
	{
		LOGIC->getSpriteList().erase(this);
	}

	void Sprite::Initialize()
	{
		texture = CreateRef<Texture>("Assets/" + pTex);
		LOGIC->getSpriteList().push_back(this);
	}

	void Sprite::Serialize(NightSerializer& stream)
	{
		StreamRead(stream, pos);
		StreamRead(stream, size);
		StreamRead(stream, rotation);
		StreamRead(stream, color);
		StreamRead(stream, pTex);
	}

	void SerializeToJson(GOC& Object, const std::string& filename_)
	{
		nlohmann::json jsonObject;

		// Set FileType
		jsonObject["FileType"] = "Archetype";

		// Set Components
		nlohmann::json componentsJson;

		// Add Sprite component
		nlohmann::json spriteJson;
		spriteJson["position"]["x"] = Object.has(Sprite)->pos.x;
		spriteJson["position"]["y"] = Object.has(Sprite)->pos.y;
		spriteJson["size"]["x"] = Object.has(Sprite)->size.x;
		spriteJson["size"]["y"] = Object.has(Sprite)->size.y;
		spriteJson["rotation"] = Object.has(Sprite)->rotation;
		spriteJson["color"]["r"] = Object.has(Sprite)->color.w;
		spriteJson["color"]["g"] = Object.has(Sprite)->color.x;
		spriteJson["color"]["b"] = Object.has(Sprite)->color.y;
		spriteJson["color"]["a"] = Object.has(Sprite)->color.z;
		spriteJson["texturePath"] = Object.has(Sprite)->pTex;

		componentsJson["Sprite"] = spriteJson;

		//// Add Transform component
		//json transformJson;
		//transformJson["position"]["x"] = transformComponent.position.x;
		//transformJson["position"]["y"] = transformComponent.position.y;
		//transformJson["size"]["x"] = transformComponent.size.x;
		//transformJson["size"]["y"] = transformComponent.size.y;
		//transformJson["rotation"] = transformComponent.rotation;

		//componentsJson["Transform"] = transformJson;

		// Add other components as needed...

		// Set Components in the main object
		jsonObject["Components"] = componentsJson;

		//return jsonObject;
	}

	void Sprite::JsonDeserialize(const nlohmann::json& json)
	{
		if (json.contains("position") && json["position"].is_object())
		{
			pos.x = json["position"]["x"].get<float>();
			pos.y = json["position"]["y"].get<float>();
		}

		if (json.contains("size") && json["size"].is_object())
		{
			size.x = json["size"]["x"].get<float>();
			size.y = json["size"]["y"].get<float>();
		}

		rotation = json.value("rotation", 0.0f);

		if (json.contains("color") && json["color"].is_object())
		{
			color.x = json["color"]["r"].get<float>();
			color.y = json["color"]["g"].get<float>();
			color.z = json["color"]["b"].get<float>();
			color.w = json["color"]["a"].get<float>();
		}

		pTex = json.value("texturePath", "");	
	}

	void Sprite::Draw()
	{
		glm::vec2 Position{ pos.x, pos.y };
		glm::vec2 Size{ size.x, size.y };
		glm::vec4 Color{ color.x, color.y, color.z, color.w };
		float Rotation = rotation;

		Renderer::DrawQuad(Position, Size, Rotation, texture);
	}

	void Sprite::noDraw()
	{
		glm::vec2 Position{ pos.x, pos.y };
		glm::vec2 Size{ size.x * 0.0f, size.y * 0.0f };
		glm::vec4 Color{ color.x, color.y, color.z, color.w };
		float Rotation = rotation;

		Renderer::DrawQuad(Position, Size, Rotation, texture);
	}

	// NOTE: GIVE THIS FUNCTION PATHS WITHOUT STARTING WITH "Assets/"
	void Sprite::ChangeTexture(std::string new_pTex)
	{
		this->pTex = new_pTex;
		Ref<Texture> new_Texture = CreateRef<Texture>(this->pTex);

		this->texture = new_Texture;
	}
}