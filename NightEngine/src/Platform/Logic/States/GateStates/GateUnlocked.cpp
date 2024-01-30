/*!*************************************************************************
****
\file GateUnlocked.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the state when the gate on the map is unlocked when the player
		has cleared all enemies in his current area.

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "GateUnlocked.h"
#include "GateLocked.h"

namespace Night
{
	GateUnlocked::GateUnlocked(StateMachine* stateMachine) { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* GateUnlocked::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}
	/*!*************************************************************************
	Enter state for gate unlocked state
	****************************************************************************/
	void GateUnlocked::OnEnter(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[0].is_Alive = false;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = false;
	}

	/*!*************************************************************************
	Update state for gate unlocked state
	****************************************************************************/
	void GateUnlocked::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		UNREFERENCED_PARAMETER(Frametime);
		for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
		{
			if (p_ecs.HaveComponent<EnemyAttributes>(i))
			{
				if (p_ecs.GetComponent<EnemyAttributes>(i).mIsAlive)
				{
					stateMachine->ChangeState(new GateLocked(stateMachine));
				}
			}
		}
	}

	/*!*************************************************************************
	Exit state for gate unlocked state
	****************************************************************************/
	void GateUnlocked::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = true;
		delete this;
	}
}