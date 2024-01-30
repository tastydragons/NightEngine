#pragma once

#include "NightEngine/ECS/Types.h"

namespace Night
{
	class IStates;

	class StateMachine
	{
	public:
		StateMachine(Entity entity);

		~StateMachine() = default;

		void ChangeState(IStates* pNewState);

		void HandleInput(const int& key);

		const Entity& GetEntityID();

		void SetEntityID(Entity& entity) { mEntityID = entity; }

		void OnUpdate(float Frametime);

	private:
		IStates* mCurrState;
		Entity mEntityID;
	};
}