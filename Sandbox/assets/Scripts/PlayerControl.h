/*!*************************************************************************
****
\file PlayerControl.h
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	This file contains the logic controller for players

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
#include "Platform/Logic/States/PlayerStates/OnIdle.h"
#include "Platform/Logic/States/PlayerStates/OnMove.h"
#include "Platform/Logic/States/PlayerStates/OnAttack_1.h"
#include "Platform/Logic/States/PlayerStates/OnAttack_2.h"


namespace EM
{

	class PlayerControl : public IScript
	{
	public:
		/*!*************************************************************************
		Default constructor for Player state machine
		****************************************************************************/
		PlayerControl(Entity entity) : mPlayerStateMachine{StateMachine(entity)} {}
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