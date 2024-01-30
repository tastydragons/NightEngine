#pragma once

#include "IScript.h"
#include "NightEngine/Core/Input.h"
#include "NightEngine/Math/Physics.h"
#include "NightEngine/Renderer/Camera2D.h"
#include "NightEngine/Renderer/Graphics.h"
#include "GLFW/glfw3.h"

namespace Night
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