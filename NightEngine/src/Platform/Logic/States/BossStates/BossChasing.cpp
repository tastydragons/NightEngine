/*!*************************************************************************
****
\file BossChase.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the enemy chasing state

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "BossChasing.h"
#include "BossAttack.h"
#include "BossOnDamage.h"

namespace Night
{
	BossChasing::BossChasing(StateMachine* stateMachine) { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* BossChasing::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for when enemy is chasing state
	****************************************************************************/
	void BossChasing::OnEnter(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("EYEBOSS_Idle_1");
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[0].is_Alive = false;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[1].is_Alive = false;
	}

	/*!*************************************************************************
	Update state for when enemy is chasing state
	****************************************************************************/
	void BossChasing::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		if (p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetTexture() == "EYEBOSS_Idle_1" && p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x == p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetMaxIndex() - 1)
		{
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("EYEBOSS_Idle_2");
		}
		float dist = 0;
		vec2D playerPos = vec2D();
		bool check = false;
		auto& transform = p_ecs.GetComponent<Transform>(stateMachine->GetEntityID());
		auto& rigidbody = p_ecs.GetComponent<RigidBody>(stateMachine->GetEntityID());
		if (!check)
		{
			for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
			{
				if (p_ecs.HaveComponent<Tag>(i) && p_ecs.GetComponent<Tag>(i).GetTag() == "Player")
				{
					playerPos = p_ecs.GetComponent<Transform>(i).GetPos();
					check = true;
					dist = distance(transform.GetPos(), playerPos);
				}
			}
		}
		if (check)
		{
			if (dist <= 0.7f)
			{
				p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown -= Frametime;
				rigidbody.SetDir(transform.GetPos().x - playerPos.x, transform.GetPos().y - playerPos.y);
				vec2D newVel = vec2D(0.0f, 0.0f);
				newVel = rigidbody.GetVel();
				newVel = rigidbody.GetDir() * length(rigidbody.GetAccel()) / 2.f;
				newVel.y *= 4;
				newVel = p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mPhys.accelent(rigidbody.GetVel(), newVel, Frametime);
				vec2D nextPos = transform.GetPos();
				nextPos.y -= rigidbody.GetVel().y - 0.008f;
				rigidbody.SetNextPos(nextPos);
			}
			if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown <= 0.0f)
			{
				stateMachine->ChangeState(new BossAttack(stateMachine));
			}
		}
	}

	/*!*************************************************************************
	Exit state for when enemy is chasing state
	****************************************************************************/
	void BossChasing::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		delete this;
	}
}