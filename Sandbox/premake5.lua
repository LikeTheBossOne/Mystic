project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"%{wks.location}/Mystic/src",
		"%{wks.location}/Mystic/vendor",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
	}

	links
	{
		"Mystic",
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib"
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