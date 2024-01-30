/*!*************************************************************************
****
\file OnChargeAttack_2.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	2nd Charge attacking state for player

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "OnChargeAttack_2.h"
#include "OnIdle.h"
#include "OnBlock.h"
#include "OnDamaged.h"
#include "OnAttack_2.h"
#include "NightEngine/Timer/Timer.h"

namespace Night
{
	OnChargeAttack_2::OnChargeAttack_2(StateMachine* stateMachine) : mTimer{ 0.0f } { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* OnChargeAttack_2::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine);
		UNREFERENCED_PARAMETER(key);
		return nullptr;
	}
	/*!*************************************************************************
		Enter state for Player Charge Attack 2
	****************************************************************************/
	void OnChargeAttack_2::OnEnter(StateMachine* stateMachine)
	{
		if (p_ecs.HaveComponent<PlayerAttributes>(stateMachine->GetEntityID())) {
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mChargeTimer = 0.6f;
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mCooldownTimer = 0.3f;
		}
		if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 0))
		{
			p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[0].should_play = true;
		}
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Normal_Attack_Swing2");
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = false;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		mTimer = 0.1f;
	}

	/*!*************************************************************************
		Update state for Player Charge Attack 2
	****************************************************************************/
	void OnChargeAttack_2::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		if (p_ecs.HaveComponent<PlayerAttributes>(stateMachine->GetEntityID())) {
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mBlockCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mBlockCoolDown -= Frametime;
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDashCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDashCoolDown -= Frametime;
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDamageCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDamageCoolDown -= Frametime;
		}
		if (p_Input->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && !p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mIsChargeAttack)
		{
			mTimer <= 0.0f ? 0.0f : mTimer -= Frametime;
			if (mTimer <= 0.0f)
			{
				p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("CA2");
				p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = true;
				p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mChargeTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mChargeTimer -= Frametime;
				if (p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mChargeTimer <= 0.0f)
				{
					p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = false;
					//play charged sound
					if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 13))
					{
						p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[13].should_play = true; // looped charging
					}
				}
			}
		}
		else if (p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mChargeTimer <= 0.0f)
		{
			//play charging sound
			if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 12))
			{
				p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[12].should_play = true; //charge atk start
			}
			if (p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mHitStopTimer <= 0.0f)
			{
				p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = true;
				p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mIsChargeAttack = true;
				p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mCooldownTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mCooldownTimer -= Frametime;
				auto& transform = p_ecs.GetComponent<Transform>(stateMachine->GetEntityID());
				auto& rigidbody = p_ecs.GetComponent<RigidBody>(stateMachine->GetEntityID());
				vec2D dir = rigidbody.GetDir();
				dir = dir * 20.0f;
				rigidbody.SetVel(p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mPhys.accelent(rigidbody.GetVel(), dir, Frametime));
				vec2D nextPos = (transform.GetPos() + rigidbody.GetVel());
				rigidbody.SetNextPos(nextPos);
				if (p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mCooldownTimer <= 0.15f)
				{
					p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = true;
				}
				if (p_ecs.HaveComponent<PlayerAttributes>(stateMachine->GetEntityID())) 
				{
					if (p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mIsDamaged)
					{
						stateMachine->ChangeState(new OnDamaged(stateMachine));
					}
					if (p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mCooldownTimer <= 0.0f)
					{
						stateMachine->ChangeState(new OnIdle(stateMachine));
					}
				}
			}
			else
			{
				p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mHitStopTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mHitStopTimer -= Frametime;
				p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = false;
			}
		}
		else if (p_Input->MouseIsReleased(GLFW_MOUSE_BUTTON_LEFT) && !p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mIsChargeAttack)
		{
			stateMachine->ChangeState(new OnAttack_2(stateMachine));
		}

		if (p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mIsDamaged)
		{
			stateMachine->ChangeState(new OnDamaged(stateMachine));
		}
	}
	/*!*************************************************************************
		Exit state for Player Charge Attack 2
	****************************************************************************/
	void OnChargeAttack_2::OnExit(StateMachine* stateMachine)
	{
		//play charging sound
		if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 12))
		{
			p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[12].should_play = false; //charge atk start
		}
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = false;
		p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mIsChargeAttack = false;
		p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mHitStopTimer = 0.0f;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = true;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		delete this;
	}
}