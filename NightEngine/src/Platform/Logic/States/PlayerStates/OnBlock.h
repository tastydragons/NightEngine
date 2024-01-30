/*!*************************************************************************
****
\file OnBlock.h
\author Cheung Jun Yin Matthew
\par DP email: j.cheung@digipen.edu
\par Course: CSD2450
\par Section: a
\par Assignment GAM200
\date 24/2/2022
\brief	Blocking state for player

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#pragma once
#include "Platform/Logic/States/IState.h"

namespace Night
{
	class OnBlock : public IStates
	{
	public:
		OnBlock(StateMachine* stateMachine);
		virtual ~OnBlock() = default;
		virtual IStates* HandleInput(StateMachine* stateMachine, const int& key) override;
		virtual void OnEnter(StateMachine* stateMachine) override;
		virtual void OnUpdate(StateMachine* stateMachine, float Frametime) override;
		virtual void OnExit(StateMachine* stateMachine) override;
	private:
		float mTimer;
		float mDuration;
		float mCamMinX;
		float mCamMaxX;
		float mCamMinY;
		float mCamMaxY;
		float EaseInOutSine(float start, float end, float value)
		{
			end -= start;
			return (float)-end * 0.5f * (float)(cosf((float)M_PI * value) - 1.0f) + start;
		}
	};
}