#pragma once

#include "IScript.h"
#include "NightEngine/Core/Input.h"
#include "GLFW/glfw3.h"
#include "NightEngine/Math/Physics.h"
#include "Platform/Logic/StateMachine/StateMachine.h"
#include "Platform/Logic/States/HazardStates/HazardIdle.h"
#include "Platform/Logic/States/HazardStates/HazardCharging.h"
#include "Platform/Logic/States/HazardStates/HazardFire.h"

namespace Night
{
	/*!*************************************************************************
	Class for Laser Turret Script
	****************************************************************************/
	class HazardScript : public IScript
	{
	public:
		/*!*************************************************************************
		Default constructor for Laser Turret
		****************************************************************************/
		HazardScript(Entity entity) : mHazardStateMachine{ StateMachine(entity) } {};
		/*!*************************************************************************
		Default destructor for Player Controller
		****************************************************************************/
		~HazardScript() = default;
		/*!*************************************************************************
		Returns a new copy of HazardScript Script
		****************************************************************************/
		virtual HazardScript* Clone() const override
		{
			return new HazardScript(*this);
		}
		/*!*************************************************************************
		Start State of HazardScript Script
		****************************************************************************/
		virtual void Start() override
		{
			mHazardStateMachine.ChangeState(new HazardIdle(&mHazardStateMachine));
		}
		/*!*************************************************************************
		Update Loop of HazardScript Script
		****************************************************************************/
		virtual void Update(float Frametime) override
		{
			mHazardStateMachine.OnUpdate(Frametime);
		}
		/*!*************************************************************************
		End State for HazardScript
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
			return "HazardScript";
		}

		virtual void SetScriptEntityID(Entity& entity) override { entityID = entity; }

		virtual Entity& GetScriptEntityID() override { return entityID; }

	private:
		StateMachine mHazardStateMachine;
	};
}