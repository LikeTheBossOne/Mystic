#pragma once
#include <string>
#include <unordered_map>

#include "Buffer.h"
#include "VertexArray.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "Mystic/Core/Core.h"

namespace Mystic
{
	class Mesh
	{
	public:
		// Data could or could not include normals and/or UVs
		std::vector<float> RawData;
		std::vector<uint32_t> indices;

		bool HasUVs = false;
		bool HasNormals = false;

		const std::string& GetName() const { return _name; }
		const std::string& GetPath() const { return _path; }

	private:
		std::string _name;
		std::string _path;
	public:
		static Ref<Mesh> CreateMeshFromMystAsset(std::string assetPath);
		static Ref<Mesh> CreateMeshFromMystAsset(std::string name, std::string assetPath);
	};

	class MeshLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Mesh>& mesh);
		void Add(const Ref<Mesh>& mesh);
		Ref<Mesh> LoadFromMystAsset(const std::string& filepath);
		Ref<Mesh> LoadFromMystAsset(const std::string& name, const std::string& filepath);

		Ref<Mesh> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Mesh>> _meshes;
	};
}
