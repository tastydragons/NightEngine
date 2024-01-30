/*!*************************************************************************
****
\file EnemyRetreat.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic for the state when enemy is retreating, on every hit there is a chance
		that the enemy will retreat before returning to attack the player.

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "hzpch.h"
#include "EnemyRetreat.h"
#include "EnemyIdle.h"
#include "EnemyDamaged.h"
#include "EnemyChase.h"

namespace Night
{
	std::default_random_engine retreatGenerator;
	std::uniform_int_distribution<>retreatDistribution(3, 6);
	EnemyRetreat::EnemyRetreat(StateMachine* stateMachine) { UNREFERENCED_PARAMETER(stateMachine); }

	IStates* EnemyRetreat::HandleInput(StateMachine* stateMachine, const int& key)
	{
		UNREFERENCED_PARAMETER(stateMachine); UNREFERENCED_PARAMETER(key);
		return nullptr;
	}

	/*!*************************************************************************
	Enter state for when enemy is retreating state
	****************************************************************************/
	void EnemyRetreat::OnEnter(StateMachine* stateMachine)
	{
		if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mEnemyType == EnemyAttributes::EnemyTypes::ENEMY_MELEE)
		{
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("EXOMATA_MELEE_ENEMY_HOVERING");
		}
		else
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).SetTexture("EXOMATA_RANGED_ENEMY_HOVERING");
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mRetreatDurationTimer = 0.5f;
	}

	/*!*************************************************************************
	Update state for when enemy is retreating state
	****************************************************************************/
	void EnemyRetreat::OnUpdate(StateMachine* stateMachine, float Frametime)
	{
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageCoolDownTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageCoolDownTimer -= Frametime;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown <= 0.0f ? 0.0f : p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown -= Frametime;
		p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageDurationTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mDamageDurationTimer -= Frametime;
		float dist = 0;
		vec2D playerPos = vec2D();
		bool check = false;
		auto& transform = p_ecs.GetComponent<Transform>(stateMachine->GetEntityID());
		auto& rigidbody = p_ecs.GetComponent<RigidBody>(stateMachine->GetEntityID());
		if (!check)
		{
			for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
			{
				if (p_ecs.HaveComponent<Tag>(i) && p_ecs.GetComponent<Tag>(i).GetTag() == "Player")
				{
					playerPos = p_ecs.GetComponent<Transform>(i).GetPos();
					check = true;
				}
			}
		}
		if (distance(playerPos, p_ecs.GetComponent<Transform>(stateMachine->GetEntityID()).GetPos()) < 0.4f && check)
		{
			if (p_ecs.HaveComponent<Audio>(stateMachine->GetEntityID()) && (p_ecs.GetComponent<Audio>(stateMachine->GetEntityID()).GetSize() > retreatDistribution(retreatGenerator)))
			{
				p_ecs.GetComponent<Audio>(stateMachine->GetEntityID())[retreatDistribution(retreatGenerator)].should_play = true;
			}
		}
		if (check) {
			p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mRetreatDurationTimer <= 0.0f ? 0.0f : p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mRetreatDurationTimer -= Frametime;
			rigidbody.SetDir(transform.GetPos().x - playerPos.x, transform.GetPos().y - playerPos.y);
			rigidbody.SetDir(-rigidbody.GetDir().x, -rigidbody.GetDir().y);
			vec2D newVel = vec2D(0.0f, 0.0f);
			newVel = rigidbody.GetVel();
			if (rigidbody.GetDir().x > 0)
			{
				p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mEnemyFacing = EnemyAttributes::Facing::LEFT;

			}
			else
			{
				p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mEnemyFacing = EnemyAttributes::Facing::RIGHT;
			}
			p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetUVCoor().x = 512.0f;
			newVel = rigidbody.GetDir() * length(rigidbody.GetAccel()) / 2.f;
			newVel = p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mPhys.accelent(rigidbody.GetVel(), newVel, Frametime);
			if (newVel.x > -99 && newVel.y < 99) {
				newVel = newVel * -1;
				rigidbody.SetVel(newVel);
				if (newVel.x < 0 && transform.GetScale().x < 0) {
					transform.GetScale().x *= -1;
				}
				if (newVel.x > 0 && transform.GetScale().x > 0) {
					transform.GetScale().x *= -1;
				}
			}
			vec2D nextPos = transform.GetPos() + rigidbody.GetVel();
			rigidbody.SetNextPos(nextPos);
			dist = distance(transform.GetPos(), playerPos);
			//Attack Range
			//check if within range. If not, set to moving state
			if (p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mEnemyType == EnemyAttributes::EnemyTypes::ENEMY_MELEE)
			{
				if (dist > 0.35f || p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mRetreatDurationTimer <= 0.0f)
				{
					//if within range to attack, set mode to attacking
					rigidbody.SetDir(transform.GetPos().x - playerPos.x, transform.GetPos().y - playerPos.y);
					vec2D dirVel = vec2D(0.0f, 0.0f);
					dirVel = rigidbody.GetVel();
					if (rigidbody.GetDir().x > 0)
					{
						p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mEnemyFacing = EnemyAttributes::Facing::LEFT;

					}
					else
					{
						p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mEnemyFacing = EnemyAttributes::Facing::RIGHT;
					}
					if (dirVel.x > -99 && dirVel.y < 99) {
						dirVel = dirVel * -1;
						rigidbody.SetVel(dirVel);
						if (dirVel.x < 0 && transform.GetScale().x < 0) {
							transform.GetScale().x *= -1;
						}
						if (dirVel.x > 0 && transform.GetScale().x > 0) {
							transform.GetScale().x *= -1;
						}
					}
					stateMachine->ChangeState(new EnemyIdle(stateMachine));
				}
			}
			else
			{
				if (dist > 0.30f || p_ecs.GetComponent<EnemyAttributes>(stateMachine->GetEntityID()).mAttackCoolDown <= 0.0f)
				{
					stateMachine->ChangeState(new EnemyChase(stateMachine));
				}
			}
		}
	}

	/*!*************************************************************************
	Exit state for when enemy is retreating state
	****************************************************************************/
	void EnemyRetreat::OnExit(StateMachine * stateMachine)
	{
		p_ecs.GetComponent<Sprite>(stateMachine->GetEntityID()).GetIndex().x = 0;
		delete this;
	}
}