#include "Scene.h"

#include "Components/Renderable.h"
#include "Components/Transform.h"
#include "GFX/Camera.h"
#include "GFX/Renderer2D.h"

Mystic::Scene::Scene()
{
	_registry = std::make_shared<entt::registry>();
	_activeCamera = std::make_shared<Camera>();
}

void Mystic::Scene::RenderScene()
{
	Renderer2D::UseShaderProgram();

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 100.f);
	Renderer2D::SetProjectionMatrix(projection);

	glm::mat4 viewMat = _activeCamera->GetViewMatrix();
	Renderer2D::SetViewMatrix(viewMat);


	const auto view = _registry->view<const Renderable, const Transform>();
	for (auto [entity, renderable, transform] : view.each())
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, transform.Position);
		glm::mat4 rotMatrix = glm::toMat4(transform.Rotation);
		model = model * rotMatrix;
		model = glm::scale(model, transform.Scale);

		std::string key = renderable.MeshKey;
		Renderer2D::RenderEnt(key, model);
	}
}

Mystic::Ref<entt::registry> Mystic::Scene::GetRegistry()
{
	return _registry;
}
