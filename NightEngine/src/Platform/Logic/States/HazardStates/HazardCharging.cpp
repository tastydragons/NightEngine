/*!*************************************************************************
****
\file HazardCharging.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the hazard charging state

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "HazardCharging.h"
#include "HazardFire.h"

namespace Night
{
	HazardCharging::HazardCharging(StateMachine* stateMachine) { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* HazardCharging::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for hazard charging state
	****************************************************************************/
	void HazardCharging::OnEnter(StateMachine* stateMachine)
	{
		if (p_ecs.HaveComponent<Attributes>(stateMachine->GetEntityID()))
		{
			if (p_ecs.GetComponent<Attributes>(stateMachine->GetEntityID()).mHazardType == Attributes::HazardTypes::HAZARD_GROUND)
			{
				p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("GroundElectricHazard");
			}
			if (p_ecs.GetComponent<Attributes>(stateMachine->GetEntityID()).mHazardType == Attributes::HazardTypes::HAZARD_LASER)
			{
				p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Laser_Hazard_Charging");
			}
		}
		if (p_ecs.HaveComponent<Attributes>(stateMachine->GetEntityID()))
		{
			p_ecs.GetComponent<Attributes>(stateMachine->GetEntityID()).mFiringCoolDown = 0.25f;
		}
	}

	/*!*************************************************************************
	Update state for hazard charging state
	****************************************************************************/
	void HazardCharging::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		if (p_ecs.HaveComponent<Attributes>(stateMachine->GetEntityID()))
		{
			p_ecs.GetComponent<Attributes>(stateMachine->GetEntityID()).mFiringCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<Attributes>(stateMachine->GetEntityID()).mFiringCoolDown -= Frametime;
		}
		if (p_ecs.GetComponent<Attributes>(stateMachine->GetEntityID()).mFiringCoolDown <= 0.0f)
		{
			stateMachine->ChangeState(new HazardFire(stateMachine));
		}
	}

	/*!*************************************************************************
	Exit state for hazard charging state
	****************************************************************************/
	void HazardCharging::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		delete this;
	}
}