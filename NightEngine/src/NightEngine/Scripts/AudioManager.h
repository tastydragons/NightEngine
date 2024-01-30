#pragma once

#include "IScript.h"

#include "GLFW/glfw3.h"

#include "NightEngine/Core/Input.h"
#include "NightEngine/Math/Physics.h"

namespace Night
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