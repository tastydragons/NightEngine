/*!*************************************************************************
****
\file OnDamaged.cpp
\author Elton Teo Zhe Wei, Cheung Jun Yin Matthew
\par DP email: e.teo@digipen.edu, j.cheung"digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the state when the player takes damage

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "NightEngine/Renderer/Camera2D.h"
#include "NightEngine/Renderer/Graphics.h"
#include "OnDamaged.h"
#include "OnIdle.h"
#include "OnDieded.h"


namespace Night
{
	OnDamaged::OnDamaged(StateMachine* stateMachine) : mTimer{ 0.0f }, mDuration{ 0.15f }, mCamMinX{ Night::Graphic::camera.GetPosition().x }, mCamMaxX{ Night::Graphic::camera.GetPosition().x + 0.02f},
		mCamMinY{ Night::Graphic::camera.GetPosition().y }, mCamMaxY{ Night::Graphic::camera.GetPosition().y + 0.02f } {UNREFERENCED_PARAMETER(stateMachine); }

	IStates* OnDamaged::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine);
		UNREFERENCED_PARAMETER(key);
		return nullptr;
	}
	/*!*************************************************************************
	Enter state for Player damaged state
	****************************************************************************/
	void OnDamaged::OnEnter(StateMachine* stateMachine)
	{
		int pDmg = 0;
		for (Entity i = 0; i < p_ecs.GetTotalEntities(); i++) {
			if (p_ecs.HaveComponent<Tag>(i) && p_ecs.GetComponent<Tag>(i).GetTag() == "Enemy") {
				pDmg = p_ecs.GetComponent<EnemyAttributes>(i).mDamage;
			}
		}
		p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mHealth -= pDmg;
		p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDamageDurationTimer = 0.5f;
		p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDamageCoolDown = 0.25f;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Damage");

		if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 5))
		{
			p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[5].should_play = true;
		}
	}

	/*!*************************************************************************
	Update state for Player damaged state
	****************************************************************************/
	void OnDamaged::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		mTimer += Frametime * 2;
		if (mTimer >= mDuration) {
			mTimer = 0.f;
			std::swap(mCamMinX, mCamMaxX);
			std::swap(mCamMinY, mCamMaxY);
		}
		Night::Graphic::camera.SetPosition({ EaseInOutSine(mCamMinX,mCamMaxX,mTimer/mDuration) , EaseInOutSine(mCamMinY,mCamMaxY,mTimer / mDuration), Night::Graphic::camera.GetPosition().z });

		p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDamageDurationTimer -= Frametime;
		p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDashCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDashCoolDown -= Frametime;
		p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mBlockCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mBlockCoolDown -= Frametime;
		if (p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mHealth <= 0)
		{
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mIsDamaged = false;
			p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDamageCoolDown = 0.0f;
			stateMachine->ChangeState(new OnDieded(stateMachine));
		}
		if (p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mDamageDurationTimer <= 0)
		{
			stateMachine->ChangeState(new OnIdle(stateMachine));
		}
	}

	/*!*************************************************************************
	Exit state for Player damaged state
	****************************************************************************/
	void OnDamaged::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<PlayerAttributes>(stateMachine->GetEntityID()).mIsDamaged = false;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		delete this;
	}
}