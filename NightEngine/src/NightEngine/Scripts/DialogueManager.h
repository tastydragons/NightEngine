#pragma once

#include "IScript.h"

#include "NightEngine/Core/Input.h"
#include "NightEngine/Renderer/Camera2D.h"

#include "NightEngine/GUI/GUI.h"

namespace Night
{
	/*!*************************************************************************
	Class for Dialogue Manager Script
	****************************************************************************/
	class DialogueManager : public IScript
	{
	public:
		/*!*************************************************************************
		Default constructor for Dialogue Manager
		****************************************************************************/
		DialogueManager() : mDialogues{ "Dialogue1", "Dialogue2", "Dialogue3", "Dialogue4","Tutorial1","Tutorial2","Tutorial3",
										"Tutorial4","Tutorial5","Tutorial6","Dialogue7","Dialogue8","Dialogue9","Dialogue10",
										"Dialogue11","Dialogue12" },
			counter{ 0 }, MaxCounter{ 4 } {}
		/*!*************************************************************************
		Default destructor for Dialogue Manager
		****************************************************************************/
		~DialogueManager() = default;
		/*!*************************************************************************
		Returns a new copy of Dialogue Manager Script
		****************************************************************************/
		virtual DialogueManager* Clone() const override
		{
			return new DialogueManager(*this);
		}
		/*!*************************************************************************
		Start State of DialogueManager Script
		****************************************************************************/
		virtual void Start() override
		{
		}
		/*!*************************************************************************
		Update Loop of DialogueManager Script
		****************************************************************************/
		virtual void Update(float Frametime) override
		{

			UNREFERENCED_PARAMETER(Frametime);
			Entity i{ 0 };

			for (Entity e = 0; e < p_ecs.GetTotalEntities(); ++e)
			{
				if (p_ecs.HaveComponent<PlayerAttributes>(e))
				{
					if (p_ecs.GetComponent<Transform>(e).GetPos().x >= p_ecs.GetComponent<Transform>(GetScriptEntityID()).GetPos().x)
					{
						p_ecs.GetComponent<Sprite>(e).SetTexture("Idle");
						p_ecs.GetComponent<Sprite>(GetScriptEntityID()).SetLayerOrder(7);
						p_ecs.GetComponent<Transform>(GetScriptEntityID()).GetPos().y = p_ecs.GetComponent<Transform>(e).GetPos().y - 0.15f;
						if (p_ecs.GetComponent<Sprite>(GetScriptEntityID()).GetTexture() == "Tutorial1") { MaxCounter = 7; counter = 4; }
						else if (p_ecs.GetComponent<Sprite>(GetScriptEntityID()).GetTexture() == "Tutorial4") { MaxCounter = 10; counter = 7; }
						else if (p_ecs.GetComponent<Sprite>(GetScriptEntityID()).GetTexture() == "Dialogue7") { MaxCounter = 14; counter = 10; }
						else if (p_ecs.GetComponent<Sprite>(GetScriptEntityID()).GetTexture() == "Dialogue11") { MaxCounter = 16; counter = 14; }
						if (p_Input->MousePressed(GLFW_MOUSE_BUTTON_LEFT))
						{
							++counter;
							if (p_ecs.HaveComponent<Audio>(GetScriptEntityID()) && ((p_ecs.GetComponent<Audio>(GetScriptEntityID())).GetSize() > 0))
							{
								p_ecs.GetComponent<Audio>(GetScriptEntityID())[0].should_play = true;
							}
							p_Input->mMouseStatus[GLFW_MOUSE_BUTTON_LEFT] = GLFW_RELEASE;
						}
						if (counter < MaxCounter)
						{
							p_ecs.GetComponent<Sprite>(GetScriptEntityID()).SetTexture(mDialogues[counter].c_str());
							for (i = 0; i < p_ecs.GetTotalEntities(); ++i)
							{
								if (p_ecs.HaveComponent<Logic>(i) && p_ecs.HaveComponent<PlayerAttributes>(i))
								{
									p_ecs.GetComponent<Logic>(i).GetScriptByName("PlayerControl")->mScriptPause = true;
								}
								if (p_ecs.HaveComponent<Logic>(i) && p_ecs.HaveComponent<EnemyAttributes>(i) && p_ecs.GetComponent<EnemyAttributes>(i).mEnemyType != EnemyAttributes::EnemyTypes::ENEMY_BOSS)
								{
									p_ecs.GetComponent<Logic>(i).GetScriptByName("EnemyScript")->mScriptPause = true;
								}
							}
						}
						else
						{
							for (i = 0; i < p_ecs.GetTotalEntities(); ++i)
							{
								if (p_ecs.HaveComponent<Logic>(i) && p_ecs.HaveComponent<PlayerAttributes>(i))
								{
									p_ecs.GetComponent<Logic>(i).GetScriptByName("PlayerControl")->mScriptPause = false;
								}
								if (p_ecs.HaveComponent<Logic>(i) && p_ecs.HaveComponent<EnemyAttributes>(i) && p_ecs.GetComponent<EnemyAttributes>(i).mEnemyType != EnemyAttributes::EnemyTypes::ENEMY_BOSS)
								{
									p_ecs.GetComponent<Logic>(i).GetScriptByName("EnemyScript")->mScriptPause = false;
								}
							}
							this->mScriptPause = true;
							p_ecs.GetComponent<Sprite>(GetScriptEntityID()).SetTexture("Blank");
						}
					}
				}
			}


		}
		/*!*************************************************************************
		End State for DialogueManager
		****************************************************************************/
		virtual void End() override
		{
			delete this;
		}
		/*!*************************************************************************
		Returns the name of Script
		****************************************************************************/
		virtual std::string GetScriptName() override
		{
			return "DialogueManager";
		}

		virtual void SetScriptEntityID(Entity& entity) override { entityID = entity; }

		virtual Entity& GetScriptEntityID() override { return entityID; }

		int counter;
	private:
		std::string mDialogues[16];
		int MaxCounter;
	};
}