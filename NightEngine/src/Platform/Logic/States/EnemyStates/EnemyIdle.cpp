/*!*************************************************************************
****
\file EnemyIdle.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the state when enemy is idle.

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "EnemyIdle.h"
#include "EnemyChase.h"
#include "EnemyDeath.h"
#include "EnemyDamaged.h"


namespace Night
{
	EnemyIdle::EnemyIdle(StateMachine* stateMachine) : mTimer{ 0.0f }, mDuration{ 1.0f }, mMinX{ p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos().x }, 
		mMaxX{ p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos().x}, 
		mMinY{ p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos().y }, 
		mMaxY{ p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos().y} { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* EnemyIdle::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for when enemy is idle state
	****************************************************************************/
	void EnemyIdle::OnEnter(StateMachine* stateMachine)
	{
		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mEnemyType == EnemyAttributes::EnemyTypes::ENEMY_MELEE)
		{
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("EXOMATA_MELEE_ENEMY_HOVERING");
		}
		else
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("EXOMATA_RANGED_ENEMY_HOVERING");
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIdleTimer = 0.5f;
		mMaxX = p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos().x + float((rand() * 1.0 / RAND_MAX * 3) + 1 - 3.0f) / 100.0f;
		mMaxY = p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos().y + float((rand() * 1.0 / RAND_MAX * 3) + 1 - 3.0f) / 100.0f;
		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHitCounter == 0)
		{
			p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHitCounter = (int)(rand() * 1.0 / RAND_MAX * 3) + 1;
		}
	}

	/*!*************************************************************************
	Update state for when enemy is idle state
	****************************************************************************/
	void EnemyIdle::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		p_ecs.GetComponent<RigidBody>(stateMachine->GetEntityID()).SetNextPos(p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos());
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageCoolDownTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageCoolDownTimer -= Frametime;
		//p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown -= Frametime;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIdleTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIdleTimer -= Frametime;
		mTimer += Frametime/2;
		if (mTimer >= mDuration) {
			mTimer = 0.f;
			std::swap(mMinX, mMaxX);
			std::swap(mMinY, mMaxY);
		}
		p_ecs.GetComponent<RigidBody>(stateMachine->GetEntityID()).SetNextPos({ EaseInOutSine(mMinX,mMaxX,mTimer / mDuration) , EaseInOutSine(mMinY,mMaxY,mTimer / mDuration)});
		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsDamaged)
		{
			stateMachine->ChangeState(new EnemyDamaged(stateMachine));
		}
		vec2D playerPos = vec2D();
		bool check = false;
		if (!check)
		{
			for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
			{
				if (p_ecs.HaveComponent<Tag>(i) && p_ecs.GetComponent<Tag>(i).GetTag() == "Player")
				{
					check = true;
					playerPos = p_ecs.GetComponent<Transform>(i).GetPos();
				}
			}
		}
		//if player moves within x radius, set mode to moving
		if (distance(playerPos, p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos()) < 0.3f && check && p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIdleTimer <= 0.0f) {

			stateMachine->ChangeState(new EnemyChase(stateMachine));
		}

		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mHealth <= 0)
		{
			stateMachine->ChangeState(new EnemyDeath(stateMachine));
		}

	}

	/*!*************************************************************************
	Exit state for when enemy is idle state
	****************************************************************************/
	void EnemyIdle::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		delete this;
	}
}