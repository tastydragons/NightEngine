#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Night
{
	class NIGHT_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		static std::string GetImguiLog();

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::ostringstream oss;
	};

}

// Core log macros
#define NIGHT_CORE_TRACE(...)    ::Night::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define NIGHT_CORE_INFO(...)     ::Night::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NIGHT_CORE_WARN(...)     ::Night::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NIGHT_CORE_ERROR(...)    ::Night::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NIGHT_CORE_CRITICAL(...) ::Night::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define NIGHT_TRACE(...)     ::Night::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NIGHT_INFO(...)      ::Night::Log::GetClientLogger()->info(__VA_ARGS__)
#define NIGHT_WARN(...)      ::Night::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NIGHT_ERROR(...)     ::Night::Log::GetClientLogger()->error(__VA_ARGS__)
#define NIGHT_CRITICAL(...)  ::Night::Log::GetClientLogger()->critical(__VA_ARGS__)