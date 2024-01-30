#pragma once

#include "IScript.h"
#include "NightEngine/Core/Input.h"
#include "GLFW/glfw3.h"
#include "NightEngine/Math/Physics.h"
#include "Platform/Logic/StateMachine/StateMachine.h"
#include "Platform/Logic/States/IState.h"
#include "Platform/Logic/States/PlayerStates/OnIdle.h"
#include "Platform/Logic/States/PlayerStates/OnMove.h"
#include "Platform/Logic/States/PlayerStates/OnAttack_1.h"
#include "Platform/Logic/States/PlayerStates/OnAttack_2.h"

namespace Night
{
	class PlayerControl : public IScript
	{
	public:
		/*!*************************************************************************
		Default constructor for Player state machine
		****************************************************************************/
		PlayerControl(Entity entity) : mPlayerStateMachine{ StateMachine(entity) } {}
		/*!*************************************************************************
		Default destructor for Player state machine
		****************************************************************************/
		~PlayerControl() = default;
		/*!*************************************************************************
		Copy constructor Player state machine
		****************************************************************************/
		virtual IScript* Clone() const override
		{
			return new PlayerControl(*this);
		}
		/*!*************************************************************************
		Start state for Player state machine
		****************************************************************************/
		virtual void Start() override
		{
			mPlayerStateMachine.ChangeState(new OnIdle(&mPlayerStateMachine));
		}
		/*!*************************************************************************
		Update state for Player state machine
		****************************************************************************/
		virtual void Update(float Frametime) override
		{
			for (auto key : p_Input->mKeyStatus)
			{
				mPlayerStateMachine.HandleInput(key.first);
			}
			for (auto key : p_Input->mMouseStatus)
			{
				mPlayerStateMachine.HandleInput(key.first);
			}
			mPlayerStateMachine.OnUpdate(Frametime);
		}
		/*!*************************************************************************
		End state constructor for Player state machine
		****************************************************************************/
		virtual void End() override
		{
			delete this;
		}
		/*!*************************************************************************
		Return script name of Player state machine
		****************************************************************************/
		virtual std::string GetScriptName() override
		{
			return "PlayerControl";
		}
		/*!*************************************************************************
		Sets the entity id for the entity holding this Player state machine
		****************************************************************************/
		virtual void SetScriptEntityID(Entity& entity) override { entityID = entity; }
		/*!*************************************************************************
		Gets the entity id for the entity holding this Player state machine
		****************************************************************************/
		virtual Entity& GetScriptEntityID() override { return entityID; }

	private:
		StateMachine mPlayerStateMachine;
	};
}