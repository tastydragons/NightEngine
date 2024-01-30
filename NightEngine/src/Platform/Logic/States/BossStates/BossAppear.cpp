/*!*************************************************************************
****
\file BossAppear.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the state when boss first appears

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "BossIdle.h"
#include "BossAppear.h"

namespace Night
{
	BossAppear::BossAppear(StateMachine* stateMachine) { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* BossAppear::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for when Boss is appearing state
	****************************************************************************/
	void BossAppear::OnEnter(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("EYEBOSS_Idle_1");
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_SpriteSheet = true;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = true;
		//p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).SetPos({ 6.714.f, 1.0f });
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mEnemyFacing = EnemyAttributes::Facing::LEFT;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIdleTimer = 1.0f;
	}

	/*!*************************************************************************
	Update state for when Boss is appearing state
	****************************************************************************/
	void BossAppear::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIdleTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIdleTimer -= Frametime;
		float dist = 0;
		vec2D playerPos = vec2D();
		auto& transform = p_ecs.GetComponent<Transform>(stateMachine->GetEntityID());
		auto& rigidbody = p_ecs.GetComponent<RigidBody>(stateMachine->GetEntityID());
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
		if (check) {
			dist = distance(transform.GetPos(), playerPos);
			rigidbody.SetDir(transform.GetPos().x - playerPos.x, transform.GetPos().y - playerPos.y);
			vec2D newVel = vec2D(0.0f, 0.0f);
			newVel = rigidbody.GetVel();
			newVel = rigidbody.GetDir() * length(rigidbody.GetAccel()) / 2.f;
			newVel.x = 0;
			newVel.y *= -50;
			newVel = p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mPhys.accelent(rigidbody.GetVel(), newVel, Frametime);
			vec2D nextPos = transform.GetPos() + rigidbody.GetVel();
			rigidbody.SetNextPos({ nextPos.x, nextPos.y + 0.06f });
		}
		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIdleTimer <= 0.0f)
		{
			stateMachine->ChangeState(new BossIdle(stateMachine));
		}
	}

	/*!*************************************************************************
	Exit state for when Boss is appearing state
	****************************************************************************/
	void BossAppear::OnExit(StateMachine* stateMachine)
	{
		UNREFERENCED_PARAMETER(stateMachine);
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown = 1.5f;
		delete this;
	}
}