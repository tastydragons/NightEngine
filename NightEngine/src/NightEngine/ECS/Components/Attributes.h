#pragma once

#include "IComponent.h"

namespace Night
{
	class Attributes : public IComponent
	{
	public:

		enum class HazardTypes
		{
			HAZARD_EMPTY,
			HAZARD_GROUND,
			HAZARD_LASER
		};

		Attributes();
		~Attributes() = default;
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;

		/*!*************************************************************************
		Retrieves Component Entity ID
		****************************************************************************/
		Entity& GetComponentEntityID() { return entityID; }
		/*!*************************************************************************
		Sets Components Entity ID
		****************************************************************************/
		void SetComponentEntityID(Entity& entity) { entityID = entity; }

		float mFiringCoolDown;
		float mFireDurationTimer;
		float mChargeCoolDown;
		int mHazardDmg;
		HazardTypes mHazardType;
	};
}