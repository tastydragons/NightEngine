#pragma once

#include <memory>

#ifdef NIGHT_PLATFORM_WINDOWS
#if NIGHT_DYNAMIC_LINK
	#ifdef NIGHT_BUILD_DLL
		#define NIGHT_API __declspec(dllexport)
	#else
		#define NIGHT_API __declspec(dllimport)
	#endif
#else
	#define NIGHT_API
#endif
#else
	#error Night Engine only supports Windows!

#endif // NIGHT_PLATFORM_WINDOWS

#ifdef NIGHT_DEBUG
	#define NIGHT_ENABLE_ASSERTS
#endif

#ifdef NIGHT_ENABLE_ASSERTS
	#define NIGHT_ASSERT(x, ...) { if(!(x)) { NIGHT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define NIGHT_CORE_ASSERT(x, ...) { if(!(x)) { NIGHT_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define NIGHT_ASSERT(x, ...)
	#define NIGHT_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define NIGHT_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Night
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}