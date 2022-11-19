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
		"%{wks.location}/Intermediate/Generated_H"
	}

	links
	{
		"Mystic",
		"ImGui",
		"yaml-cpp"
	}

	prebuildcommands
	{
		"\"%{wks.location}bin\\" .. outputdir .. "\\MysticHeaderTool\\MysticHeaderTool.exe\"" ..
		" \"%{wks.location}GameCode\\src\\Components\"" ..
		" \"%{wks.location:sub(1, -2)}\"" ..
		" \"%{wks.location}GameCode\\src\\Generated\"" ..
		" \"18/08/2015-06:30:15.006542\""

	}

	postbuildmessage "Copying DLL to MysticEditor bin location"
	postbuildcommands
	{
		"{COPYFILE} \"%{wks.location}bin\\" .. outputdir .. "\\GameCode\\GameCode.dll\" \"%{wks.location}bin\\" .. outputdir .. "\\Game\\GameCode.dll\"",
		"{COPYFILE} \"%{wks.location}bin\\" .. outputdir .. "\\GameCode\\GameCode.dll\" \"%{wks.location}bin\\" .. outputdir .. "\\MysticEditor\\GameCode.dll\"",
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