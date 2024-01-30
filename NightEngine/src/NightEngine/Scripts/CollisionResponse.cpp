#include "hzpch.h"

#include "CollisionResponse.h"

#include "NightEngine/ECS/SceneManager.h"
#include "NightEngine/Timer/Timer.h"
#include "NightEngine/Timer/FPS.h"

namespace Night
{
	/*!*************************************************************************
	This function clones the script by allocating and constructing a copy of this
	script and returning it
	****************************************************************************/
	CollisionResponse* CollisionResponse::Clone() const
	{
		return new CollisionResponse(*this);
	}
	/*!*************************************************************************
	This function initialises the script. As there are no data members that require
	initialization, this function is empty
	****************************************************************************/
	void CollisionResponse::Start()
	{
		for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
		{
			if (p_ecs.HaveComponent<EnemyAttributes>(i))
			{
				++mTotalEnemies;
			}
			if (p_ecs.HaveComponent<PlayerAttributes>(i))
			{
				mEntityPlayer = i;
			}
		}
	}
	/*!*************************************************************************
	This function runs the logic of the script to check for collision and apply
	a counter-force to the entity's current velocity which is calculated based on
	the current velocity and the collision normal
	****************************************************************************/
	void CollisionResponse::Update(float Frametime)
	{
		UNREFERENCED_PARAMETER(Frametime);
		if (p_ecs.HaveComponent<Collider>(GetScriptEntityID())) {
			auto& col = p_ecs.GetComponent<Collider>(GetScriptEntityID());

			//Taking Damage As Player
			if (p_ecs.HaveComponent<Tag>(GetScriptEntityID()) && p_ecs.GetComponent<Tag>(GetScriptEntityID()).GetTag() == "Player")
			{
				for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
				{
					if (p_ecs.HaveComponent<EnemyAttributes>(i))
					{
						if (p_ecs.GetComponent<Collider>(i).GetCollisionArray()[1].is_Alive)
						{
							if (col.GetCollisionArray()[0].mHit == 2)
							{
								if (p_ecs.GetComponent<EnemyAttributes>(i).mEnemyType == EnemyAttributes::EnemyTypes::ENEMY_PICKUP)
								{
									auto& playerstats = p_ecs.GetComponent<PlayerAttributes>(GetScriptEntityID());
									playerstats.mDamageCoolDown = 0.1f;
									p_ecs.GetComponent<EnemyAttributes>(i).mIsPickedUp = true;
								}
								else
								{
									if ((p_ecs.GetComponent<Transform>(GetScriptEntityID()).GetPos().x > p_ecs.GetComponent<Transform>(i).GetPos().x) && p_ecs.GetComponent<EnemyAttributes>(i).mEnemyFacing != EnemyAttributes::Facing::LEFT)
									{
										if (p_ecs.GetComponent<Transform>(GetScriptEntityID()).GetPos().y <= p_ecs.GetComponent<Transform>(i).GetPos().y + p_ecs.GetComponent<Collider>(i).GetCollisionArray()[1].mMax.y
											&& p_ecs.GetComponent<Transform>(GetScriptEntityID()).GetPos().y >= p_ecs.GetComponent<Transform>(i).GetPos().y - p_ecs.GetComponent<Collider>(i).GetCollisionArray()[1].mMin.y)
										{
											auto& playerstats = p_ecs.GetComponent<PlayerAttributes>(GetScriptEntityID());
											if (playerstats.mDamageCoolDown <= 0.0f)
											{
												playerstats.mIsDamaged = true;
											}
										}
									}
									if (((p_ecs.GetComponent<Transform>(GetScriptEntityID()).GetPos().x < p_ecs.GetComponent<Transform>(i).GetPos().x) && p_ecs.GetComponent<EnemyAttributes>(i).mEnemyFacing != EnemyAttributes::Facing::RIGHT))
									{
										if (p_ecs.GetComponent<Transform>(GetScriptEntityID()).GetPos().y <= p_ecs.GetComponent<Transform>(i).GetPos().y + p_ecs.GetComponent<Collider>(i).GetCollisionArray()[1].mMax.y
											&& p_ecs.GetComponent<Transform>(GetScriptEntityID()).GetPos().y >= p_ecs.GetComponent<Transform>(i).GetPos().y - p_ecs.GetComponent<Collider>(i).GetCollisionArray()[1].mMin.y)
										{
											auto& playerstats = p_ecs.GetComponent<PlayerAttributes>(GetScriptEntityID());
											if (playerstats.mDamageCoolDown <= 0.0f)
											{
												playerstats.mIsDamaged = true;
											}
										}
									}
								}
							}
						}
					}
					if (p_ecs.HaveComponent<Attributes>(i))
					{
						if (p_ecs.GetComponent<Collider>(i).GetCollisionArray()[1].is_Alive)
						{
							if (col.GetCollisionArray()[0].mHit == 2)
							{
								auto& playerstats = p_ecs.GetComponent<PlayerAttributes>(GetScriptEntityID());
								if (playerstats.mDamageCoolDown <= 0.0f)
								{
									playerstats.mIsDamaged = true;
								}
							}
						}
					}
				}
			}
			//Enemy Taking Damage
			if (p_ecs.HaveComponent<Tag>(GetScriptEntityID()) && p_ecs.GetComponent<Tag>(GetScriptEntityID()).GetTag() == "Enemy")
			{
				for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
				{
					if (p_ecs.HaveComponent<PlayerAttributes>(i))
					{
						if (p_ecs.GetComponent<Collider>(i).GetCollisionArray()[1].is_Alive)
						{
							if (col.GetCollisionArray()[0].mHit == 2)
							{
								auto& enemystats = p_ecs.GetComponent<EnemyAttributes>(GetScriptEntityID());
								if (enemystats.mDamageCoolDownTimer <= 0.0f)
								{
									if ((p_ecs.GetComponent<PlayerAttributes>(mEntityPlayer).mFacing == PlayerAttributes::Facing::RIGHT &&
										p_ecs.GetComponent<EnemyAttributes>(GetScriptEntityID()).mEnemyFacing == EnemyAttributes::Facing::LEFT) ||
										(p_ecs.GetComponent<PlayerAttributes>(mEntityPlayer).mFacing == PlayerAttributes::Facing::LEFT &&
											p_ecs.GetComponent<EnemyAttributes>(GetScriptEntityID()).mEnemyFacing == EnemyAttributes::Facing::RIGHT))
									{
										enemystats.mIsDamaged = true;
										p_ecs.GetComponent<PlayerAttributes>(mEntityPlayer).mHitStopTimer = 0.12f;
									}
									else if (p_ecs.GetComponent<PlayerAttributes>(mEntityPlayer).mIsBlocking)
									{
										if (p_ecs.GetComponent<PlayerAttributes>(mEntityPlayer).mIsChargeAttack) {
											enemystats.mIsChargedDamage = true;
										}
										enemystats.mIsDamaged = true;
										p_ecs.GetComponent<PlayerAttributes>(mEntityPlayer).mHitStopTimer = 0.12f;
									}
								}
							}
						}
					}
					if (p_ecs.HaveComponent<Attributes>(i))
					{
						if (p_ecs.GetComponent<Collider>(i).GetCollisionArray()[1].is_Alive)
						{
							if (col.GetCollisionArray()[0].mHit == 2)
							{
								auto& enemystats = p_ecs.GetComponent<EnemyAttributes>(GetScriptEntityID());
								if (enemystats.mEnemyType != EnemyAttributes::EnemyTypes::ENEMY_BOSS && enemystats.mDamageCoolDownTimer <= 0.0f)
								{
									enemystats.mIsDamaged = true;
								}
							}
						}
					}
				}
			}
		}

	}
	/*!*************************************************************************
	This function ends the script by deleting the pointer to this script
	****************************************************************************/
	void CollisionResponse::End()
	{
		delete this;
	}
	/*!*************************************************************************
	This function returns the purpose of the script as a flag for when it is called
	****************************************************************************/
	std::string CollisionResponse::GetScriptName() { return "CollisionResponse"; }
}