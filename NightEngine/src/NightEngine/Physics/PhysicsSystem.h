#pragma once

#include "NightEngine/Math/Physics.h"

#include "NightEngine/ECS/Components/Components.h"

#include "Platform/System/System.h"

namespace Night
{
	class PhysicsSystem : public System
	{
	public:
		/*!*************************************************************************
		This function returns the purpose of the system as a flag for when it is called
		****************************************************************************/
		virtual std::string GetName() { return "Update Positions"; }
		/*!*************************************************************************
		This function initialises the system. As there are no data members that require
		initialization, this function is empty
		****************************************************************************/
		virtual void Init() override;
		/*!*************************************************************************
		This function runs the logics of the system to update the entity positions
		by setting the current position as the entity's next position which has been
		calculated previously
		****************************************************************************/
		virtual void Update();
		/*!*************************************************************************
		This function ends the system. As there are no data members that require
		initialization, there are no data members that need to be un-initialised.
		Therefore this function is empty
		****************************************************************************/
		void End();
	private:
	};
}