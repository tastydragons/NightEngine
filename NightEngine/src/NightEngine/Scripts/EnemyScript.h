#pragma once

#include "IScript.h"
#include "NightEngine/Core/Input.h"
#include "GLFW/glfw3.h"
#include "NightEngine/Math/Physics.h"
#include "Platform/Logic/StateMachine/StateMachine.h"
#include "Platform/Logic/States/IState.h"
#include "Platform/Logic/States/EnemyStates/EnemyNotActive.h"

namespace Night
{
	class EnemyScript : public IScript
	{
	public:
		/*!*************************************************************************
		Default constructor for Enemy state machine
		****************************************************************************/
		EnemyScript(Entity entity) : mEnemyStateMachine{ StateMachine(entity) } {}
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