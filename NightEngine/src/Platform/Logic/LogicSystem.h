#pragma once

#include "Platform/System/System.h"

#include "NightEngine/ECS/Components/Components.h"

#include "NightEngine/ECS/ECS.h"

namespace Night
{
	class LogicSystem : public System
	{
	public:
		/*!*************************************************************************
		Returns string name of system
		****************************************************************************/
		virtual std::string GetName() { return "LogicSystem"; }
		/*!*************************************************************************
		Initialize Logic System
		****************************************************************************/
		virtual void Init() override;
		/*!*************************************************************************
		Update Loop for Logic System
		****************************************************************************/
		virtual void Update(float Frametime) override;
		/*!*************************************************************************
		End State for Logic System which will delete all script data
		****************************************************************************/
		virtual void End() override;
	};
}