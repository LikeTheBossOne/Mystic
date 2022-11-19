#pragma once
#include <string>
#include <unordered_map>

namespace ofbx
{
	struct Vec2;
	struct Vec3;
	struct Texture;
	struct Material;
	struct Geometry;
	struct Mesh;
	struct IScene;
}

namespace Mystic
{
	enum class FBXAssetType
	{
		
		ROOT,
		GEOMETRY,
		SHAPE,
		MATERIAL,
		MESH,
		TEXTURE,
		LIMB_NODE,
		NULL_NODE,
		NODE_ATTRIBUTE,
		CLUSTER,
		SKIN,
		BLEND_SHAPE,
		BLEND_SHAPE_CHANNEL,
		ANIMATION_STACK,
		ANIMATION_LAYER,
		ANIMATION_CURVE,
		ANIMATION_CURVE_NODE,
		POSE
	};
	
	class FBXImporter
	{
	public:
		// Map of types to map of names to subPaths
		static bool Import(std::string fbxPath, std::unordered_map<FBXAssetType, std::unordered_map<std::string, std::string>>& outAssetMap);

	private:
		static ofbx::IScene* OpenFile(std::string filePath);

		static std::string OutputMeshGroup(const ofbx::Mesh& mesh);
		static void OutputMeshGroup(const ofbx::Mesh& mesh, std::ofstream& out);

		// Outputs mesh into .obj file
		static void OutputMesh(const ofbx::Mesh& mesh, const ofbx::Geometry& geo, std::ofstream& out);
		static void OutputMaterial(const ofbx::Material& material);
		static void OutputTexture(const ofbx::Texture& texture);

		static void OutputVertex(const ofbx::Vec3& vec, std::ofstream& out);
		static void OutputFace(const uint32_t* indices, uint32_t size, std::ofstream& out);
		static void OutputNormal(const ofbx::Vec3& vec, std::ofstream& out);
		static void OutputUV(const ofbx::Vec2& vec, std::ofstream& out);


		static void OutputVec3(const ofbx::Vec3& vec, std::ofstream& out);
		static void OutputVec2(const ofbx::Vec2& vec, std::ofstream& out);
		static void OutputFloat(const float val, std::ofstream& out);
		static void OutputDouble(const double val, std::ofstream& out);
	};
}
