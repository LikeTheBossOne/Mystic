project "MysticHeaderTool"
	kind "ConsoleApp"
	language "C#"
	csversion "10.0"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.cs",
	}

	includedirs
	{
		"src",
	}

	excludes
	{
		"**/bin/**",
		"**/bin-int/**",
	}

	nuget
	{
		"System.Text.RegularExpressions:4.3.1"
	}

	links
	{
		"System.Text.RegularExpressions"
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