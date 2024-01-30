/*!*************************************************************************
****
\file BossDeath.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the state when boss dies.

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "NightEngine/Renderer/Graphics.h"
#include "BossDeath.h"
#include "NightEngine/ECS/SceneManager.h"

namespace Night
{
	BossDeath::BossDeath(StateMachine* stateMachine){ UNREFERENCED_PARAMETER(stateMachine); }

	IStates* BossDeath::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for when enemy dies
	****************************************************************************/
	void BossDeath::OnEnter(StateMachine* stateMachine)
	{
		if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > 2))
		{
			p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[2].should_play = true;
		}

		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("EYEBOSS_Death");

		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[0].is_Alive = false;
		p_ecs.GetComponent<Collider>(stateMachine->GetEntityID()).GetCollisionArray()[1].is_Alive = false;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsAlive = false;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDeathTimer = 4.f;
	}

	/*!*************************************************************************
	Update state for when enemy dies
	****************************************************************************/
	void BossDeath::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDeathTimer -= Frametime;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mFadeofftimer += Frametime;

		if(p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x == p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetMaxIndex() - 1)
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("Blank");

		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDeathTimer < 3.0f)
		{
			Graphic::camera.SetPosition({ 8.0f, 1.0f, 0.0f });
		}
		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDeathTimer <= 0.0f) {
			p_Scene->setSceneToLoad("Assets/Scene/Menu.json");
		}

	}

	/*!*************************************************************************
	Exit state for when enemy dies
	****************************************************************************/
	void BossDeath::OnExit(StateMachine* stateMachine)
	{
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mIsAlive = false;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).is_Animated = false;;
		delete this;
	}
}