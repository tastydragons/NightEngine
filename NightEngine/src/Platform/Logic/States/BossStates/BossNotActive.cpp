/*!*************************************************************************
****
\file BossNotActive.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the state when Boss is not in range of the player. This is a
		transitional stage for the enemies to spawn when in player proximity.

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "BossNotActive.h"
#include "BossAppear.h"

namespace Night
{
	BossNotActive::BossNotActive(StateMachine* stateMachine) { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* BossNotActive::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for when Boss is not active state
	****************************************************************************/
	void BossNotActive::OnEnter(StateMachine* stateMachine)
	{
		if (p_ecs.HaveComponent<EnemyAttributes>(stateMachine->GetEntityID())) {
			p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsAlive = false;
		}
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Blank");
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[0].is_Alive = false;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[1].is_Alive = false;
	}

	/*!*************************************************************************
	Update state for when Boss is not active state
	****************************************************************************/
	void BossNotActive::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		UNREFERENCED_PARAMETER(Frametime);
		vec2D playerPos = vec2D();
		int EnemyPopulation = 0;
		for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
		{
			if (p_ecs.HaveComponent<Tag>(i) && p_ecs.GetComponent<Tag>(i).GetTag() == "Player")
			{
				playerPos = p_ecs.GetComponent<Transform>(i).GetPos();
			}
			if (p_ecs.HaveComponent<EnemyAttributes>(i))
			{
				if (p_ecs.GetComponent<EnemyAttributes>(i).mEnemyType != EnemyAttributes::EnemyTypes::ENEMY_BOSS)
				{
					if (p_ecs.GetComponent<EnemyAttributes>(i).mHealth != 0 )
					{
						EnemyPopulation++;
					}
				}
			}
		}

		if(EnemyPopulation == 0 || playerPos.x > 5.67f) // the enemy has been kill
			stateMachine->ChangeState(new BossAppear(stateMachine));
		
	}

	/*!*************************************************************************
	Exit state for when Boss is not active state
	****************************************************************************/
	void BossNotActive::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[0].is_Alive = true;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[1].is_Alive = false;
		if (p_ecs.HaveComponent<EnemyAttributes>(stateMachine->GetEntityID())) {
			p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsAlive = true;
		}
		delete this;
	}
}