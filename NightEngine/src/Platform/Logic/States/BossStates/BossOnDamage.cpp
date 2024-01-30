/*!*************************************************************************
****
\file BossOnDamage.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the state when takes damage from the player

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "BossOnDamage.h"
#include "BossChasing.h"
#include "BossDeath.h"
#include "BossAttack.h"
#include "BossIdle.h"

namespace Night
{
	BossOnDamage::BossOnDamage(StateMachine* stateMachine) { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* BossOnDamage::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for when boss is damaged state
	****************************************************************************/
	void BossOnDamage::OnEnter(StateMachine* stateMachine)
	{
		int pDmg = 0;
		for (Entity i = 0; i < p_ecs.GetTotalEntities(); i++) {
			if (p_ecs.HaveComponent<Tag>(i) && p_ecs.GetComponent<Tag>(i).GetTag() == "Player") {
				if (p_ecs.GetComponent<PlayerAttributes>(i).mIsBlocking)
				{
					pDmg = p_ecs.GetComponent<PlayerAttributes>(i).mDamage / p_ecs.GetComponent<PlayerAttributes>(i).mDamage;
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
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("EYEBOSS_Damaged");
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHealth -= pDmg;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHealth = p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHealth;
		--p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHitCounter;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageDurationTimer = 0.25f;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageCoolDownTimer = 0.1f;

		if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 1))
		{
			p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[1].should_play = true;
		}
	}

	/*!*************************************************************************
	Update state for when boss is damaged state
	****************************************************************************/
	void BossOnDamage::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageDurationTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageDurationTimer -= Frametime;
		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageDurationTimer <= 0) {
			if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHealth <= 0)
			{
				if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 2))
				{
					p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[2].should_play = true;
				}
				if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 3))
				{
					p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[3].should_play = true;
				}

				stateMachine->ChangeState(new BossDeath(stateMachine));
			}
			else if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHitCounter == 0)
			{	
				stateMachine->ChangeState(new BossAttack(stateMachine));
			}
			else
			{
				stateMachine->ChangeState(new BossIdle(stateMachine));
			}
		}
	}

	/*!*************************************************************************
	Exit state for when boss is damaged state
	****************************************************************************/
	void BossOnDamage::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsDamaged = false;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageCoolDownTimer = 0.2f;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		delete this;
	}
}