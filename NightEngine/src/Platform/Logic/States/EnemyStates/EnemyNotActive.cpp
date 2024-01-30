/*!*************************************************************************
****
\file EnemyNotActive.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the state when enemy is not in range of the player. This is a 
		transitional stage for the enemies to spawn when in player proximity.

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "EnemyNotActive.h"
#include "EnemyIdle.h"
#include "EnemyChase.h"
#include "EnemyDamaged.h"

namespace Night
{
	EnemyNotActive::EnemyNotActive(StateMachine* stateMachine) : spawnBehind{ true } { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* EnemyNotActive::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for when enemy is not active state
	****************************************************************************/
	void EnemyNotActive::OnEnter(StateMachine* stateMachine)
	{
		if (p_ecs.HaveComponent<EnemyAttributes>(stateMachine->GetEntityID())) {
			p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsAlive = false;
		}
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = false;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_SpriteSheet = false;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[0].is_Alive = false;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[1].is_Alive = false;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Blank");
		spawnBehind = rand() % 2;
	}

	/*!*************************************************************************
	Update state for when enemy is not active state
	****************************************************************************/
	void EnemyNotActive::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		UNREFERENCED_PARAMETER(Frametime);
		vec2D playerPos = vec2D();
		for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
		{
			if (p_ecs.HaveComponent<Tag>(i) && p_ecs.GetComponent<Tag>(i).GetTag() == "Player")
			{
				playerPos = p_ecs.GetComponent<Transform>(i).GetPos();
			}
		}
		//if player moves within x radius, set mode to moving
		/*if (spawnBehind) {
			if ((playerPos.x - p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos().x) > 0) {
				if (distance(playerPos, p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos()) >= 0.2f) {
					p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsAlive = true;
					stateMachine->ChangeState(new EnemyIdle(stateMachine));
				}
			}
		}*/
		/*else {
		}*/

		if (distance(playerPos, p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos()) < 0.5f) {
			p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsAlive = true;
			stateMachine->ChangeState(new EnemyIdle(stateMachine));
		}
		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsAlive)
		{
			stateMachine->ChangeState(new EnemyIdle(stateMachine));
		}
	}

	/*!*************************************************************************
	Exit state for when enemy is not active state
	****************************************************************************/
	void EnemyNotActive::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = true;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_SpriteSheet = true;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[0].is_Alive = true;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID())[1].is_Alive = false;
		delete this;
	}
}