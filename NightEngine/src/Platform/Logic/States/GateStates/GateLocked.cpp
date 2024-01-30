/*!*************************************************************************
****
\file GateLocked.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the state when the gate on the map is locked, which is only
		unlocked when the player has cleared all enemies in his current area.

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "GateLocked.h"
#include "GateUnlocked.h"

namespace Night
{
	GateLocked::GateLocked(StateMachine* stateMachine) { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* GateLocked::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}
	/*!*************************************************************************
	Enter state for gate locked state
	****************************************************************************/
	void GateLocked::OnEnter(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("LASERGATE_BG_V2");
        p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[0].is_Alive = true;
        p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = true;
	}

	/*!*************************************************************************
	Update state for gate locked state
	****************************************************************************/
	void GateLocked::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		UNREFERENCED_PARAMETER(Frametime);
        int aliveCount{};
        for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
        {
            if (p_ecs.HaveComponent<EnemyAttributes>(i))
            {
				if (p_ecs.GetComponent<EnemyAttributes>(i).mIsAlive)
				{
					++aliveCount;
				}
            }
        }
		vec2D playerPos = vec2D();
		bool check = false;
		for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
		{
			if (p_ecs.HaveComponent<Tag>(i) && p_ecs.GetComponent<Tag>(i).GetTag() == "Player")
			{
				check = true;
				playerPos = p_ecs.GetComponent<Transform>(i).GetPos();
			}
		}
		//if player moves within x radius, set mode to moving
		if (distance(playerPos, p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos()) < 0.4f && check) 
		{
			if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 0))
			{
				p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[0].should_play = true;
			}
		}
        if (aliveCount == 0)
        {
			stateMachine->ChangeState(new GateUnlocked(stateMachine));
        }
	}

	/*!*************************************************************************
	Exit state for gate locked state
	****************************************************************************/
	void GateLocked::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetMaxIndex() - 1;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = false;
		if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 0) && p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[0].is_Playing == true)
		{
			p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[0].should_stop = true;
		}
		delete this;
	}
}