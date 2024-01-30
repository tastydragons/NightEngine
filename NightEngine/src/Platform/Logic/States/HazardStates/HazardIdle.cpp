/*!*************************************************************************
****
\file HazardIdle.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the hazard idle state

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "HazardIdle.h"
#include "HazardCharging.h"

namespace Night
{
	HazardIdle::HazardIdle(StateMachine* stateMachine) { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* HazardIdle::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	enter state for hazard idle state
	****************************************************************************/
	void HazardIdle::OnEnter(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Blank");
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = false;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_SpriteSheet = false;
		if (p_ecs.HaveComponent<Attributes>(stateMachine->GetEntityID()))
		{
			p_ecs.GetComponent<Attributes>(stateMachine->GetEntityID()).mChargeCoolDown = 2.0f;
		}
	}

	/*!*************************************************************************
	Update state for hazard idle state
	****************************************************************************/
	void HazardIdle::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		if (p_ecs.HaveComponent<Attributes>(stateMachine->GetEntityID()))
		{
			p_ecs.GetComponent<Attributes>(stateMachine->GetEntityID()).mChargeCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<Attributes>(stateMachine->GetEntityID()).mChargeCoolDown -=Frametime;
		}
		if (p_ecs.GetComponent<Attributes>(stateMachine->GetEntityID()).mChargeCoolDown <= 0.0f)
		{
			stateMachine->ChangeState(new HazardCharging(stateMachine));
		}
	}

	/*!*************************************************************************
	Exit state for hazard idle state
	****************************************************************************/
	void HazardIdle::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = true;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_SpriteSheet = true;

		delete this;
	}
}