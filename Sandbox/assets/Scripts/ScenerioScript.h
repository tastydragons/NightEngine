/*!*************************************************************************
****
\file ScenerioScript.h
\author Lau Yong Hui
\par DP email: l.yonghui.edu
\par Course: CSD2400
\par Section: a
\par Assignment GAM250
\date 12/01/2023
\brief	Script That act as the Game scene manager when the game is being played

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#pragma once
#include "empch.h"
#include "ExoEngine/Scripts/IScript.h"
#include "ExoEngine/ECS/Types.h"
#include "GLFW/glfw3.h"
#include "ExoEngine/ECS/ECS.h"
#include "ExoEngine/Input/Input.h"

namespace EM
{
	class ScenerioScript : public IScript
	{
	public:
		ScenerioScript();
		~ScenerioScript() = default;
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
		virtual IScript* Clone() const override;
		/*!*************************************************************************
		This function returns the purpose of the script as a flag for when it is called
		****************************************************************************/
		virtual std::string GetScriptName() override;

		virtual void SetScriptEntityID(Entity& entity) override { entityID = entity; }

		virtual Entity& GetScriptEntityID() override { return entityID; }

	private:
		/*!*************************************************************************
		This Variable originally hold the name of the selected string
		****************************************************************************/
		std::string SelectedScene;
	};
}