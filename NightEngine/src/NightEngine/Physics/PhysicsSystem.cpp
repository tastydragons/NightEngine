#include "hzpch.h"

#include "PhysicsSystem.h"

#include "NightEngine/ECS/ECS.h"
#include "NightEngine/ResourceManager/ResourceManager.h"

namespace Night
{
	/*!*************************************************************************
	This function initialises the system. As there are no data members that require
	initialization, this function is empty
	****************************************************************************/
	void PhysicsSystem::Init() {}
	/*!*************************************************************************
	This function runs the logics of the system to update the entity positions
	by setting the current position as the entity's next position which has been
	calculated previously
	****************************************************************************/
	void PhysicsSystem::Update()
	{
		Timer::GetInstance().Start(Systems::PHYSICS);
		Timer::GetInstance().GetDT(Systems::PHYSICS);
		for (const auto& entity : mEntities)
		{
			auto& mTrans = p_ecs.GetComponent<Transform>(entity);
			auto& mRigid = p_ecs.GetComponent<RigidBody>(entity);
			if (mRigid.GetNextPos().x != 0 || mRigid.GetNextPos().y != 0) {
				if (distance(mTrans.GetPos(), mRigid.GetNextPos()) < 1.0f) {
					mTrans.SetPos(mRigid.GetNextPos());
				}
				else {
					mRigid.SetNextPos(mTrans.GetPos());
				}
			}
		}
		Timer::GetInstance().Update(Systems::PHYSICS);
	}
	/*!*************************************************************************
	This function ends the system. As there are no data members that require
	initialization, there are no data members that need to be un-initialised.
	Therefore this function is empty
	****************************************************************************/
	void PhysicsSystem::End() {}
}