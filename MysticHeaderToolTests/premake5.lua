project "MysticHeaderToolTests"
	kind "SharedLib"
	language "C#"
	csversion "10.0"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"test/**.cs",
	}

	includedirs
	{
		"test",
	}

	excludes
	{
		"**/bin/**",
		"**/bin-int/**",
	}

	nuget
	{
		"Microsoft.NET.Test.Sdk:17.1.0",
		"MSTest.TestAdapter:2.2.8",
		"MSTest.TestFramework:2.2.8"
	}

	links
	{
		"MysticHeaderTool"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"MYST_PLATFORM_WINDOWS",
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