/*!*************************************************************************
****
\file GateController.h
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2400
\par Section: a
\par Assignment GAM200
\date 2/11/2022
\brief	Script for Gate Controller

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
#include "Platform/Logic/States/GateStates/GateUnlocked.h"
#include "Platform/Logic/States/GateStates/GateLocked.h"

namespace EM
{
	/*!*************************************************************************
	Class for Gate Controller Script
	****************************************************************************/
	class GateController : public IScript
	{
	public:
		/*!*************************************************************************
		Default constructor for Gate Controller
		****************************************************************************/
		GateController(Entity entity) : mGateStateMachine{ StateMachine(entity) } {}
		/*!*************************************************************************
		Default destructor for Gate Controller
		****************************************************************************/
		~GateController() = default;
		/*!*************************************************************************
		Returns a new copy of GateController Script
		****************************************************************************/
		virtual GateController* Clone() const override
		{
			return new GateController(*this);
		}
		/*!*************************************************************************
		Start State of GateController Script
		****************************************************************************/
		virtual void Start() override
		{
			mGateStateMachine.ChangeState(new GateLocked(&mGateStateMachine));
		}
		/*!*************************************************************************
		Update Loop of GateController Script
		****************************************************************************/
		virtual void Update(float Frametime) override
		{
			mGateStateMachine.OnUpdate(Frametime);
		}
		/*!*************************************************************************
		End State for GateController
		****************************************************************************/
		virtual void End() override
		{
			delete this;
		}
		/*!*************************************************************************
		Returns the name of Script
		****************************************************************************/
		virtual std::string GetScriptName() override
		{
			return "GateController";
		}

		virtual void SetScriptEntityID(Entity& entity) override { entityID = entity; }

		virtual Entity& GetScriptEntityID() override { return entityID; }

	private:
		StateMachine mGateStateMachine;
	};
}