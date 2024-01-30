#include "hzpch.h"

#include "StateMachine.h"

#include "Platform/Logic/States/IState.h"

namespace Night
{
	StateMachine::StateMachine(Entity entity) : mCurrState{}, mEntityID{ entity } {}

	void StateMachine::ChangeState(IStates* pNewState)
	{
		if (mCurrState != nullptr)
		{
			mCurrState->OnExit(this);
		}
		mCurrState = pNewState;
		mCurrState->OnEnter(this);
	}

	void StateMachine::HandleInput(const int& key)
	{
		IStates* state = mCurrState->HandleInput(this, key);
		if (state != nullptr)
		{
			ChangeState(state);
		}

	}

	const Entity& StateMachine::GetEntityID()
	{
		return mEntityID;
	}

	void StateMachine::OnUpdate(float Frametime)
	{
		mCurrState->OnUpdate(this, Frametime);
	}
}