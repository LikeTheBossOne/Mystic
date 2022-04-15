include "./vendor/premake/premake_customization/solution_items.lua"
#include "Dependencies.lua"

workspace "Mystic"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "Mystic/vendor/GLFW"
	include "Mystic/vendor/Glad"
	include "Mystic/vendor/imgui"
	include "Mystic/vendor/ryml"
group ""

#include "Mystic"
#include "Sandbox"


project "Mystic"
	location "Mystic"
	kind "SharedLibrary"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h"
		"%{prj.name}/src/**.cpp"
	}

	include
	{
		"vendor/entt/src"
		"vendor/ryml/src"
		"vendor/glm/src"
		"vendor/KHR/src"
		"vendor/GLFW/src"
		"vendor/glad/src"
	}

	filter "system:windows"
		cppdialect "C++latest"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"MYST_PLATFORM_WINDOWS"
			"MYST_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "MYST_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "MYST_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "MYST_DIST"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h"
		"%{prj.name}/src/**.cpp"
	}

	include
	{
		"vendor/entt/src"
		"Mystic/src"
	}

	links
	{
		"Mystic"
	}

	filter "system:windows"
		cppdialect "C++latest"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"MYST_PLATFORM_WINDOWS"
			"MYST_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "MYST_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "MYST_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "MYST_DIST"
		optimize "On"