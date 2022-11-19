#include "Mesh.h"

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <utility>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "Mystic/Assets/FBXImporter.h"

namespace Mystic
{
	struct VertexFormatA
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 UV;
	};

	struct VertexFormatB
	{
		glm::vec3 Position;
		glm::vec2 UV;
	};

	struct IndexFormat
	{
		uint32_t Vertex = 0;
		uint32_t UV = 0;
		uint32_t Normal = 0;

		uint32_t& operator[](std::size_t idx)
		{
			Log::Assert(idx <= 2, "IndexFormat operator[] only supports args with values between 0 and 2");
			if (idx == 0)
			{
				return Vertex;
			}
			else if (idx == 1)
			{
				return UV;
			}
			else
			{
				return Normal;
			}
		}

		bool operator==(const IndexFormat& other) const
		{
			return Vertex == other.Vertex &&
				Normal == other.Normal &&
				UV == other.UV;
		}
	};
}

namespace std {

	template <>
	struct hash<Mystic::IndexFormat>
	{
		size_t operator()(const Mystic::IndexFormat& indexF) const
		{
			return hash<uint32_t>()(indexF.Vertex) ^ std::hash<uint32_t>()(indexF.UV) ^ std::hash<uint32_t>()(indexF.Normal);
		}
	};

}

namespace Mystic
{
	/*
	 * ==== MystAsset (.mysta) File Format ====
	 *
	 * Line1 (has normals) = 0/1
	 * Line2 (has UVs) = 0/1
	 * Lines+ (.obj vertices) = 'v 0.1 0.2 -0.5'
	 * Lines+ (.obj UVs)      = 'vt 0.500 1' //TODO-NOTE: 'w' is not supported at this time
	 * Lines+ (.obj normals)  = 'vn 0.1 0.2 -0.5'
	 * Lines+ (.obj faces - Vertex/UV/Normal indices)    = 'f 11 2 4' //TODO-NOTE: indices are one-indexed, faces must be ONLY TRIANGLES
	 *
	 *
	 * # is used for comments
	 */
	Ref<Mesh> Mesh::CreateMeshFromMystAsset(std::string assetPath)
	{
		std::string meshName = assetPath.substr(assetPath.size() - 6);
		return CreateMeshFromMystAsset(meshName, assetPath);
	}

