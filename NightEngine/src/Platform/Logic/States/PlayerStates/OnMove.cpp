/*!*************************************************************************
****
\file OnMove.cpp
\author Cheung Jun Yin Matthew
\par DP email: j.cheung@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains logic for player moving state.

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "OnMove.h"
#include "OnAttack_1.h"
#include "OnIdle.h"
#include "OnBlock.h"
#include "OnDamaged.h"
#include "OnDash.h"

namespace Night
{
	OnMove::OnMove(StateMachine* stateMachine) : footstep_switch{ true } { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* OnMove::HandleInput(StateMachine* stateMachine, const int& key)
	{
		if (key == GLFW_MOUSE_BUTTON_LEFT && p_Input->MousePressed(key))
		{
			return new OnAttack_1(stateMachine);

		}
		if (key == GLFW_MOUSE_BUTTON_RIGHT && p_Input->MousePressed(key) && p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mBlockCoolDown <= 0.0f)
		{
			return new OnBlock(stateMachine);
		}
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for when player is in moving state
	****************************************************************************/
	void OnMove::OnEnter(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Running");
	}

	/*!*************************************************************************
	Update state for when player is in moving state
	****************************************************************************/
	void OnMove::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		if (p_ecs.HaveComponent<PlayerAttributes>(stateMachine->GetEntityID())) {
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mCooldownTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mCooldownTimer -= Frametime;
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mBlockCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mBlockCoolDown -= Frametime;
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDashCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDashCoolDown -= Frametime;
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDamageCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDamageCoolDown -= Frametime;
			vec2D vel{};
			if (p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mIsDamaged)
			{
				stateMachine->ChangeState(new OnDamaged(stateMachine));
			}
			else
			{
				if ((p_Input->KeyHold(GLFW_KEY_W) || p_Input->KeyHold(GLFW_KEY_D)
					|| p_Input->KeyHold(GLFW_KEY_S) || p_Input->KeyHold(GLFW_KEY_A)))
				{
					if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 2))
					{
						auto& aud = p_ecs.GetComponent<Audio>(stateMachine->GetEntityID());
						if (footstep_switch == true && (aud[1].triggered == false && aud[1].is_Playing == false && aud[1].should_play == false) &&
							(aud[2].triggered == false && aud[2].is_Playing == false && aud[2].should_play == false))
						{
							aud[1].should_play = true;
							footstep_switch = false;
						}
						else if (footstep_switch == false && (aud[1].triggered == false && aud[1].is_Playing == false && aud[1].should_play == false) &&
							(aud[2].triggered == false && aud[2].is_Playing == false && aud[2].should_play == false))
						{
							aud[2].should_play = true;
							footstep_switch = true;
						}
					}

					if (p_Input->KeyHold(GLFW_KEY_W)) {
						vel.y = p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mVel.y/3;
					}
					if (p_Input->KeyHold(GLFW_KEY_S)) {
						vel.y = -p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mVel.y/3;
					}
					if (p_Input->KeyHold(GLFW_KEY_D)) {
						vel.x = p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mVel.x;
						p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDir.x = 1.0f;
						p_ecs.GetComponent<RigidBody>(stateMachine->GetEntityID()).SetDir(1.0f, 0.0f);
						p_ecs.GetComponent <PlayerAttributes>(stateMachine->GetEntityID()).mFacing = PlayerAttributes::Facing::RIGHT;
						if (p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetScale().x < 0)
						{
							p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).SetScale(-p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetScale().x,
								p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetScale().y);
						}
					}
					if (p_Input->KeyHold(GLFW_KEY_A)) {
						vel.x = -p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mVel.x;
						p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDir.x = -1.0f;
						p_ecs.GetComponent<RigidBody>(stateMachine->GetEntityID()).SetDir(-1.0f, 0.0f);
						p_ecs.GetComponent <PlayerAttributes>(stateMachine->GetEntityID()).mFacing = PlayerAttributes::Facing::LEFT;
						if (p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetScale().x > 0)
						{
							p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).SetScale(-p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetScale().x,
								p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetScale().y);
						}
					}
					if (p_Input->KeyPressed(GLFW_KEY_SPACE) && p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDashCoolDown <= 0.0f)
					{
						stateMachine->ChangeState(new OnDash(stateMachine));
					}
					auto& pRigid = p_ecs.GetComponent<RigidBody>(stateMachine->GetEntityID());
					auto& pTrans = p_ecs.GetComponent<Transform>(stateMachine->GetEntityID());
					if (vel.x != 0.0f || vel.y != 0.0f) {
						Normalize(vel, vel);
						vel = vel * 30;
					}
					pRigid.SetVel(p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mPhys.friction(pRigid.GetVel(), Frametime));
					pRigid.SetVel(p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mPhys.accelent(pRigid.GetVel(), vel, Frametime));
					vec2D nextPos = pTrans.GetPos() + pRigid.GetVel();
					pRigid.SetNextPos(nextPos);
				}
				else
				{
					stateMachine->ChangeState(new OnIdle(stateMachine));
				}
			}
		}
	}

	/*!*************************************************************************
	Exit state for when player is in moving state
	****************************************************************************/
	void OnMove::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		delete this;
	}
}