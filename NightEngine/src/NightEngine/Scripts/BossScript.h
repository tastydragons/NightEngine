#pragma once

#include "IScript.h"

#include "GLFW/glfw3.h"

#include "NightEngine/Core/Input.h"
#include "NightEngine/Math/Physics.h"

#include "Platform/Logic/StateMachine/StateMachine.h"
#include "Platform/Logic/States/IState.h"

#include "Platform/Logic/States/BossStates/BossNotActive.h"

namespace Night
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