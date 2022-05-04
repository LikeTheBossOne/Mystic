#include "ProjectScene.h"

#include <fstream>
#include <sstream>

#include "RuntimeScene.h"
#include "Mystic/ECS/Components/GUIDComponent.h"
#include "Mystic/GFX/Camera.h"
#include "Mystic/GFX/Renderer.h"
#include "Mystic/ECS/ComponentRegistry.h"
#include "Mystic/ECS/Components/TransformComponent.h"
#include "Mystic/GFX/Renderer2D.h"

namespace Mystic
{

	ProjectScene::ProjectScene()
	{
		_registry = entt::registry();
		_mainCamera = std::make_shared<Camera>();
	}

	ProjectScene::~ProjectScene()
	{
	}

	Ref<RuntimeScene> ProjectScene::CreateRuntimeScene()
	{
		Ref<RuntimeScene> runtimeScene = std::make_shared<RuntimeScene>();

		runtimeScene->_name = _name;

		std::unordered_map<GUID, entt::entity> uuidToEntMap;

		for (const auto ent : _registry.view<GUIDComponent>())
		{
			GUID guid = _registry.get<GUIDComponent>(ent).GUID;

			entt::entity toEnt = runtimeScene->_registry.create();
			uuidToEntMap[guid] = toEnt;
		}

		ComponentRegistry::InvokeCopy(runtimeScene->_registry, _registry, uuidToEntMap);

		return runtimeScene;
	}

	void ProjectScene::OnUpdate(EditorCamera& camera)
	{
		/*Renderer3D::UseShaderProgram();

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 100.f);
		Renderer3D::SetProjectionMatrix(projection);

		glm::mat4 viewMat = _mainCamera->GetViewMatrix();
		Renderer3D::SetViewMatrix(viewMat);

		auto group = _registry->group<Renderable>(entt::get<TransformComponent>);
		group.each([](const Renderable& renderable, const TransformComponent& transform)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, transform.Position);
			glm::mat4 rotMatrix = glm::toMat4(transform.Rotation);
			model = model * rotMatrix;
			model = glm::scale(model, transform.Scale);

			std::string key = renderable.MeshKey;
			Renderer3D::RenderEnt(key, model);
		});*/

		Renderer2D::BeginScene(camera);

		auto group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		Renderer2D::EndScene();
	}
}
