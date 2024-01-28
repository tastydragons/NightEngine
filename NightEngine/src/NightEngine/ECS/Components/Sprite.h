#pragma once

#include "IComponent.h"
#include "glm/glm.hpp"

namespace Night
{
	class Sprite : public IComponent
	{
	public:
		struct Coordinates
		{
			int x; int y;

		}Index;

		Sprite();
		~Sprite() = default;
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;

		void SetTexture(const std::string& name) { mTextureName = name; }
		std::string& GetTexture() { return mTextureName; }

		void SetIndex(int& x, int& y) { Index.x = x, Index.y = y; } // y will always remind as zero
		Coordinates& GetIndex() { return Index; }

		std::vector<float>& GetDisplayTime() { return displayTime; }
		int& GetMaxIndex() { return MaxIndex_X; }
		void SetUVCoor(float& u, float& v) { mUVcooridnates.x = u; mUVcooridnates.y = v; }
		vec2D& GetUVCoor() { return mUVcooridnates; }

		void SetAlpha(float in) { mAlpha = in; }
		float GetAlpha() { return mAlpha; }

		void SetLayerOrder(int layer) { LayerOrder = layer; }
		/*!*************************************************************************
		Retrieves Component Entity ID
		****************************************************************************/
		Entity& GetComponentEntityID() { return entityID; }
		/*!*************************************************************************
		Sets Components Entity ID
		****************************************************************************/
		void SetComponentEntityID(Entity& entity) { entityID = entity; }

	public:

		bool is_SpriteSheet; // to tell whether we are using a spritesheet
		bool is_Animated;	// if is a spritesheet do we want it to be animated 
		float internaltimer;
		int LayerOrder;
		//altas 
	private:
		std::string mTextureName;
		vec2D mUVcooridnates;
		float mAlpha;
		int MaxIndex_X;
		std::vector<float>displayTime;
		//TODO insert a increment for mIndex so that it can lease with the mAimator
	};
}