/*!*************************************************************************
****
\file OnIdle.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the state when the player is idle

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "OnIdle.h"
#include "OnMove.h"
#include "OnAttack_1.h"
#include "OnDash.h"
#include "OnBlock.h"
#include "OnDamaged.h"
#include "OnChargeAttack_1.h"
#include "NightEngine/Timer/Timer.h"
#include "NightEngine/Timer/FPS.h"

namespace Night
{
	float time{};
	OnIdle::OnIdle(StateMachine* stateMachine) : mTempDamage{ p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDamage } { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* OnIdle::HandleInput(StateMachine* stateMachine, const int& key)
	{
		if ((key == GLFW_KEY_W || key == GLFW_KEY_A || key == GLFW_KEY_S || key == GLFW_KEY_D) && p_Input->KeyHold(key))
		{
			return new OnMove(stateMachine);
		}
		if (key == GLFW_KEY_SPACE && p_Input->isKeyPressed(key) && p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDashCoolDown <= 0.0f)
		{
			return new OnDash(stateMachine);
		}
		if (key == GLFW_MOUSE_BUTTON_LEFT && p_Input->MousePressed(key))
		{
			return new OnChargeAttack_1(stateMachine);
		}
		if (key == GLFW_MOUSE_BUTTON_RIGHT && p_Input->MousePressed(key) && p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mBlockCoolDown <= 0.0f)
		{
			return new OnBlock(stateMachine);
		}
		if (key == GLFW_KEY_F4 && p_Input->KeyPressed(GLFW_KEY_F4))
		{
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mHealth != p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mMaxHealth ?
				p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mHealth = p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mMaxHealth : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mHealth;
		}
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for when player is in idle state
	****************************************************************************/
	void OnIdle::OnEnter(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = false;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Idle");
	}

	/*!*************************************************************************
	Update state for when player is in idle state
	****************************************************************************/
	void OnIdle::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		if (p_ecs.HaveComponent<PlayerAttributes>(stateMachine->GetEntityID())) {
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mCooldownTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mCooldownTimer -= Frametime;
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mBlockCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mBlockCoolDown -= Frametime;
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mBlockDurationTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mBlockDurationTimer -= Frametime;
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDashCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDashCoolDown -= Frametime;
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDamageCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDamageCoolDown -= Frametime;
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mHitStopTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mHitStopTimer -= Frametime;
			if (p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mIsDamaged)
			{
				stateMachine->ChangeState(new OnDamaged(stateMachine));
			}
		}
	}

	/*!*************************************************************************
	Exit state for when player is in idle state
	****************************************************************************/
	void OnIdle::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		delete this;
	}
}