#pragma once
#include "Mystic/Render/Mesh.h"
#include "Mystic/Render/Shader.h"
#include "Mystic/Render/Texture.h"

namespace Mystic
{
	/*
	 * Class used by editor and game runtime to store all the assets that a game would need.
	 * Should be deserialized by ProjectSerializer. At least for now, ProjectSerializer is actually a ProjectSerializer.
	 *
	 * Requires OpenGL to be initialized before starting
	 *
	 * TODO: Store assets in a format that is more memory-efficient
	 */
	class AssetLibrary
	{
	public:
		AssetLibrary();
		
		std::unordered_map<std::string, Ref<Mesh>> Meshes;
		std::unordered_map<std::string, Ref<Shader>> Shaders;
		std::unordered_map<std::string, Ref<Texture2D>> Textures;
	};
}
