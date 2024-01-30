#pragma once

#include "IScript.h"
#include "NightEngine/Core/Input.h"
#include "GLFW/glfw3.h"
#include "NightEngine/Math/Physics.h"
#include "Platform/Logic/StateMachine/StateMachine.h"
#include "Platform/Logic/States/GateStates/GateUnlocked.h"
#include "Platform/Logic/States/GateStates/GateLocked.h"

namespace Night
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