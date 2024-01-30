/*!*************************************************************************
****
\file EnemyDamaged.cpp
\author Elton Teo Zhe Wei, Cheung Jun Yin Matthew
\par DP email: e.teo@digipen.edu, j.cheung@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the state when enemy is damaged.

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "EnemyDamaged.h"
#include "EnemyDeath.h"
#include "EnemyChase.h"
#include "EnemyRetreat.h"
#include "EnemyAttack.h"
#include "EnemyIdle.h"
#include "NightEngine/Scripts/GateController.h"

namespace Night
{
	EnemyDamaged::EnemyDamaged(StateMachine* stateMachine) { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* EnemyDamaged::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for when enemy is damaged state
	****************************************************************************/
	void EnemyDamaged::OnEnter(StateMachine* stateMachine)
	{
		int pDmg = 0;
		for (Entity i = 0; i < p_ecs.GetTotalEntities(); i++) {
			if (p_ecs.HaveComponent<Tag>(i) && p_ecs.GetComponent<Tag>(i).GetTag() == "Player") {
				if (p_ecs.GetComponent<PlayerAttributes>(i).mIsBlocking)
				{
					pDmg = p_ecs.GetComponent<PlayerAttributes>(i).mDamage/p_ecs.GetComponent<PlayerAttributes>(i).mDamage;
				}
				else if (p_ecs.GetComponent<PlayerAttributes>(i).mIsChargeAttack)
				{
					pDmg = p_ecs.GetComponent<PlayerAttributes>(i).mDamage * 2;
				}
				else
				{
					pDmg = p_ecs.GetComponent<PlayerAttributes>(i).mDamage;
				}
			}
		}
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHealth -= pDmg;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHealth = p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHealth;
		--p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHitCounter;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageDurationTimer = 0.25f;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageCoolDownTimer = 0.1f;
		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mEnemyType == EnemyAttributes::EnemyTypes::ENEMY_MELEE)
		{
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Enemy_Damaged_Normal_Attack");
		}
		else
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("EXOMATA_RANGED_ENEMY_HOVERING");

		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsChargedDamage) {
			//play charged damage sound
			if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 7))
			{
				p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[7].should_play = true;
				p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsChargedDamage = false;
			}
		}
		else {
			if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 1))
			{
				p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[1].should_play = true;
			}
		}
	}

	/*!*************************************************************************
	Update state for when enemy is damaged state
	****************************************************************************/
	void EnemyDamaged::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageDurationTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageDurationTimer -= Frametime;
		//enemy damaged knockback
		for (Entity i = 0; i < p_ecs.GetTotalEntities(); i++) {
			if (p_ecs.HaveComponent<Tag>(i) && p_ecs.GetComponent<Tag>(i).GetTag() == "Player" && p_ecs.GetComponent<PlayerAttributes>(i).mIsChargeAttack) 
			{
				auto& transform = p_ecs.GetComponent<Transform>(stateMachine->GetEntityID());
				auto& rigidbody = p_ecs.GetComponent<RigidBody>(stateMachine->GetEntityID());
				vec2D dir = rigidbody.GetDir();
				dir = dir * 350.0f;
				rigidbody.SetVel(p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mPhys.accelent(rigidbody.GetVel(), dir, Frametime));
				vec2D nextPos = (transform.GetPos() + rigidbody.GetVel());
				rigidbody.SetNextPos(nextPos);
			}
			if (p_ecs.HaveComponent<Tag>(i) && p_ecs.GetComponent<Tag>(i).GetTag() == "Player" && p_ecs.GetComponent<PlayerAttributes>(i).mIsBlocking)
			{
				auto& transform = p_ecs.GetComponent<Transform>(stateMachine->GetEntityID());
				auto& rigidbody = p_ecs.GetComponent<RigidBody>(stateMachine->GetEntityID());
				vec2D dir = rigidbody.GetDir();
				dir = dir * 350.0f;
				rigidbody.SetVel(p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mPhys.accelent(rigidbody.GetVel(), dir, Frametime));
				vec2D nextPos = (transform.GetPos() + rigidbody.GetVel());
				rigidbody.SetNextPos(nextPos);
			}
		}

		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageDurationTimer <= 0) {
			if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHealth <= 0)
			{
				stateMachine->ChangeState(new EnemyDeath(stateMachine));
			}
			else
			{
				if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mEnemyType == EnemyAttributes::EnemyTypes::ENEMY_RANGED)
				{	
					stateMachine->ChangeState(new EnemyRetreat(stateMachine));
				}
				else if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHitCounter == 0)
				{
					stateMachine->ChangeState(new EnemyAttack(stateMachine));
				}
				else
				{
					stateMachine->ChangeState(new EnemyIdle(stateMachine));
				}
			}
		}
	}

	/*!*************************************************************************
	Exit state for when enemy is damaged state
	****************************************************************************/
	void EnemyDamaged::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsDamaged = false;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageCoolDownTimer = 0.1f;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		delete this;
	}
}