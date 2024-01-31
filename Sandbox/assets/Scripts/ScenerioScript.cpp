/*!*************************************************************************
****
\file ScenerioScript.cpp
\author Lau Yong Hui
\par DP email:l.yonghui.edu
\par Course: CSD2400
\par Section: a
\par Assignment GAM200
\date 12/01/2023
\brief	Script That act as the Game scene manager when the game is being played

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "empch.h"
#include "../ECS/SceneManager.h"
#include "ScenerioScript.h"


namespace EM {
	/*!*************************************************************************
	Default constructor for the scenario script
	****************************************************************************/
	ScenerioScript::ScenerioScript() : SelectedScene{""} {}
		/*!*************************************************************************
		Start state for the scenario script
		****************************************************************************/
		void ScenerioScript::Start(){}
		/*!*************************************************************************
		Update state for the scenario script
		****************************************************************************/
		void ScenerioScript::Update(float Frametime)
		{
			UNREFERENCED_PARAMETER(Frametime);
		}
		/*!*************************************************************************
		End state for the scenario script
		****************************************************************************/
		void ScenerioScript::End()
		{
			delete this;
		}

		/*!*************************************************************************
		Returns a clone of the scenario script
		****************************************************************************/
		IScript* ScenerioScript::Clone() const
		{
			return new ScenerioScript(*this);
		}
		/*!*************************************************************************
		Returns the name of the scenario script
		****************************************************************************/
		std::string ScenerioScript::GetScriptName()
		{
			return "ScenerioScript";
		}


}