	Ref<Mesh> Mesh::CreateMeshFromMystAsset(std::string name, std::string assetPath)
	{
		std::ifstream in;
		in.open(assetPath);
		Log::Assert(in.is_open(), "obj file was not opened");

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> UVs;
		std::vector<IndexFormat> indices;

		std::string line;
		while (std::getline(in, line))
		{
			if (line.starts_with("#"))
				continue;

			std::stringstream stream(line);
			// tokens
			std::string type;
			auto& b = std::getline(stream, type, ' ');
			Log::Assert(!!b, "Type Token missing from obj file ");

			if (type == "v")
			{
				uint8_t i = 0;
				glm::vec3 vec;

				std::string val;
				while (std::getline(stream, val, ' '))
				{
					Log::Assert(i < 3, "vertices only support three parameters (max)");

					vec[i] = std::stof(val);
					i++;
				}
				Log::Assert(i == 3, "vertices only support three parameters (min)");

				vertices.emplace_back(vec);
			}
			else if (type == "vn")
			{
				uint8_t i = 0;
				glm::vec3 vec;

				std::string val;
				while (std::getline(stream, val, ' '))
				{
					Log::Assert(i < 3, "normals only support three parameters (max)");

					vec[i] = std::stof(val);
					i++;
				}
				Log::Assert(i == 3, "normals only support two parameters (min)");

				normals.emplace_back(vec);
			}
			else if (type == "vt")
			{
				uint8_t i = 0;
				glm::vec2 vec;

				std::string val;
				while (std::getline(stream, val, ' '))
				{
					Log::Assert(i < 2, "UVs only support two parameters (max)");

					vec[i] = std::stof(val);
					i++;
				}
				Log::Assert(i == 2, "UVs only support two parameters (min)");

				UVs.emplace_back(vec);
			}
			else if (type == "f")
			{
				std::string indexGroup;
				while (std::getline(stream, indexGroup, ' '))
				{
					IndexFormat index;

					std::stringstream indexStream(indexGroup);

					uint8_t idx = 0;
					std::string val;
					while (std::getline(indexStream, val, '/'))
					{
						if (!val.empty())
						{
							index[idx] = std::stoul(val) - 1;
						}

						idx++;
					}

					indices.emplace_back(index);
				}
			}
			else
			{
				Log::Assert(false, "Unknown type");
			}
		}

		
		Ref<Mesh> mesh = std::make_shared<Mesh>();
		mesh->_name = name;
		mesh->_path = assetPath;
		mesh->HasUVs = true;

		std::vector<float>& vertexBuffer = mesh->RawData;
		std::vector<uint32_t>& indexBuffer = mesh->indices;
		bool& useNormals = mesh->HasNormals;

		if (indices[0].Normal == 0)
		{
			useNormals = false;
		}

		// Temp solution to A vs B dilemna
		std::unordered_map<IndexFormat, uint32_t> indexToVertexOffset;
		if (useNormals)
		{
			// Get unique combos and create new offsets into index buffer.
			for (uint32_t i = 0; i < indices.size(); i++)
			{
				IndexFormat& index = indices[i];

				// If unique combo does not exist, add it and note the offset into the vertex buffer
				// Either way, add offset into vertex buffer to indexBuffer.
				if (!indexToVertexOffset.contains(index))
				{
					indexToVertexOffset.emplace(index, vertexBuffer.size() / 8);

					indexBuffer.emplace_back(vertexBuffer.size() / 8);

					glm::vec3& pos = vertices[index.Vertex];
					glm::vec3& norm = normals[index.Normal];
					glm::vec2& UV = UVs[index.UV];
					vertexBuffer.emplace_back(pos[0]);
					vertexBuffer.emplace_back(pos[1]);
					vertexBuffer.emplace_back(pos[2]);
					vertexBuffer.emplace_back(norm[0]);
					vertexBuffer.emplace_back(norm[1]);
					vertexBuffer.emplace_back(norm[2]);
					vertexBuffer.emplace_back(UV[0]);
					vertexBuffer.emplace_back(UV[1]);
				}
				else
				{
					indexBuffer.emplace_back(indexToVertexOffset[index]);
				}
			}
		}
		else
		{
			// Get unique combos and create new offsets into index buffer.
			for (uint32_t i = 0; i < indices.size(); i++)
			{
				IndexFormat& index = indices[i];

				// If unique combo does not exist, add it and note the offset into the vertex buffer
				// Either way, add offset into vertex buffer to indexBuffer.
				if (!indexToVertexOffset.contains(index))
				{
					indexToVertexOffset.emplace(index, vertexBuffer.size() / 5);

					indexBuffer.emplace_back(vertexBuffer.size() / 5);

					glm::vec3& pos = vertices[index.Vertex];
					glm::vec2& UV = UVs[index.UV];
					vertexBuffer.emplace_back(pos[0]);
					vertexBuffer.emplace_back(pos[1]);
					vertexBuffer.emplace_back(pos[2]);
					vertexBuffer.emplace_back(UV[0]);
					vertexBuffer.emplace_back(UV[1]);
				}
				else
				{
					indexBuffer.emplace_back(indexToVertexOffset[index]);
				}
			}
		}

		return mesh;
	}

	void MeshLibrary::Add(const std::string& name, const Ref<Mesh>& mesh)
	{
		Log::Assert(!Exists(name), "Mesh already exists!");
		_meshes[name] = mesh;
	}

	void MeshLibrary::Add(const Ref<Mesh>& mesh)
	{
		auto& name = mesh->GetName();
		Add(name, mesh);
	}

	Ref<Mesh> MeshLibrary::LoadFromMystAsset(const std::string& filepath)
	{
		Ref<Mesh> mesh = Mesh::CreateMeshFromMystAsset(filepath);
		Add(mesh);
		return mesh;
	}

	Ref<Mesh> MeshLibrary::LoadFromMystAsset(const std::string& name, const std::string& filepath)
	{
		Ref<Mesh> mesh = Mesh::CreateMeshFromMystAsset(filepath);
		Add(name, mesh);
		return mesh;
	}

	Ref<Mesh> MeshLibrary::Get(const std::string& name)
	{
		Log::Assert(Exists(name), "Mesh not found!");
		return _meshes[name];
	}

	bool MeshLibrary::Exists(const std::string& name) const
	{
		return _meshes.contains(name);
	}
}
