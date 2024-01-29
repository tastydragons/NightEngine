#pragma once

#include <GLFW/glfw3.h>
#include "NightEngine/Core/Core.h"

namespace Night
{
	class NIGHT_API FPS
	{
	public:

		/*!*************************************************************************
		Init various frame counters
		****************************************************************************/
		void InitFrame()
		{
			mCounter = 0.0f;
			mMaxFrameRate = 60.0f;
			mFrameRate = mMaxFrameRate;
			mFrameTime = 1 / mFrameRate;
			mMinFrameTime = 1 / mMaxFrameRate;
		}

		/*!*************************************************************************
		Start frame count using glfwGetTime()
		****************************************************************************/
		void StartFrameCount() { mStart = (float)glfwGetTime(); }

		/*!*************************************************************************
		End frame count
		****************************************************************************/
		float EndFrameCount()
		{
			while (((float)glfwGetTime() - mStart) < mMinFrameTime)
			{
				float end = (float)glfwGetTime();
				mFrameTime = end - mStart;
				mFps = 1.0f / mFrameTime;
			}

			return mFps;
		}

		/*!*************************************************************************
		Return fps
		****************************************************************************/
		float GetFps()const { return mFps; }
		static FPS& GetInstance()
		{
			static FPS instance;
			return instance;
		}
	private:
		float mCounter, mMaxFrameRate, mFrameRate, mFrameTime,
			mMinFrameTime, mStart, mFps;
	};
}