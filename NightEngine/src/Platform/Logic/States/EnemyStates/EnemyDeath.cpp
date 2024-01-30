/*!*************************************************************************
****
\file EnemyDeath.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the state when enemy dies.

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "EnemyDeath.h"
#include "EnemyHealthDrop.h"
#include "NightEngine/Scripts/GateController.h"

namespace Night
{
	EnemyDeath::EnemyDeath(StateMachine* stateMachine) : mDeathTimer{ 0.3f }, mItemDropChance{0} { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* EnemyDeath::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for when enemy dies
	****************************************************************************/
	void EnemyDeath::OnEnter(StateMachine* stateMachine)
	{
		if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 2))
		{
			p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[2].should_play = true;
		}

		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mEnemyType == EnemyAttributes::EnemyTypes::ENEMY_MELEE)
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("EXOMATA_MELEE_ENEMY_DEATH");
		else
		{
			mDeathTimer = 0.6f;
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("EXOMATA_RANGED_ENEMY_DEATH");
		}
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[0].is_Alive = false;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = false;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsAlive = false;
		mItemDropChance = (int)(rand() * 1.0 / RAND_MAX * 2) + 1;
	}

	/*!*************************************************************************
	Update state for when enemy dies
	****************************************************************************/
	void EnemyDeath::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		mDeathTimer -= Frametime;
		if (mDeathTimer <= 0) {
			mDeathTimer = 0;
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Blank");
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = false;
			if (mItemDropChance == 1)
			{
				stateMachine->ChangeState(new EnemyHealthDrop(stateMachine));
			}
		}
	}

	/*!*************************************************************************
	Exit state for when enemy dies
	****************************************************************************/
	void EnemyDeath::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsAlive = false;
		delete this;
	}
}