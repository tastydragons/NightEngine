/*!*************************************************************************
****
\file On_Attack_2.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	2nd attacking state for player

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "OnAttack_2.h"
#include "OnIdle.h"
#include "OnBlock.h"
#include "OnDamaged.h"

namespace Night
{
	std::default_random_engine atk2generator;
	std::uniform_int_distribution<>atk2range(8, 11);
	OnAttack_2::OnAttack_2(StateMachine* stateMachine) { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* OnAttack_2::HandleInput(StateMachine* stateMachine, const int& key)
	{
		if (key == GLFW_MOUSE_BUTTON_LEFT && p_Input->MousePressed(key))
		{
		}
		if (key == GLFW_MOUSE_BUTTON_RIGHT && p_Input->MousePressed(key) && p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mBlockCoolDown <= 0.0f)
		{
			return new OnBlock(stateMachine);
		}
		return nullptr;
	}

	/*!*************************************************************************
		Enter state for Player Attack 2
	****************************************************************************/
	void OnAttack_2::OnEnter(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mCooldownTimer = 0.3f;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Normal_Attack_Swing2");
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > atk2range(atk2generator)))
		{
			p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[atk2range(atk2generator)].should_play = true;
		}
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = true;
	}

	/*!*************************************************************************
	update state for Player Attack 2
	****************************************************************************/
	void OnAttack_2::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		if (p_ecs.HaveComponent<PlayerAttributes>(stateMachine->GetEntityID())) {
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mBlockCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mBlockCoolDown -= Frametime;
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDashCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDashCoolDown -= Frametime;
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDamageCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDamageCoolDown -= Frametime;
		}
		if (p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mHitStopTimer <= 0.0f)
		{
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = true;
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mCooldownTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mCooldownTimer -= Frametime;
			auto& transform = p_ecs.GetComponent<Transform>(stateMachine->GetEntityID());
			auto& rigidbody = p_ecs.GetComponent<RigidBody>(stateMachine->GetEntityID());
			vec2D dir = rigidbody.GetDir();
			rigidbody.SetVel(p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mPhys.accelent(rigidbody.GetVel(), dir, Frametime));
			vec2D nextPos = (transform.GetPos() + rigidbody.GetVel());
			rigidbody.SetNextPos(nextPos);
			if (p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mCooldownTimer <= 0.15f)
			{
				p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = true;
			}
			if (p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mIsDamaged)
			{
				stateMachine->ChangeState(new OnDamaged(stateMachine));
			}
			if (p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mCooldownTimer <= 0.0f)
			{
				p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = false;
				stateMachine->ChangeState(new OnIdle(stateMachine));
			}
		}
		else
		{
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mHitStopTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mHitStopTimer -= Frametime;
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = false;
		}
	}
	/*!*************************************************************************
	Exit state for Player Attack 2
	****************************************************************************/
	void OnAttack_2::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = false;
		p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mHitStopTimer = 0.0f;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = true;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		delete this;
	}
}