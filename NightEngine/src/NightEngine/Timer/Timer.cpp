#include "nepch.h"

#include "NightEngine/Timer/Timer.h"

namespace Night
{
	/*!*************************************************************************
	Constructor for timer
	****************************************************************************/
	Timer::Timer()
	{
		GlobalTimeStarter();
		for (int i = 0; i < mStartTime.size(); i++)
			Start(static_cast<Systems>(i));

		for (int i = 0; i < mSystemDT.size(); i++)
			mSystemDT[i] = 0.016f;

		mGlobalDT = (float)(1.0f / 60.0f);
	}

	/*!*************************************************************************
	Start run timer
	****************************************************************************/
	void Timer::RunTimeStarter()
	{
		mRunTimeStart = clock();
	}

	/*!*************************************************************************
	Start global timer
	****************************************************************************/
	void Timer::GlobalTimeStarter()
	{
		mGlobalStart = clock();
	}

	/*!*************************************************************************
	Begin each system's start time
	****************************************************************************/
	void Timer::Start(Systems system)
	{
		if (mStartTime.size())
		{
			mStartTime[static_cast<int>(system)] = std::chrono::system_clock::now();
		}
	}

	/*!*************************************************************************
	Update timer
	****************************************************************************/
	void Timer::Update(Systems system)
	{
		mEnd = std::chrono::system_clock::now();
		mStart = GetStartTime(system);
		mDT = mEnd - mStart;
		SetDeltaTime(system, mDT.count() / 1000.f);
	}

	/*!*************************************************************************
	Set delta time
	****************************************************************************/
	void Timer::SetDeltaTime(Systems system, float deltatime)
	{
		if (mSystemDT.size())
			mSystemDT[static_cast<int>(system)] = deltatime;
	}

	/*!*************************************************************************
	Retrieve start time info
	****************************************************************************/
	std::chrono::system_clock::time_point Timer::GetStartTime(Systems system)
	{
		return mStartTime[static_cast<int>(system)];
	}

	/*!*************************************************************************
	Retrive time info
	****************************************************************************/
	time_t Timer::GetSystemTimeNow()
	{
		return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); // converts current time to seconds
	}

	/*!*************************************************************************
	Update global time
	****************************************************************************/
	void Timer::GlobalUpdate()
	{
		mTotalRuntime = (float)(clock() - mRunTimeStart) / 1000.0f;
		mGlobalDT = (float)(clock() - mGlobalStart);
		if (mGlobalDT > 0)
			mFps = 1000.0f / mGlobalDT;

		mGlobalDT = 1.0f / mFps;
	}
}