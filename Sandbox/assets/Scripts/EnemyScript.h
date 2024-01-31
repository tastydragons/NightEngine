/*!*************************************************************************
****
\file EnemyMovement.h
\author Tan Ek Hern / Elton Teo Zhe Wei / Matthew Cheung
\par DP email: t.ekhern@digipen.edu / e.teo@digipen.edu / j.cheung@digipen.edu
\par Course: CSD2400
\par Section: a
\par Assignment GAM200
\date 2/11/2022
\brief	Script for basic pathfinding for Enemy Movement will be expanded
for more complex pathfinding

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#pragma once
#include "IScript.h"
#include "ExoEngine/Input/Input.h"
#include "GLFW/glfw3.h"
#include "ExoEngine/Math/physics.h"
#include "Platform/Logic/StateMachines/StateMachine.h"
#include "Platform/Logic/States/IStates.h"
#include "Platform/Logic/States/EnemyStates/EnemyNotActive.h"

namespace EM
{

	class EnemyScript : public IScript
	{
	public:
		/*!*************************************************************************
		Default constructor for Enemy state machine
		****************************************************************************/
		EnemyScript(Entity entity) : mEnemyStateMachine{StateMachine(entity)} {}
		/*!*************************************************************************
		Default destructor for Enemy state machine
		****************************************************************************/
		~EnemyScript() = default;
		/*!*************************************************************************
		Copy constructor for Enemy state machine
		****************************************************************************/
		virtual IScript* Clone() const override
		{
			return new EnemyScript(*this);
		}
		/*!*************************************************************************
		Start state for Enemy state machine
		****************************************************************************/
		virtual void Start() override
		{
			mEnemyStateMachine.ChangeState(new EnemyNotActive(&mEnemyStateMachine));
		}
		/*!*************************************************************************
		Update state for Enemy state machine
		****************************************************************************/
		virtual void Update(float Frametime) override
		{
			mEnemyStateMachine.OnUpdate(Frametime);
		}
		/*!*************************************************************************
		End state for Enemy state machine
		****************************************************************************/
		virtual void End() override
		{
			delete this;
		}
		/*!*************************************************************************
		Get name of script for Enemy state machine
		****************************************************************************/
		virtual std::string GetScriptName() override
		{
			return "EnemyScript";
		}
		/*!*************************************************************************
		Sets the entity id for the entity holding this Enemy state machine
		****************************************************************************/
		virtual void SetScriptEntityID(Entity& entity) override { entityID = entity; }
		/*!*************************************************************************
		Gets the entity id for the entity holding this Enemy state machine
		****************************************************************************/
		virtual Entity& GetScriptEntityID() override { return entityID; }

	private:
		StateMachine mEnemyStateMachine;
	};
}