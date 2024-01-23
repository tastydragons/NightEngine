#pragma once

#ifdef NIGHT_PLATFORM_WINDOWS

extern Night::Application* Night::CreateApplication();

int main(int argc, char** argv)
{
	Night::Log::Init();
	NIGHT_CORE_WARN("Initialized Log!");
	int a = 5;
	NIGHT_INFO("Hello! Var={0}", a);

	auto app = Night::CreateApplication();
	app->Run();
	delete app;
}

#endif // NIGHT_PLATFORM_WINDOWS
