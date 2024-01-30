/*!*************************************************************************
****
\file EnemyTransition.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for calculating the chance for the enemy to go into retreat state or
		continue attacking state after each attack.

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "EnemyTransition.h"
#include "EnemyAttack.h"
#include "EnemyRetreat.h"
#include "EnemyChase.h"
#include "EnemyIdle.h"

namespace Night
{
	EnemyTransition::EnemyTransition(StateMachine* stateMachine) { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* EnemyTransition::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for when enemy transition state
	****************************************************************************/
	void EnemyTransition::OnEnter(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("EXOMATA_MELEE_ENEMY_HOVERING");
	}

	/*!*************************************************************************
	Update state for when enemy transition state
	****************************************************************************/
	void EnemyTransition::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageCoolDownTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageCoolDownTimer -= Frametime;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown -= Frametime;
		if ((rand() % 100) <= 80 || p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHitCounter != 0) {
			vec2D playerPos = vec2D();
			bool check = false;
			if (!check)
			{
				for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
				{
					if (p_ecs.HaveComponent<Tag>(i) && p_ecs.GetComponent<Tag>(i).GetTag() == "Player")
					{
						playerPos = p_ecs.GetComponent<Transform>(i).GetPos();
						check = true;
					}
				}
			}
			if (check && distance(playerPos, p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos()) <= 0.10f
				&& p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown <= 0.0f) {
				stateMachine->ChangeState(new EnemyAttack(stateMachine));
			}
			else
			{
				stateMachine->ChangeState(new EnemyIdle(stateMachine));
			}
		}
		else
		{
			stateMachine->ChangeState(new EnemyRetreat(stateMachine));
		}
	}

	/*!*************************************************************************
	Exit state for when enemy transition state
	****************************************************************************/
	void EnemyTransition::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		delete this;
	}
}