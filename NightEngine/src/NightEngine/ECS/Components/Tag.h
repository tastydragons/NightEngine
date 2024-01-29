#pragma once

#include "IComponent.h"

namespace Night
{
	class Tag : public IComponent
	{
	public:
		Tag();
		~Tag() = default;

		void SetTag(std::string tag) { mTag = tag; }
		std::string& GetTag() { return mTag; }

		void SetTargetTag(std::string tag) { mTargetTag = tag; }
		std::string& GetTargetTag() { return mTargetTag; }

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

	private:
		std::string mTag;
		std::string mTargetTag;

	};
}