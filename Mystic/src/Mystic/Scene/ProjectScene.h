//#pragma once
//
//#include "Scene.h"
//#include "../Core/Core.h"
//#include "Mystic/Render/Mesh.h"
//#include "Mystic/Render/Texture.h"
//
//namespace Mystic
//{
//	class AssetLibrary;
//	class EditorCamera;
//	class RuntimeScene;
//
//	class MYSTIC_API ProjectScene : public Scene
//	{
//	public:
//		ProjectScene();
//		~ProjectScene() override;
//
//		Ref<RuntimeScene> CreateRuntimeScene();
//
//	public:
//		void OnUpdate(float deltaTime);
//		void OnRender(EditorCamera& camera);
//
//		friend class ProjectSerializer;
//
//	private:
//		Ref<AssetLibrary> _assetLibrary;
//		Ref<Mesh> _mesh;
//		Ref<Texture2D> _texture;
//	};
//}
//
