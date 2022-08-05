include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Mystic"
	architecture "x64"
	startproject "MysticEditor"

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
	include "Mystic/vendor/glfw"
	include "Mystic/vendor/Glad"
	include "Mystic/vendor/imgui"
	include "Mystic/vendor/yaml-cpp"
	include "Mystic/vendor/OpenFBX"
group ""

include "Mystic"
include "Sandbox"
include "MysticEditor"
include "MysticHeaderTool"