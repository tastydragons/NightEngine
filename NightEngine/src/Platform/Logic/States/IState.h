#pragma once

#include "hzpch.h"

#include "GLFW/glfw3.h"

#include "NightEngine/Core/Input.h"
#include "NightEngine/ECS/ECS.h"

#include "Platform/Logic/StateMachine/StateMachine.h"

namespace Night
{
	class StateMachine;

	class IStates
	{
	public:
		virtual ~IStates() = default;
		virtual IStates* HandleInput(StateMachine* stateMachine, const int& key) = 0;
		virtual void OnEnter(StateMachine* stateMachine) = 0;
		virtual void OnUpdate(StateMachine* stateMachine, float Frametime) = 0;
		virtual void OnExit(StateMachine* stateMachine) = 0;
	};
}
