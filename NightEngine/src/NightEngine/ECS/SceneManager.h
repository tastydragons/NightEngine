#pragma once

#include "ECS.h"

#include "Types.h"
#include "ComponentManager.h"
#include "EntityManager.h"

#include "Platform/System/SystemManager.h"

#include "Serialization/JSONSerialization.h"

#define p_Scene Night::SceneManager::GetInstance()

namespace Night
{
	class SceneManager : public JSONSerializer
	{
	public:
		/*!*************************************************************************
		Returns an instance of the SceneManager
		****************************************************************************/
		static std::unique_ptr<SceneManager>& GetInstance();

		/*!*************************************************************************
		Initialize all registered components
		****************************************************************************/
		void Init();
		/*!*************************************************************************
		Deserializes ECS entities and data from JSON file to build ECS (File loading)
		****************************************************************************/
		virtual bool Deserialize(const rapidjson::Value& obj);
		/*!*************************************************************************
		Serializes ECS entities and data to JSON file to save ECS (File saving)
		****************************************************************************/
		virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;

		// temp: scene to load code
		void setSceneToLoad(std::string s);
		void checkForSceneToLoad();
		void changeScene(std::string s);

		bool isGameplay;
	private:
		// Unique pointer to SceneManager
		static std::unique_ptr<SceneManager> m_instance;

		std::string sceneToLoad;
	};
}