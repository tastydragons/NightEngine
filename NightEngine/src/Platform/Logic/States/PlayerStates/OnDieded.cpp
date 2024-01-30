/*!*************************************************************************
****
\file OnDieded.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the state when the player dies

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "OnDieded.h"
#include "NightEngine/ECS/SceneManager.h"

namespace Night
{
	OnDieded::OnDieded(StateMachine* stateMachine) { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* OnDieded::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for Player died state
	****************************************************************************/
	void OnDieded::OnEnter(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Death");
		p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDeathDuration = 2.f;
		if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 6))
		{
			p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[6].should_play = true;
		}
		
	}

	/*!*************************************************************************
	Update state for Player died state
	****************************************************************************/
	void OnDieded::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		UNREFERENCED_PARAMETER(stateMachine);
		p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDeathDuration -= Frametime;

		if (p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x == p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetMaxIndex()-1)
		{
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Blank");
		}
		if (p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDeathDuration <= 0.0f)
		{
			p_Scene->setSceneToLoad("Assets/Scene/Game_Over.json");
		}
	}

	/*!*************************************************************************
	Exit state for Player died state
	****************************************************************************/
	void OnDieded::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsAlive = false;
		delete this;
	}
}