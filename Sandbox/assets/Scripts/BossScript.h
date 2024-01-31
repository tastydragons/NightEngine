/*!*************************************************************************
****
\file BossScript.h
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
#include "Platform/Logic/States/BossStates/BossNotActive.h"
namespace EM
{

	class BossScript : public IScript
	{
	public:
		/*!*************************************************************************
		Default constructor for Boss state machine
		****************************************************************************/
		BossScript(Entity entity) : mBossStateMachine{ StateMachine(entity) } {}
		/*!*************************************************************************
		Destructor for Boss state machine
		****************************************************************************/
		~BossScript() = default;
		/*!*************************************************************************
		Copy constructor for Boss state machine
		****************************************************************************/
		virtual IScript* Clone() const override
		{
			return new BossScript(*this);
		}
		/*!*************************************************************************
		Start state for Boss state machine
		****************************************************************************/
		virtual void Start() override
		{
			mBossStateMachine.ChangeState(new BossNotActive(&mBossStateMachine));
		}
		/*!*************************************************************************
		Update state for Boss state machine
		****************************************************************************/
		virtual void Update(float Frametime) override
		{
			mBossStateMachine.OnUpdate(Frametime);
		}
		/*!*************************************************************************
		End state for Boss state machine
		****************************************************************************/
		virtual void End() override
		{
			delete this;
		}
		/*!*************************************************************************
		Get name of script for Boss state machine
		****************************************************************************/
		virtual std::string GetScriptName() override
		{
			return "BossScript";
		}
		/*!*************************************************************************
		Sets the entity id for the entity holding this Boss state machine
		****************************************************************************/
		virtual void SetScriptEntityID(Entity& entity) override { entityID = entity; }
		/*!*************************************************************************
		Gets the entity id for the entity holding this Boss state machine
		****************************************************************************/
		virtual Entity& GetScriptEntityID() override { return entityID; }

	private:
		StateMachine mBossStateMachine;
	};
}