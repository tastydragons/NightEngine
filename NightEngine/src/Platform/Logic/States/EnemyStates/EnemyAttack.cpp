/*!*************************************************************************
****
\file EnemyAttack.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the enemy attack state

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "EnemyAttack.h"
#include "EnemyChase.h"
#include "EnemyDeath.h"
#include "EnemyIdle.h"
#include "EnemyDamaged.h"
#include "EnemyTransition.h"

namespace Night
{
	EnemyAttack::EnemyAttack(StateMachine* stateMachine) { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* EnemyAttack::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine);
		UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for enemy attacking state
	****************************************************************************/
	void EnemyAttack::OnEnter(StateMachine* stateMachine)
	{
		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mEnemyType == EnemyAttributes::EnemyTypes::ENEMY_MELEE)
			p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackTimer = 0.47f;
		else//for now only have range enemy
			p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackTimer = 1.92f;

		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mEnemyType == EnemyAttributes::EnemyTypes::ENEMY_MELEE)
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("EXOMATA_ENEMY_MELEE_ATTACKING_SPRITESHEET");
		else//for now only have range enemy
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("EXOMATA_RANGED_ENEMY_ATTACKING");

		if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 0))
		{
			p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[0].should_play = true;
		}

		for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
		{
			if (p_ecs.HaveComponent<Tag>(i) && p_ecs.GetComponent<Tag>(i).GetTag() == "RangeLaser")
			{
				p_ecs.GetComponent<Sprite>(i).GetIndex().x = 0;
			}
		}
	}

	/*!*************************************************************************
	Update state for enemy attacking state
	****************************************************************************/
	void EnemyAttack::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		float offsetlaser = 0;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageCoolDownTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageCoolDownTimer -= Frametime;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackTimer -= Frametime;
		//if is ranged enemy and passed ranged enemy atk timer
		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mEnemyType == EnemyAttributes::EnemyTypes::ENEMY_RANGED && p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackTimer < 0.96f)
		{
			p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = true;
			for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
			{
				if (p_ecs.HaveComponent<Tag>(i) && p_ecs.GetComponent<Tag>(i).GetTag() == "RangeLaser")
				{
					p_ecs.GetComponent<Sprite>(i).SetLayerOrder(5);
					if (p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetScale().x <= 0) offsetlaser = 0.2f;
					else offsetlaser = -0.2f;
					p_ecs.GetComponent<Transform>(i).SetPos(p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos().x + offsetlaser,
						p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos().y - 0.018f);
					p_ecs.GetComponent<Transform>(i).SetScale(p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetScale());
				}
			}
			if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackTimer <= 0.0f)
			{
				p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = false;
				stateMachine->ChangeState(new EnemyTransition(stateMachine));
			}
			if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsDamaged && p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHitCounter != 0)
			{
				stateMachine->ChangeState(new EnemyDamaged(stateMachine));
			}
			if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHealth <= 0)
			{
				stateMachine->ChangeState(new EnemyDeath(stateMachine));
			}
		}
		else
		{
			//if melee enemy and passed melee enemy atk timer 
			if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackTimer < 0.23f)
			{
				p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = true;
			}

			if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackTimer <= 0.0f)
			{
				p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = false;
				stateMachine->ChangeState(new EnemyTransition(stateMachine));
			}
			if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsDamaged && p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHitCounter != 0)
			{
				stateMachine->ChangeState(new EnemyDamaged(stateMachine));
			}
			if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHealth <= 0)
			{
				stateMachine->ChangeState(new EnemyDeath(stateMachine));
			}
		}
	}

	/*!*************************************************************************
	Exit state for enemy attacking state
	****************************************************************************/
	void EnemyAttack::OnExit(StateMachine* stateMachine)
	{
		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mEnemyType == EnemyAttributes::EnemyTypes::ENEMY_RANGED)
		{
			for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
			{
				if (p_ecs.HaveComponent<Tag>(i) && p_ecs.GetComponent<Tag>(i).GetTag() == "RangeLaser")
				{
					p_ecs.GetComponent<Sprite>(i).SetLayerOrder(8);
				}
			}
		}
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = false;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown = 1.25f;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		delete this;
	}
}