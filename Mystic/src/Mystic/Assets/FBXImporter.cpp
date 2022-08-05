#include "FBXImporter.h"

#include <chrono>
#include <filesystem>

#include "mystpch.h"
#include <ios>

#include "AssetUtilities.h"
#include "Mystic/Core/Core.h"
#include "ofbx.h"

namespace Mystic
{
	// currently only imports meshes
	bool FBXImporter::Import(std::string fbxPath, std::unordered_map<FBXAssetType, std::unordered_map<std::string, std::string>>& outAssetMap)
	{

		const ofbx::IScene* scene = OpenFile(std::move(fbxPath));
		if (!scene)
			return false;

		const ofbx::Object* root = scene->getRoot();
		if (!root)
			return false;

		// Go through each child of root and create a mesh group from them if applicable
		outAssetMap[FBXAssetType::MESH] = std::unordered_map<std::string, std::string>();
		int idx = 0;
		while(ofbx::Object* child = root->resolveObjectLink(idx))
		{
			if (child->getType() == ofbx::Object::Type::MESH)
			{
				outAssetMap[FBXAssetType::MESH][child->name] = OutputMeshGroup(*static_cast<ofbx::Mesh*>(child));
			}
			idx++;
		}

		return true;
	}

	ofbx::IScene* FBXImporter::OpenFile(std::string filePath)
	{
		FILE* fp = fopen(filePath.c_str(), "rb");

		if (!fp)
		{
			return nullptr;
		}

		fseek(fp, 0, SEEK_END);
		long file_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		auto* content = new ofbx::u8[file_size];
		fread(content, 1, file_size, fp);
		ofbx::IScene* scene = ofbx::load((ofbx::u8*)content, file_size, (ofbx::u64)ofbx::LoadFlags::TRIANGULATE);

		return scene;
	}

	std::string FBXImporter::OutputMeshGroup(const ofbx::Mesh& mesh)
	{
		std::ofstream out;
		std::string fileName = AssetUtilities::g_baseAssetPath + "meshes/" + mesh.name + AssetUtilities::g_assetFileEnding;
		out.open(fileName);
		OutputMeshGroup(mesh, out);
		out.close();
		return fileName;
	}

	void FBXImporter::OutputMeshGroup(const ofbx::Mesh& mesh, std::ofstream& out)
	{
		// output this mesh
		OutputMesh(mesh, *mesh.getGeometry(), out);

		// recursively output children
		int idx = 0;
		while(ofbx::Object* child = mesh.resolveObjectLink(idx))
		{
			if (child->getType() == ofbx::Object::Type::MESH)
			{
				OutputMeshGroup(*static_cast<ofbx::Mesh*>(child), out);
			}
			idx++;
		}
	}

	void FBXImporter::OutputMesh(const ofbx::Mesh& mesh, const ofbx::Geometry& geo, std::ofstream& out)
	{

		// Vertices
		int vertexCount = geo.getVertexCount();
		const ofbx::Vec3* vertices = geo.getVertices();
		for (int i = 0; i < vertexCount; i++)
		{
			OutputVertex(vertices[i], out);
		}

		// Normals
		bool hasNormals = geo.getNormals() != nullptr;
		if (hasNormals)
		{
			const ofbx::Vec3* normals = geo.getNormals();
			int count = geo.getVertexCount();

			for (int j = 0; j < count; ++j)
			{
				OutputNormal(normals[j], out);
			}
		}

		bool hasUVs = geo.getUVs() != nullptr;
		if (hasUVs)
		{
			const ofbx::Vec2* uvs = geo.getUVs();
			int count = geo.getIndexCount();

			for (int j = 0; j < count; ++j)
			{
				OutputUV(uvs[j], out);
			}
		}

		const int* faceIndices = geo.getFaceIndices();
		int indexCount = geo.getIndexCount();
		bool newFace = true;
		for (int j = 0; j < indexCount; ++j)
		{
			if (newFace)
			{
				out << "f ";
				newFace = false;
			}
			int idx = (faceIndices[j] < 0) ? -faceIndices[j] : (faceIndices[j] + 1);
			out << idx;

			// If we have problems with normals/UVs, address them by re-adding the commented lines.
			if (hasUVs)
			{
				//out << '/' << j + 1;
			}
			else
			{
				//out << '/';
			}

			if (hasNormals)
			{
				//out << '/' << j + 1;
			}
			else
			{
				//out << '/';
			}

			newFace = faceIndices[j] < 0;
			out << (newFace ? '\n' : ' ');
		}
	}

	void FBXImporter::OutputMaterial(const ofbx::Material& material)
	{
	}

	void FBXImporter::OutputTexture(const ofbx::Texture& texture)
	{
		
	}

	void FBXImporter::OutputVertex(const ofbx::Vec3& vec, std::ofstream& out)
	{
		out << "v ";
		OutputVec3(vec, out);
		out << std::endl;
	}

	void FBXImporter::OutputFace(const uint32_t* indices, uint32_t size, std::ofstream& out)
	{
		
	}

	void FBXImporter::OutputNormal(const ofbx::Vec3& vec, std::ofstream& out)
	{
		out << "vn ";
		OutputVec3(vec, out);
		out << std::endl;
	}

	void FBXImporter::OutputUV(const ofbx::Vec2& vec, std::ofstream& out)
	{
		out << "vt ";
		OutputVec2(vec, out);
		out << std::endl;
	}

	void FBXImporter::OutputVec3(const ofbx::Vec3& vec, std::ofstream& out)
	{
		out << vec.x << " " << vec.y << " " << vec.z;
	}

	void FBXImporter::OutputVec2(const ofbx::Vec2& vec, std::ofstream& out)
	{
		out << vec.x << " " << vec.y;
	}

	void FBXImporter::OutputFloat(const float val, std::ofstream& out)
	{
		out << val;
	}

	void FBXImporter::OutputDouble(const double val, std::ofstream& out)
	{
		out << val;
	}
}
