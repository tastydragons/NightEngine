#pragma once

#include "NightEngine/Core/Core.h"

#include "NightEngine/ECS/Types.h"

namespace Night
{
	template class NIGHT_API std::set<Entity>;

	class NIGHT_API System
	{
	public:
		/*!*************************************************************************
		Container for all entities within this system
		****************************************************************************/
		std::set<Entity> mEntities;
		/*!*************************************************************************
		Returns name of System
		****************************************************************************/
		virtual std::string GetName() = 0;
		virtual ~System() {};
		//System function
		virtual void Init() = 0 {};
		virtual void Update(float Frametime) { UNREFERENCED_PARAMETER(Frametime); };
		virtual void End() = 0 {};

	protected:
		std::string m_DebugName;

	};
}