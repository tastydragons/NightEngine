/*!*************************************************************************
****
\file BossIdle.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the state when boss is in an idle state. This is a transition stage
		which goes to the chase state

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "BossIdle.h"
#include "BossChasing.h"
#include "BossOnDamage.h"

namespace Night
{
	BossIdle::BossIdle(StateMachine* stateMachine) { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* BossIdle::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for when Boss is idle
	****************************************************************************/
	void BossIdle::OnEnter(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("EYEBOSS_Idle_1");
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_SpriteSheet = true;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = true;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[0].is_Alive = true;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[1].is_Alive = false;
		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHitCounter == 0)
		{
			p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHitCounter = (int)(rand() * 1.0 / RAND_MAX * 5) + 1;
		}
	}

	/*!*************************************************************************
	Update state for when Boss is idle
	****************************************************************************/
	void BossIdle::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		//UNREFERENCED_PARAMETER(Frametime);
		if (p_ecs.HaveComponent<EnemyAttributes>(stateMachine->GetEntityID())) {
			p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown -= Frametime;
			p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageCoolDownTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageCoolDownTimer -= Frametime;
			
			if (p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetTexture() == "EYEBOSS_Idle_1" && p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x == p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetMaxIndex()-1)
			{
				p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
				p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("EYEBOSS_Idle_2");
			}
			if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsDamaged && p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown > 0.0f)
			{
				stateMachine->ChangeState(new BossOnDamage(stateMachine));
			}
			if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown <= 0.0f)
			{
				stateMachine->ChangeState(new BossChasing(stateMachine));
			}
		}
	}

	/*!*************************************************************************
	Exit state for when Boss is idle
	****************************************************************************/
	void BossIdle::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown = 1.5f;
		delete this;
	}
}