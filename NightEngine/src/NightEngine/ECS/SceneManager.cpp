#include "hzpch.h"

#include "SceneManager.h"

#include "NightEngine/Renderer/Graphics.h"

#include "NightEngine/Audio/AudioEngine.h"

namespace Night
{
	// Unique pointer to instance of Scene
	std::unique_ptr<SceneManager> SceneManager::m_instance;

	/*!*************************************************************************
	Returns an instance of the SceneManager
	****************************************************************************/
	std::unique_ptr<SceneManager>& SceneManager::GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = std::make_unique<SceneManager>();
		}
		return m_instance;
	}

	/*!*************************************************************************
	Initialize all registered components
	****************************************************************************/
	void SceneManager::Init()
	{
		p_ecs.RegisterComponent<Transform>();
		p_ecs.RegisterComponent<RigidBody>();
		p_ecs.RegisterComponent<Collider>();
		p_ecs.RegisterComponent<NameTag>();
		p_ecs.RegisterComponent<Sprite>();
		p_ecs.RegisterComponent<Logic>();
		p_ecs.RegisterComponent<Tag>();
		p_ecs.RegisterComponent<Audio>();
		p_ecs.RegisterComponent<HUDComponent>();
		p_ecs.RegisterComponent<Attributes>();
		p_ecs.RegisterComponent<PlayerAttributes>();
		p_ecs.RegisterComponent<EnemyAttributes>();
		sceneToLoad = "";
	}

	//NEW DESERIALIZE
	bool SceneManager::Deserialize(const rapidjson::Value& obj)
	{
		p_ecs.ResetAllEntitySignatures();
		p_ecs.ResetEntities();
		p_ecs.SetTotalEntitiesForWorldBuild(obj["Number of Entities"]["Entities"].GetUint());
		auto aliveTotal = obj["Number of Entities"]["Entities"].GetUint();
		Entity j = 0;
		while (j < aliveTotal)
		{
			Signature signature(obj["EntitySignatures"][j].GetString());
			p_ecs.SetEntitySignature(j, signature);
			for (ComponentType CompID = 0; CompID < p_ecs.GetTotalRegisteredComponents(); ++CompID)
			{
				if (p_ecs.GetEntitySignature(j).test(CompID))
				{
					if (p_ecs.GetComponentTypeName(CompID) == "Transform")
					{
						Transform transform;
						transform.SetComponentEntityID(j);
						if (transform.Deserialize(obj["Components"][std::to_string(j).c_str()][static_cast<int>(CompID)].GetObj()))
						{
							p_ecs.AddComponent<Transform>(j, transform);
						}
					}
					if (p_ecs.GetComponentTypeName(CompID) == "RigidBody")
					{
						RigidBody rigidbody;
						rigidbody.SetComponentEntityID(j);
						if (rigidbody.Deserialize(obj["Components"][std::to_string(j).c_str()][static_cast<int>(CompID)].GetObj()))
						{
							p_ecs.AddComponent<RigidBody>(j, rigidbody);
						}
					}
					if (p_ecs.GetComponentTypeName(CompID) == "Collider")
					{
						Collider collider;
						collider.SetComponentEntityID(j);
						if (collider.Deserialize(obj["Components"][std::to_string(j).c_str()][static_cast<int>(CompID)].GetObj()))
						{
							p_ecs.AddComponent<Collider>(j, collider);
						}
					}
					if (p_ecs.GetComponentTypeName(CompID) == "NameTag")
					{
						NameTag nametag;
						nametag.SetComponentEntityID(j);
						if (nametag.Deserialize(obj["Components"][std::to_string(j).c_str()][static_cast<int>(CompID)].GetObj()))
						{
							p_ecs.AddComponent<NameTag>(j, nametag);
						}
					}
					if (p_ecs.GetComponentTypeName(CompID) == "Sprite")
					{
						Sprite sprite;
						sprite.SetComponentEntityID(j);
						if (sprite.Deserialize(obj["Components"][std::to_string(j).c_str()][static_cast<int>(CompID)].GetObj()))
						{
							p_ecs.AddComponent<Sprite>(j, sprite);
						}
					}
					if (p_ecs.GetComponentTypeName(CompID) == "Logic")
					{
						Logic logic;
						logic.SetComponentEntityID(j);
						if (logic.Deserialize(obj["Components"][std::to_string(j).c_str()][static_cast<int>(CompID)].GetObj()))
						{
							p_ecs.AddComponent<Logic>(j, logic);
						}
					}
					if (p_ecs.GetComponentTypeName(CompID) == "Tag")
					{
						Tag tag;
						tag.SetComponentEntityID(j);
						if (tag.Deserialize(obj["Components"][std::to_string(j).c_str()][static_cast<int>(CompID)].GetObj()))
						{
							p_ecs.AddComponent<Tag>(j, tag);
						}
					}
					if (p_ecs.GetComponentTypeName(CompID) == "Audio")
					{
						Audio mAudio;
						mAudio.SetComponentEntityID(j);
						if (mAudio.Deserialize(obj["Components"][std::to_string(j).c_str()][static_cast<int>(CompID)].GetObj()))
						{
							p_ecs.AddComponent<Audio>(j, mAudio);
						}
					}
					if (p_ecs.GetComponentTypeName(CompID) == "HUDComponent")
					{
						HUDComponent mHUD;
						mHUD.SetComponentEntityID(j);
						if (mHUD.Deserialize(obj["Components"][std::to_string(j).c_str()][static_cast<int>(CompID)].GetObj()))
						{
							p_ecs.AddComponent<HUDComponent>(j, mHUD);
						}
					}
					if (p_ecs.GetComponentTypeName(CompID) == "Attributes")
					{
						Attributes mAttributes;
						mAttributes.SetComponentEntityID(j);
						if (mAttributes.Deserialize(obj["Components"][std::to_string(j).c_str()][static_cast<int>(CompID)].GetObj()))
						{
							p_ecs.AddComponent<Attributes>(j, mAttributes);
						}
					}
					if (p_ecs.GetComponentTypeName(CompID) == "PlayerAttributes")
					{
						PlayerAttributes mPlayerAttributes;
						mPlayerAttributes.SetComponentEntityID(j);
						if (mPlayerAttributes.Deserialize(obj["Components"][std::to_string(j).c_str()][static_cast<int>(CompID)].GetObj()))
						{
							p_ecs.AddComponent<PlayerAttributes>(j, mPlayerAttributes);
						}
					}
					if (p_ecs.GetComponentTypeName(CompID) == "EnemyAttributes")
					{
						EnemyAttributes mEnemyAttributes;
						mEnemyAttributes.SetComponentEntityID(j);
						if (mEnemyAttributes.Deserialize(obj["Components"][std::to_string(j).c_str()][static_cast<int>(CompID)].GetObj()))
						{
							p_ecs.AddComponent<EnemyAttributes>(j, mEnemyAttributes);
						}
					}
				}
			}
			++j;
		}
		return true;
	}


	//Due to restrictions of not using RTTR, components type cannot be deduced at runtime and hence needs to be checked manually
	//Will work on improving serialization further
	/*!*************************************************************************
	Serializes ECS entities and data to JSON file to save ECS (File saving)
	****************************************************************************/
	bool SceneManager::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
	{
		writer->StartObject();
		writer->Key("Number of Entities");
		writer->StartObject();
		writer->Key("Entities");
		writer->Uint(p_ecs.GetTotalEntities());
		writer->EndObject();

		/*writer->Key("EntityToIndexMap");
		writer->StartObject();
		for (ComponentType i = 0; i < p_ecs.GetTotalRegisteredComponents(); ++i)
		{
			writer->Key(p_ecs.GetComponentTypeName(i).c_str());
			writer->StartArray();
			for (auto j = p_ecs.GetEntityToIndexMapECS(i).begin(); j != p_ecs.GetEntityToIndexMapECS(i).end(); ++j)
			{
				writer->Uint64(*j);
			}
			writer->EndArray();
		}
		writer->EndObject();*/

		writer->Key("EntitySignatures");
		writer->StartArray();
		auto aliveTotal = p_ecs.GetTotalEntities();
		Entity aliveCount = 0;
		Entity iterEntity = 0;
		while (aliveCount < aliveTotal)
		{
			writer->String(p_ecs.GetEntitySignature(iterEntity).to_string().c_str());
			if (p_ecs.GetEntitySignature(iterEntity).any())
			{
				aliveCount++;
			}
			iterEntity++;
		}
		writer->EndArray();

		writer->Key("Components");
		writer->StartObject();
		aliveTotal = p_ecs.GetTotalEntities();
		Entity j = 0;
		while (j < aliveTotal)
		{
			writer->Key(std::to_string(j).c_str());
			writer->StartArray();
			for (ComponentType compID = 0; compID < p_ecs.GetTotalRegisteredComponents(); ++compID)
			{
				writer->StartObject();
				if (p_ecs.GetEntitySignature(j).any())
				{
					if (p_ecs.GetEntitySignature(j).test(compID))
					{
						//ADD COMPONENTS HERE FOR SERIALIZE
						if (p_ecs.GetComponentTypeName(compID) == "Transform")
						{
							p_ecs.GetComponent<Transform>(j).Serialize(writer);
						}
						if (p_ecs.GetComponentTypeName(compID) == "RigidBody")
						{
							p_ecs.GetComponent<RigidBody>(j).Serialize(writer);
						}
						if (p_ecs.GetComponentTypeName(compID) == "Collider")
						{
							p_ecs.GetComponent<Collider>(j).Serialize(writer);
						}
						if (p_ecs.GetComponentTypeName(compID) == "NameTag")
						{
							p_ecs.GetComponent<NameTag>(j).Serialize(writer);
						}
						if (p_ecs.GetComponentTypeName(compID) == "Sprite")
						{
							p_ecs.GetComponent<Sprite>(j).Serialize(writer);
						}
						if (p_ecs.GetComponentTypeName(compID) == "Logic")
						{
							p_ecs.GetComponent<Logic>(j).Serialize(writer);
						}
						if (p_ecs.GetComponentTypeName(compID) == "Tag")
						{
							p_ecs.GetComponent<Tag>(j).Serialize(writer);
						}
						if (p_ecs.GetComponentTypeName(compID) == "Audio")
						{
							p_ecs.GetComponent<Audio>(j).Serialize(writer);
						}
						if (p_ecs.GetComponentTypeName(compID) == "HUDComponent")
						{
							p_ecs.GetComponent<HUDComponent>(j).Serialize(writer);
						}
						if (p_ecs.GetComponentTypeName(compID) == "Attributes")
						{
							p_ecs.GetComponent<Attributes>(j).Serialize(writer);
						}
						if (p_ecs.GetComponentTypeName(compID) == "PlayerAttributes")
						{
							p_ecs.GetComponent<PlayerAttributes>(j).Serialize(writer);
						}
						if (p_ecs.GetComponentTypeName(compID) == "EnemyAttributes")
						{
							p_ecs.GetComponent<EnemyAttributes>(j).Serialize(writer);
						}
					}
				}
				writer->EndObject();
			}
			writer->EndArray();
			++j;
		}
		writer->EndObject();

		writer->EndObject();
		return true;
	}
}

