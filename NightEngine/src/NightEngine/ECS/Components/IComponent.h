#pragma once

#include "Serialization/JSONSerialization.h"

#include "NightEngine/Math/Vector2D.h"
#include "NightEngine/ECS/Types.h"

namespace Night
{
	//When Buiilding a New Component
	//Ensure that the SceneManager Init has the component registered
	//Ensure that the Serialize and De-Serialize functions in the SceneManager has those components checked
	/*!*************************************************************************
	Interface Class for IComponent
	****************************************************************************/
	class IComponent : public JSONSerializer
	{
	public:
		virtual ~IComponent() = default;
		virtual void ToEditor() {}
		virtual bool Deserialize(const rapidjson::Value& obj) = 0;
		virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const = 0;

		/*!*************************************************************************
		Retrieves Component Entity ID
		****************************************************************************/
		virtual Entity& GetComponentEntityID() = 0;
		/*!*************************************************************************
		Sets Components Entity ID
		****************************************************************************/
		virtual void SetComponentEntityID(Entity& entity) = 0;

	protected:
		Entity entityID{};
	};
}