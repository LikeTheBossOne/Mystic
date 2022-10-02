project "GameCode"
	kind "SharedLib"
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
		"src",
		"%{wks.location}/Mystic/src",
		"%{wks.location}/Mystic/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
	}

	links
	{
		"Mystic",
		"ImGui",
		"yaml-cpp"
	}

	postbuildmessage "Copying DLL to MysticEditor bin location"
	postbuildcommands
	{
		"copy /b \"%{wks.location}bin\\" .. outputdir .. "\\GameCode\\GameCode.dll\" \"%{wks.location}bin\\" .. outputdir .. "\\MysticEditor\\GameCode.dll\"",
		"copy /b \"%{wks.location}bin\\" .. outputdir .. "\\GameCode\\GameCode.dll\" \"%{wks.location}bin\\" .. outputdir .. "\\Game\\GameCode.dll\"",
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