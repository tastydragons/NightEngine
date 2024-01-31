/*!*************************************************************************
****
\file IScript.h
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2400
\par Section: a
\par Assignment GAM200
\date 2/11/2022
\brief	Abstract class for IScript
Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/

#pragma once
#include "ExoEngine/ECS/Types.h"
#include "ExoEngine/ECS/ECS.h"

namespace EM
{
	/*!*************************************************************************
	Interface for Script
	When Adding a new script
	1) Ensure the Logic Component has the inserted script when deserializing
	2) Ensure the Level Editor has the new script that can be inserted
	****************************************************************************/
	class IScript
	{
	public:
		virtual ~IScript() = default;
		virtual void Start() {};
		virtual void Update(float Frametime) { UNREFERENCED_PARAMETER(Frametime); };
		virtual void End() {};
		virtual IScript* Clone() const = 0;
		virtual std::string GetScriptName() = 0;

		virtual void SetScriptEntityID(Entity& entity) = 0;
		virtual Entity & GetScriptEntityID() = 0;

		void SetScriptInit()
		{
			if (!mScriptInit)
			{
				mScriptInit = true;
			}
		}

		bool GetScriptInit()
		{
			return mScriptInit;
		}

		void TogglePauseScript()
		{
			mScriptPause == true ? false : true;
		}

		bool GetScriptPaused()
		{
			return mScriptPause;
		}

		void SetScriptPause(bool value)
		{
			mScriptPause = value;
		}

		bool mScriptPause{ false };

	protected:
		Entity entityID{};
		bool mScriptInit{ false };
	};
}