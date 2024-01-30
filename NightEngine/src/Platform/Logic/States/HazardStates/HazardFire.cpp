/*!*************************************************************************
****
\file HazardFire.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the hazard fire state

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "HazardFire.h"
#include "HazardIdle.h"

namespace Night
{
	HazardFire::HazardFire(StateMachine* stateMachine) : mCurrTrans{ vec2D() }, mCurrScale{ vec2D() } {UNREFERENCED_PARAMETER(stateMachine); }

	IStates* HazardFire::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for Hazard fire state
	****************************************************************************/
	void HazardFire::OnEnter(StateMachine* stateMachine)
	{
		mCurrTrans = p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos();
		mCurrScale = p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetScale();
		if (p_ecs.HaveComponent<Attributes>(stateMachine->GetEntityID()))
		{
			if (p_ecs.GetComponent<Attributes>(stateMachine->GetEntityID()).mHazardType == Attributes::HazardTypes::HAZARD_GROUND)
			{
				p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).SetPos(mCurrTrans.x + -0.023f, mCurrTrans.y + 0.070f);
				p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Ground_Hazards_Fire");
			}
			if (p_ecs.GetComponent<Attributes>(stateMachine->GetEntityID()).mHazardType == Attributes::HazardTypes::HAZARD_LASER)
			{
				p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).SetScale(0.600f, 0.600f);
				p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).SetPos(mCurrTrans.x + 0.083f, mCurrTrans.y - 0.096f);
				p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Laser_Hazard_Fire");
			}
		}
		if (p_ecs.HaveComponent<Attributes>(stateMachine->GetEntityID()))
		{
			p_ecs.GetComponent<Attributes>(stateMachine->GetEntityID()).mFireDurationTimer = 0.25f;
		}
		
	
	}

	/*!*************************************************************************
	Update state for Hazard fire state
	****************************************************************************/
	void HazardFire::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		vec2D playerPos = vec2D();
		bool check = false;
		for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
		{
			if (p_ecs.HaveComponent<Tag>(i) && p_ecs.GetComponent<Tag>(i).GetTag() == "Player")
			{
				check = true;
				playerPos = p_ecs.GetComponent<Transform>(i).GetPos();
			}
		}

		if (p_ecs.HaveComponent<Attributes>(stateMachine->GetEntityID()))
		{
			p_ecs.GetComponent<Attributes>(stateMachine->GetEntityID()).mFireDurationTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<Attributes>(stateMachine->GetEntityID()).mFireDurationTimer -= Frametime;
		}
		if (p_ecs.HaveComponent<Collider>(stateMachine->GetEntityID()))
		{
			p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = true;
		}
		if (p_ecs.GetComponent<Attributes>(stateMachine->GetEntityID()).mFireDurationTimer > 0.0f)
		{
			//if player moves within x radius, set mode to moving
			if (distance(playerPos, p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos()) < 0.4f && check) {
				if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 0))
				{
					p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[0].should_play = true;
				}
			}
		}
		if (p_ecs.GetComponent<Attributes>(stateMachine->GetEntityID()).mFireDurationTimer <= 0.0f)
		{
			p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = false;
			if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 0))
			{
				p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[0].should_play = false;
			}
			stateMachine->ChangeState(new HazardIdle(stateMachine));
		}
		
	}

	/*!*************************************************************************
	Exit state for Hazard fire state
	****************************************************************************/
	void HazardFire::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = false;
		p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).SetPos(mCurrTrans.x, mCurrTrans.y);
		p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).SetScale(mCurrScale.x, mCurrScale.y);
		delete this;
	}
}