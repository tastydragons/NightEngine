/*!*************************************************************************
****
\file AudioManager.h
\author Tan Ek Hern
\par DP email: t.ekhern@digipen.edu
\par Course: CSD2400
\par Section: a
\par Assignment GAM200
\date 28/01/2023
\brief	Script for Audio Manager

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#pragma once
#include "IScript.h"
#include "GLFW/glfw3.h"
#include "ExoEngine/Input/Input.h"
#include "ExoEngine/Math/physics.h"

namespace EM
{
	/*!*************************************************************************
	Class for Audio Manager Script
	****************************************************************************/
	class AudioManager : public IScript
	{
	public:
		/*!*************************************************************************
		Default constructor for Audio Manager
		****************************************************************************/
		AudioManager();
		/*!*************************************************************************
		Default destructor for HUD Controller
		****************************************************************************/
		~AudioManager() = default;
		/*!*************************************************************************
		Returns a new copy of Audio Manager Script
		****************************************************************************/
		virtual AudioManager* Clone() const override;
		/*!*************************************************************************
		Start State of Audio Manager Script
		****************************************************************************/
		virtual void Start() override;
		/*!*************************************************************************
		Update Loop of HUD Controller Script
		****************************************************************************/
		virtual void Update(float Frametime) override;
		/*!*************************************************************************
		End State for HUD Controller
		****************************************************************************/
		virtual void End() override;
		/*!*************************************************************************
		Returns the name of Script
		****************************************************************************/
		virtual std::string GetScriptName();

		virtual void SetScriptEntityID(Entity& entity) override { entityID = entity; }

		virtual Entity& GetScriptEntityID() override { return entityID; }
		
	private:
		float BGMvol;
		float SFXvol;
		float Mastervol;
	};
}