#include "ProjectScene.h"

#include <fstream>
#include <sstream>

#include "RuntimeScene.h"
#include "../ECS/Components/GUIDComponent.h"
#include "../GFX/Camera.h"
#include "../GFX/Renderer3D.h"
#include "../ECS/ComponentRegistry.h"

namespace Mystic
{

	ProjectScene::ProjectScene()
	{
		_registry = std::make_shared<entt::registry>();
		_activeCamera = std::make_shared<Camera>();
	}

	ProjectScene::~ProjectScene()
	{
	}

	Ref<RuntimeScene> ProjectScene::CreateRuntimeScene()
	{
		Ref<RuntimeScene> runtimeScene = std::make_shared<RuntimeScene>();

		runtimeScene->_name = _name;

		const Ref<entt::registry> to = runtimeScene->_registry;
		const Ref<entt::registry> from = _registry;

		std::unordered_map<GUID, entt::entity> uuidToEntMap;

		for (const auto ent : from->view<GUIDComponent>())
		{
			GUID guid = from->get<GUIDComponent>(ent).GUID;

			entt::entity toEnt = to->create();
			uuidToEntMap[guid] = toEnt;
		}

		ComponentRegistry::InvokeCopy(*to, *from, uuidToEntMap);

		return runtimeScene;
	}

	void ProjectScene::RenderScene()
	{
		Renderer3D::UseShaderProgram();

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 100.f);
		Renderer3D::SetProjectionMatrix(projection);

		glm::mat4 viewMat = _activeCamera->GetViewMatrix();
		Renderer3D::SetViewMatrix(viewMat);

		auto group = _registry->group<Renderable>(entt::get<Transform>);
		group.each([](const Renderable& renderable, const Transform& transform)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, transform.Position);
			glm::mat4 rotMatrix = glm::toMat4(transform.Rotation);
			model = model * rotMatrix;
			model = glm::scale(model, transform.Scale);

			std::string key = renderable.MeshKey;
			Renderer3D::RenderEnt(key, model);
		});
	}
}
