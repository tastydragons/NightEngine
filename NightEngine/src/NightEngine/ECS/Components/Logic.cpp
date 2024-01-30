#include "hzpch.h"

#include "Logic.h"

#include "NightEngine/Scripts/CollisionResponse.h"
#include "NightEngine/Scripts/ButtonResponse.h"
#include "NightEngine/Scripts/PlayerControl.h"
#include "NightEngine/Scripts/EnemyScript.h"
#include "NightEngine/Scripts/ScenerioScript.h"
#include "NightEngine/Scripts/SliderScript.h"
#include "NightEngine/Scripts/AudioManager.h"
#include "NightEngine/Scripts/PauseMenu.h"
#include "NightEngine/Scripts/HUDController.h"
#include "NightEngine/Scripts/EnemyScript.h"
#include "NightEngine/Scripts/GateController.h"
#include "NightEngine/Scripts/BackgroundAudio.h"
#include "NightEngine/Scripts/CutScene.h"
#include "NightEngine/Scripts/HazardScript.h"
#include "NightEngine/Scripts/DialogueManager.h"
#include "NightEngine/Scripts/BossScript.h"

namespace Night
{
	/*!*************************************************************************
	Ctor for Logic Component
	****************************************************************************/
	Logic::Logic() { entityID = GetComponentEntityID(); }

	/*!*************************************************************************
	Copy Ctor
	****************************************************************************/
	Logic::Logic(const Logic& rhs)
	{
		mScriptNameVector = rhs.mScriptNameVector;
		mScriptsVector.resize(rhs.mScriptsVector.size());
		for (size_t i = 0; i < mScriptsVector.size(); ++i)
		{
			mScriptsVector[i] = rhs.mScriptsVector[i]->Clone();
			mScriptsVector[i]->SetScriptEntityID(this->GetComponentEntityID());
		}
	}
	/*!*************************************************************************
	Copy Assignment for operator overload
	****************************************************************************/
	Logic& Logic::operator= (const Logic& rhs)
	{
		mScriptNameVector = rhs.mScriptNameVector;
		mScriptsVector.resize(rhs.mScriptsVector.size());
		for (size_t i = 0; i < mScriptsVector.size(); ++i)
		{
			mScriptsVector[i] = rhs.mScriptsVector[i]->Clone();
			mScriptsVector[i]->SetScriptEntityID(this->GetComponentEntityID());
		}
		return *this;
	}

	/*!*************************************************************************
	Sets the Script's entity to retrieve data only from that entity
	****************************************************************************/
	void Logic::SetScriptEntity(Entity entity)
	{
		this->SetComponentEntityID(entity);
		for (auto i = mScriptsVector.begin(); i != mScriptsVector.end(); ++i)
		{
			(*i)->SetScriptEntityID(this->GetComponentEntityID());
		}
	}

	/*!*************************************************************************
	Deserialize for Logic Component
	****************************************************************************/
	bool Logic::Deserialize(const rapidjson::Value& obj)
	{
		for (int i = 0; i < obj["ScriptCount"].GetInt(); ++i)
		{
			mScriptNameVector.push_back(obj[std::to_string(i).c_str()].GetString());
		}
		for (size_t j = 0; j < mScriptNameVector.size(); ++j)
		{
			if (mScriptNameVector[j] == "PlayerControl")
			{
				mScriptsVector.push_back(new PlayerControl(GetComponentEntityID()));
			}
			if (mScriptNameVector[j] == "EnemyScript")
			{
				mScriptsVector.push_back(new EnemyScript(GetComponentEntityID()));
			}
			if (mScriptNameVector[j] == "BossScript")
			{
				mScriptsVector.push_back(new BossScript(GetComponentEntityID()));
			}
			if (mScriptNameVector[j] == "CollisionResponse")
			{
				mScriptsVector.push_back(new CollisionResponse());
			}
			if (mScriptNameVector[j] == "ButtonResponse")
			{
				mScriptsVector.push_back(new ButtonResponse());
			}
			if (mScriptNameVector[j] == "ScenerioScript")
			{
				mScriptsVector.push_back(new ScenerioScript());
			}
			if (mScriptNameVector[j] == "HUDController")
			{
				mScriptsVector.push_back(new HUDController());
			}
			if (mScriptNameVector[j] == "AudioManager")
			{
				mScriptsVector.push_back(new AudioManager());
			}
			if (mScriptNameVector[j] == "GateController")
			{
				mScriptsVector.push_back(new GateController(GetComponentEntityID()));
			}
			if (mScriptNameVector[j] == "BackgroundAudio")
			{
				mScriptsVector.push_back(new BackgroundAudio());
			}
			if (mScriptNameVector[j] == "SliderScript")
			{
				mScriptsVector.push_back(new SliderScript());
			}
			if (mScriptNameVector[j] == "PauseMenu")
			{
				mScriptsVector.push_back(new PauseMenu());
			}
			if (mScriptNameVector[j] == "HazardScript")
			{
				mScriptsVector.push_back(new HazardScript(GetComponentEntityID()));
			}
			if (mScriptNameVector[j] == "CutScene")
			{
				mScriptsVector.push_back(new CutScene());
			}
			if (mScriptNameVector[j] == "DialogueManager")
			{
				mScriptsVector.push_back(new DialogueManager());
			}
		}
		return true;
	}

	/*!*************************************************************************
	Serialize for Logic Component
	****************************************************************************/
	bool Logic::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
	{
		int i = 0;
		for (i = 0; i < mScriptNameVector.size(); ++i)
		{
			writer->Key(std::to_string(i).c_str());
			writer->String(mScriptNameVector[i].c_str());
		}
		writer->Key("ScriptCount");
		writer->Int(i);
		return true;
	}

	/*!*************************************************************************
	Inserts the scripts into the vector to loop through
	****************************************************************************/
	void Logic::InsertScript(IScript* script, Entity entity)
	{
		this->SetComponentEntityID(entity);
		script->SetScriptEntityID(this->GetComponentEntityID());
		mScriptNameVector.push_back(script->GetScriptName());
		mScriptsVector.push_back(script);
	}

	void Logic::DeleteScript(std::string scriptname)
	{
		for (int idx = 0; idx < mScriptNameVector.size(); idx++) {
			if (mScriptNameVector[idx] == scriptname) {
				mScriptNameVector.erase(mScriptNameVector.begin() + idx);
				mScriptsVector.erase(mScriptsVector.begin() + idx);
			}
		}
	}

	/*!*************************************************************************
	Clear all Scripts inside the Vector
	****************************************************************************/
	void Logic::ClearAllScripts()
	{
		mScriptNameVector.clear();
		mScriptsVector.clear();
		for (auto i = mScriptsVector.begin(); i != mScriptsVector.end(); ++i)
		{
			(*i)->End();
		}
	}

	IScript* Logic::GetScriptByName(std::string ScriptName)
	{
		for (size_t i = 0; i < mScriptNameVector.size(); ++i)
		{
			if (mScriptNameVector[i] == ScriptName)
			{
				return mScriptsVector[i];
			}
		}
		return nullptr;
	}
}