#pragma once

#include "NightEngine/Scripts/IScript.h"

#include "NightEngine/Core/Input.h"
#include "NightEngine/Math/NEMath.h"

#include "NightEngine/ECS/Types.h"
#include "NightEngine/ECS/ECS.h"

#include "Platform/Windows/Window.h"
#include "Platform/Logic/States/IState.h"

namespace Night
{
	class ButtonResponse : public IScript
	{
	public:
		ButtonResponse() : clicked{ false }, selected{ false }, initial_hover{ true }, how_to_play{ false } {};
		~ButtonResponse() = default;
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
		virtual ButtonResponse* Clone() const override;
		/*!*************************************************************************
		This function returns the purpose of the script as a flag for when it is called
		****************************************************************************/
		virtual std::string GetScriptName() override;
		/*!*************************************************************************
		This function check if the mouse is within the bounding box of the button
		****************************************************************************/
		bool is_within_box(glm::vec2 cur, Collider box, Transform trans);

		virtual void SetScriptEntityID(Entity& entity) override { entityID = entity; }

		virtual Entity& GetScriptEntityID() override { return entityID; }

		bool clicked = false;
		bool selected = false;
		bool Action = false;

		enum class button_state
		{
			idle,
			select,
			click,
			release,
		};

	private:
		struct button_bb
		{
			vec2D min;
			vec2D max;
		};
		button_bb Button_BoundingBox{};
		button_state curr_state{ button_state::idle };
		bool HTP{ false };
		bool initial_hover;
		bool how_to_play;

	};
}