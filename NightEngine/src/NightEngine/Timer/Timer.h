#pragma once

namespace Night
{
#define TotalSystems 6

	enum class Systems
	{
		NONE = 0,
		COLLISION,
		GRAPHIC,
		PHYSICS,
		LOGIC,
		API
	};

	class Timer
	{
	public:
		Timer();
		~Timer() = default;

		//for global calculations
		void RunTimeStarter();
		void GlobalTimeStarter();
		void GlobalUpdate();

		// returns global delta time
		float GetGlobalDT() const { return mGlobalDT; }
		float GetDT(Systems system) { return mSystemDT[static_cast<int>(system)]; }
		float GetFps() const { return mFps; }

		//Timer for system runtime
		void Start(Systems system);
		void Update(Systems system);
		void SetDeltaTime(Systems system, float deltatime);

		std::chrono::system_clock::time_point GetStartTime(Systems system);

		static Timer& GetInstance()
		{
			static Timer instance;
			return instance;
		}

	public:
		std::array<std::chrono::system_clock::time_point, TotalSystems> mStartTime;
		std::array<float, TotalSystems> mSystemDT;

	private:
		std::chrono::system_clock::time_point mStart, mEnd;
		std::chrono::duration<float, std::milli> mDT;
		clock_t mGlobalStart, mRunTimeStart;
		float mGlobalDT, mTotalRuntime, mFps, mFloatDeltaTime;

	private:
		time_t GetSystemTimeNow();
	};
}