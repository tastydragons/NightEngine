#pragma once

#include "IComponent.h"

#include "NightEngine/Scripts/IScript.h"

namespace Night
{
	class IScript;
	class Logic : public IComponent
	{
	public:
		Logic();
		Logic(const Logic& rhs);
		Logic& operator= (const Logic& rhs);
		~Logic() = default;

		std::vector<IScript*>& GetScript() { return mScriptsVector; }
		void SetScriptEntity(Entity entity);
		void InsertScript(IScript* script, Entity entity);
		void DeleteScript(std::string scriptname);
		void ClearAllScripts();
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;
		std::vector<std::string> GetScriptNames() { return mScriptNameVector; }

		IScript* GetScriptByName(std::string ScriptName);

		/*!*************************************************************************
		Retrieves Component Entity ID
		****************************************************************************/
		Entity& GetComponentEntityID() { return entityID; }
		/*!*************************************************************************
		Sets Components Entity ID
		****************************************************************************/
		void SetComponentEntityID(Entity& entity) { entityID = entity; }

	private:
		std::vector<std::string> mScriptNameVector;
		std::vector<IScript*> mScriptsVector;

		// Go into IScript.h, use std::string GetScriptName() to find script name
		// Compare script name to mScriptNameVector to find script in vector
		// Use that vector pos to find script in mScriptVector
		// This accesses the object, through the object we can access the member functions.
		// Access state machine through this.
	};
}