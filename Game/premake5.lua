project "Game"
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
		"%{wks.location}/Mystic/src",
		"%{wks.location}/Mystic/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.singleton_cpp}"
	}

	links
	{
		"Mystic",
		"ImGui",
		"singleton-cpp",
	}

	filter "system:windows"
		postbuildmessage "Copying singleton-cpp.dll to MysticEditor bin location"
		postbuildcommands
		{
			--"copy /b \"%{wks.location}bin\\" .. outputdir .. "\\Mystic\\Mystic.dll\" \"%{wks.location}bin\\" .. outputdir .. "\\MysticEditor\\Mystic.dll\"",
			"copy /b \"%{wks.location}Mystic\\vendor\\singleton-cpp\\bin\\" .. outputdir .. "\\singleton-cpp\\singleton-cpp.dll\" \"%{wks.location}bin\\" .. outputdir .. "\\Game\\singleton-cpp.dll\""
		}

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