void Night::SceneManager::setSceneToLoad(std::string s)
{
	p_Audio->StopChannel(CAudioEngine::channel_groups::bgm);
	sceneToLoad = s;
}

void Night::SceneManager::checkForSceneToLoad()
{
	if (sceneToLoad != "")
	{
		if (sceneToLoad == "Assets/Scene/Elton.json")
		{
			isGameplay = true;
			Graphic::camera.SetZoomLevel(0.25f);
		}

		if (sceneToLoad == "Assets/Scene/Menu.json")
		{
			isGameplay = false;
			Graphic::camera.SetZoomLevel(1.0f);
			Graphic::camera.SetPosition({ 0.0f,0.0f,0.0f });
		}

		if (sceneToLoad == "Assets/Scene/Options.json")
		{
			isGameplay = false;
			Graphic::camera.SetZoomLevel(1.0f);
			Graphic::camera.SetPosition({ 0.0f,0.0f,0.0f });
		}

		if (sceneToLoad == "Assets/Scene/Game_Over.json")
		{
			isGameplay = false;
			Graphic::camera.SetZoomLevel(1.0f);
			Graphic::camera.SetPosition({ 0.0f,0.0f,0.0f });
		}

		if (sceneToLoad == "Assets/Scene/CutScene.json")
		{
			isGameplay = false;
			Graphic::camera.SetZoomLevel(0.25f);
		}

		DeserializeFromFile(sceneToLoad);
		sceneToLoad = "";
	}
}

void Night::SceneManager::changeScene(std::string s)
{

}