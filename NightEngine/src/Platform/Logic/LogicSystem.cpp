#include "hzpch.h"

#include "LogicSystem.h"

#include "NightEngine/GUI/GUI.h"
#include "NightEngine/Timer/Timer.h"

namespace Night
{
	/*!*************************************************************************
	Initialize Logic System
	****************************************************************************/
	void LogicSystem::Init()
	{

	}

	/*!*************************************************************************
	Update Loop for Logic System
	****************************************************************************/
	void LogicSystem::Update(float Frametime)
	{
		Timer::GetInstance().Start(Systems::LOGIC);
		Timer::GetInstance().GetDT(Systems::LOGIC);

		for (const auto& entity : mEntities)
		{
			auto& LogicComp = p_ecs.GetComponent<Logic>(entity);
			LogicComp.SetScriptEntity(entity);
			if (!LogicComp.GetScript().empty())
			{
				for (auto i = LogicComp.GetScript().begin(); i != LogicComp.GetScript().end(); ++i)
				{
					if ((p_GUI->check_pause() == false || (*i)->GetScriptName() == "PauseMenu" || (*i)->GetScriptName() == "SliderScript"))
					{
						if (!(*i)->GetScriptInit())
						{
							(*i)->Start();
							(*i)->SetScriptInit();
						}
						if (!(*i)->GetScriptPaused())
							(*i)->Update(Frametime);
					}
				}
			}
		}
		Timer::GetInstance().Update(Systems::LOGIC);
	}

	/*!*************************************************************************
	End State for Logic System which will delete all script data
	****************************************************************************/
	void LogicSystem::End()
	{
		for (const auto& entity : mEntities)
		{
			auto& LogicComp = p_ecs.GetComponent<Logic>(entity);
			for (auto i = LogicComp.GetScript().begin(); i != LogicComp.GetScript().end(); ++i)
			{
				(*i)->End();
			}
		}
	}
}