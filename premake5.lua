workspace "NightEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "NightEngine/vendor/GLFW/include"
IncludeDir["ImGui"] = "NightEngine/vendor/imgui"
IncludeDir["glm"] = "NightEngine/vendor/glm"
IncludeDir["stb_image"] = "NightEngine/vendor/stb_image"

include "NightEngine/vendor/GLFW"
include "NightEngine/vendor/imgui"

project "NightEngine"
	location "NightEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "nepch.h"
	pchsource "NightEngine/src/nepch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"NIGHT_PLATFORM_WINDOWS",
			"NIGHT_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "NIGHT_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "NIGHT_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "NIGHT_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING"
	}

	includedirs
	{
		"NightEngine/vendor/spdlog/include",
		"NightEngine/src",
		"NightEngine/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"NightEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"NIGHT_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "NIGHT_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "NIGHT_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "NIGHT_DIST"
		runtime "Release"
		optimize "on"
		