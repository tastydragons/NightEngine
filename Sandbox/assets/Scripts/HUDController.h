/*!*************************************************************************
****
\file HUDController.h
\author Tan Ek Hern
\par DP email: t.ekhern@digipen.edu
\par Course: CSD2400
\par Section: a
\par Assignment GAM200
\date 28/01/2023
\brief	Script for HUD Controller

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#pragma once
#include "IScript.h"
#include "ExoEngine/Input/Input.h"
#include "ExoEngine/Math/physics.h"
#include "Platform/Graphics/Camera2D.h"
#include "Platform/Graphics/Graphics.h"
#include "GLFW/glfw3.h"

namespace EM
{
	/*!*************************************************************************
	Class for HUD Controller Script
	****************************************************************************/
	class HUDController : public IScript
	{
	public:
		/*!*************************************************************************
		Default constructor for HUD Controller
		****************************************************************************/
		HUDController();
		/*!*************************************************************************
		Default destructor for HUD Controller
		****************************************************************************/
		~HUDController() = default;
		/*!*************************************************************************
		Returns a new copy of HUDController Script
		****************************************************************************/
		virtual HUDController* Clone() const override;
		/*!*************************************************************************
		Start State of HUDController Script
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
		virtual std::string GetScriptName() override;

		virtual void SetScriptEntityID(Entity& entity) override { entityID = entity; }

		virtual Entity& GetScriptEntityID() override { return entityID; }
	private:
		Entity mEntityID;
	};
}