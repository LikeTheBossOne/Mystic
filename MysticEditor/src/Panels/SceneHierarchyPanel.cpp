#include "SceneHierarchyPanel.h"

#include "glm/vec3.hpp"
#include <glm/gtx/quaternion.hpp>

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "../Utility/EditorStructControls.h"
#include "Mystic/ECS/Components/CameraComponent.h"
#include "Mystic/ECS/Components/CharacterComponent.h"
#include "Mystic/ECS/Components/SpriteRendererComponent.h"
#include "Mystic/ECS/Components/TagComponent.h"
#include "Mystic/ECS/Components/TransformComponent.h"
#include "Mystic/ECS/Components/VelocityComponent.h"

namespace Mystic
{
	extern const std::string g_AssetPath;

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		_sceneContext = scene;
		_selectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		_sceneContext->_registry.each([&](auto entityID)
			{
				Entity entity = _sceneContext->GetEntity(entityID);
				DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			_selectionContext = {};

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				_sceneContext->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (_selectionContext)
		{
			DrawComponents(_selectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		_selectionContext = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = _sceneContext->_registry.get<TagComponent>(entity.EntId).Tag;

		ImGuiTreeNodeFlags flags = ((_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)entity.EntId, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			_selectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			_sceneContext->DestroyEntity(entity);
			if (_selectionContext == entity)
				_selectionContext = {};
		}
	}

	//TODO: ECS-ify this. This should theoretically act like a pure ECS system instead of this
	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (_sceneContext->EntityHasComponent<TagComponent>(entity))
		{
			auto& tagComp = _sceneContext->EntityGetComponent<TagComponent>(entity);
			std::string tag = tagComp.Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("CameraComponent"))
			{
				if (!_sceneContext->EntityHasComponent<CameraComponent>(_selectionContext))
					_sceneContext->EntityAddComponent<CameraComponent>(entity);
				//else
					//HZ_CORE_WARN("This entity already has the Camera Component!");
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("SpriteRendererComponent"))
			{
				if (!_sceneContext->EntityHasComponent<SpriteRendererComponent>(_selectionContext))
					_sceneContext->EntityAddComponent<SpriteRendererComponent>(_selectionContext);
				//else
					//HZ_CORE_WARN("This entity already has the Sprite Renderer Component!");
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("VelocityComponent"))
			{
				if (!_sceneContext->EntityHasComponent<VelocityComponent>(_selectionContext))
					_sceneContext->EntityAddComponent<VelocityComponent>(_selectionContext);
				//else
					//HZ_CORE_WARN("This entity already has the Velocity Component!");
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("CharacterComponent"))
			{
				if (!_sceneContext->EntityHasComponent<CharacterComponent>(_selectionContext))
					_sceneContext->EntityAddComponent<CharacterComponent>(_selectionContext);
				//else
					//HZ_CORE_WARN("This entity already has the Character Component!");
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		EditorStructControls::DrawComponent<TransformComponent>("TransformComponent", entity, [](TransformComponent& component)
			{
				EditorStructControls::DrawVec3Control("Translation", component.Position);
				glm::vec3 eulerRotation = glm::eulerAngles(component.Rotation);
				glm::vec3 rotationDegs = glm::degrees(eulerRotation);
				EditorStructControls::DrawVec3Control("Rotation", rotationDegs);
				component.Rotation = glm::quat(glm::radians(rotationDegs));
				EditorStructControls::DrawVec3Control("Scale", component.Scale, 1.0f);
			});

		EditorStructControls::DrawComponent<CameraComponent>("CameraComponent", entity, [](CameraComponent& component)
			{
				auto& camera = component.Camera;

				ImGui::Checkbox("Primary", &component.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

					float perspectiveNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear))
						camera.SetPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar))
						camera.SetPerspectiveFarClip(perspectiveFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
				}
			});

		EditorStructControls::DrawComponent<SpriteRendererComponent>("SpriteRendererComponent", entity, [](SpriteRendererComponent& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

				ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						const std::wstring wstrPath(path);
						const std::string strPath(wstrPath.begin(), wstrPath.end());
						std::string texturePath = g_AssetPath + "/" + strPath;
						//TODO: Change this to use an actual name
						Ref<Texture2D> texture = Texture2D::Create(strPath, texturePath);
						if (texture->IsLoaded())
							component.Texture = texture;
						//else
							//HZ_WARN("Could not load texture {0}", texturePath.filename().string());
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
			});

		EditorStructControls::DrawComponent<VelocityComponent>("VelocityComponent", entity, [](VelocityComponent& component)
			{
				EditorStructControls::DrawVec3Control("Velocity", component.Velocity);
			});

		EditorStructControls::DrawComponent<CharacterComponent>("CharacterComponent", entity, [](CharacterComponent& component)
			{
				ImGui::Checkbox("Active", &component.Active);
			});
	}
}
