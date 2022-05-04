project "MysticEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"%{wks.location}/Mystic/vendor/spdlog/include",
		"%{wks.location}/Mystic/src",
		"%{wks.location}/Mystic/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"Mystic"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"MYST_PLATFORM_WINDOWS",
			"MYST_BUILD_DLL"
		}

	filter "configurations:Debug"
		defines "MYST_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "MYST_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "MYST_DIST"
		runtime "Release"
		optimize "On"