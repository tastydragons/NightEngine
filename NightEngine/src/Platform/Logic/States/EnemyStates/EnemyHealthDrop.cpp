/*!*************************************************************************
****
\file EnemyHealthDrop.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the state after the enemy dies and
		becomes a health drop

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "EnemyDeath.h"
#include "EnemyHealthDrop.h"
#include "NightEngine/Scripts/GateController.h"

namespace Night
{
	EnemyHealthDrop::EnemyHealthDrop(StateMachine* stateMachine) : mPickUpTimer{ 0.3f }, mPickUp{ false }, mOriginalColMinX{p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[1].mMin.x},
	mOriginalColMinY{ p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[1].mMin.y }, mOriginalColMaxX{ p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[1].mMax.x },
	mOriginalColMaxY{ p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[1].mMax.y } { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* EnemyHealthDrop::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for health drop
	****************************************************************************/
	void EnemyHealthDrop::OnEnter(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mEnemyType = EnemyAttributes::EnemyTypes::ENEMY_PICKUP;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = true;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Chip_Health_SpriteSheet"); //Set to PickUp
	}

	/*!*************************************************************************
	Update state for health drop
	****************************************************************************/
	void EnemyHealthDrop::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		if (!p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsPickedUp)
		{
			mPickUpTimer -= Frametime;
			if (mPickUpTimer <= 0) {
				mPickUpTimer = 0;
				p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[1].mMin = vec2D(0.01f, 0.01f);
				p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[1].mMax = vec2D(0.01f, 0.01f);
				p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[1].is_Alive = true;
			}
		}
		else if (!mPickUp)
		{
			//player hp regen
			for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
			{
				if (p_ecs.HaveComponent<Tag>(i) && p_ecs.GetComponent<Tag>(i).GetTag() == "Player")
				{
					//play hp regen sound
					if (p_ecs.HaveComponent<Audio>(i) && (p_ecs.GetComponent<Audio>(i).GetSize() > 8))
					{
						p_ecs.GetComponent<Audio>(i)[8].should_play = true;
					}
					//increment player hp based on current player hp
					p_ecs.GetComponent<PlayerAttributes>(i).mHealth < p_ecs.GetComponent<PlayerAttributes>(i).mMaxHealth ? p_ecs.GetComponent<PlayerAttributes>(i).mHealth += (15 + (int)(0.10f * p_ecs.GetComponent<PlayerAttributes>(i).mMaxHealth)):
					p_ecs.GetComponent<PlayerAttributes>(i).mHealth = p_ecs.GetComponent<PlayerAttributes>(i).mMaxHealth; //higher hp will regen less health
					if (p_ecs.GetComponent<PlayerAttributes>(i).mHealth > p_ecs.GetComponent<PlayerAttributes>(i).mMaxHealth)
					{
						p_ecs.GetComponent<PlayerAttributes>(i).mHealth = p_ecs.GetComponent<PlayerAttributes>(i).mMaxHealth;
					}
					mPickUp = true;
					p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = false;
					p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = false;
					p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
					p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Blank"); //Set to PickUp
					break;
				}
			}
		}
	}

	/*!*************************************************************************
	Exit state for health drop
	****************************************************************************/
	void EnemyHealthDrop::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsAlive = false;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[1].mMin = vec2D(mOriginalColMinX,mOriginalColMinY);
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[1].mMax = vec2D(mOriginalColMaxX,mOriginalColMaxY);
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[1].is_Alive = false;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = false;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Blank"); //Set to PickUp
		delete this;
	}
}