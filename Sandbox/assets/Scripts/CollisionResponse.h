/*!*****************************************************************************
\file CollisionResponse.h
\author Tan Ek Hern
\par DP email: t.ekhern@digipen.edu
\par Course: csd2125
\par Section: a
\par 
\date 27-11-2022
\brief  This file contains the script declaration for the collision system to
        calculate the appropriate counter-force on an object in the event of
		a detected collision
 
 Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#pragma once
#include "ExoEngine/Scripts/IScript.h"
#include "ExoEngine/ECS/Types.h"
#include "ExoEngine/ECS/ECS.h"
#include "ExoEngine/Math/Vmath.h"
#include "ExoEngine/Math/physics.h"

namespace EM
{
	class CollisionResponse : public IScript
	{
	public:
		CollisionResponse() : mTotalEnemies{ 0 }, mTotalDeadEnemies{ 0 } {}
		~CollisionResponse() = default;
		/*!*************************************************************************
		This function initialises the script. As there are no data members that require
		initialization, this function is empty
		****************************************************************************/
		virtual void Start() override;
		/*!*************************************************************************
		This function runs the logic of the script to check for collision and apply
        a counter-force to the entity's current velocity which is calculated based on
        the current velocity and the collision normal
		****************************************************************************/
		virtual void Update(float Frametime) override;
		/*!*************************************************************************
		This function ends the script by deleting the pointer to this script
		****************************************************************************/
		virtual void End() override;
		/*!*************************************************************************
		This function clones the script by allocating and constructing a copy of this 
		script and returning it
		****************************************************************************/
		virtual CollisionResponse* Clone() const override;
		/*!*************************************************************************
		This function returns the purpose of the script as a flag for when it is called
		****************************************************************************/
		virtual std::string GetScriptName() override;

		virtual void SetScriptEntityID(Entity& entity) override { entityID = entity; }

		virtual Entity& GetScriptEntityID() override { return entityID; }

	private:
		entityPhysics mPhys;
		Entity mEntityTarget{};
		Entity mEntityPlayer{};
		Entity mTotalEnemies;
		Entity mTotalDeadEnemies;
	};
}