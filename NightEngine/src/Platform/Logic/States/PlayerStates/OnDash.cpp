/*!*************************************************************************
****
\file OnDash.cpp
\author Cheung Jun Yin Matthew
\par DP email: j.cheung@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains logic for player dash state.

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "OnDash.h"
#include "OnIdle.h"

namespace Night
{
	OnDash::OnDash(StateMachine* stateMachine) { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* OnDash::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for Player dashing state
	****************************************************************************/
	void OnDash::OnEnter(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDashDurationTimer = 0.5f;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Dash");
		if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 4))
		{
			p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[4].should_play = true;
		}
		vec2D tempOff = p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[0].mOffset;
		float tempRad = p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[0].mRadius;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[0].mCol = Collider::ColliderType::dashCirc;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[0].mOffset = tempOff;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[0].mRadius = tempRad;
	}

	/*!*************************************************************************
	Update state for Player dashing state
	****************************************************************************/
	void OnDash::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDashDurationTimer -= Frametime*2;
		auto& pRigid = p_ecs.GetComponent<RigidBody>(stateMachine->GetEntityID());
		auto& pTrans = p_ecs.GetComponent<Transform>(stateMachine->GetEntityID());
		vec2D dir = p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDir;
		dir = dir * 100.0f * 0.5f;
		pRigid.SetVel(p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mPhys.accelent(pRigid.GetVel(), dir, Frametime));
		pRigid.SetVel(p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mPhys.friction(pRigid.GetVel(), Frametime));
		vec2D nextPos = (pTrans.GetPos() + pRigid.GetVel());
		pRigid.SetNextPos(nextPos);
		if (p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDashDurationTimer <= 0.0f)
		{
			stateMachine->ChangeState(new OnIdle(stateMachine));
		}
	}

	/*!*************************************************************************
	Exit state for Player dashing state
	****************************************************************************/
	void OnDash::OnExit(StateMachine* stateMachine)
	{
		vec2D tempOff = p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[0].mOffset;
		float tempRad = p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[0].mRadius;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[0].mCol = Collider::ColliderType::circle;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[0].mOffset = tempOff;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[0].mRadius = tempRad;
		p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDashCoolDown = 1.0f;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		delete this;
	}
}