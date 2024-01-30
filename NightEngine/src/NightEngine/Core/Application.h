#pragma once

#include "Core.h"
#include "Log.h"
#include "Platform/System/SystemManager.h"
#include "NightEngine/ECS/ECS.h"

namespace Night
{
	class Application
	{
	public:
		enum class state
		{
			pause,
			play,
			quit
		};

		Application();
		virtual ~Application(); // will be override
		void SystemInput(System* system);
		void Run();				//run the application 
		void End();

	private:
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}

