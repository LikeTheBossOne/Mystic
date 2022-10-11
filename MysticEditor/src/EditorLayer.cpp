#define NOMINMAX
#include "EditorLayer.h"
#include <imgui/imgui.h>

//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "Mystic/Scene/ProjectSerializer.h"
#include "Mystic/Scene/RuntimeScene.h"
#include "Mystic/Scene/ProjectScene.h"

#include "ImGuizmo.h"
#include "singleton.h"
#include "glm/gtc/type_ptr.hpp"
#include "Mystic/Assets/FBXImporter.h"
#include "Mystic/Core/Application.h"
#include "Mystic/Core/Input.h"
#include "Mystic/ECS/Components/TagComponent.h"
#include "Mystic/ECS/Components/TransformComponent.h"
#include "Mystic/Render/RenderCommand.h"
#include "Mystic/Render/Renderer2D.h"
#include "Mystic/ImGui/ImGuiLayer.h"
#include "Mystic/Render/Mesh.h"
#include "Mystic/Render/Renderer3D.h"
#include "Mystic/GameCode/GameCodeSystem.h"

namespace Mystic {

	extern const std::string g_AssetPath = "assets";

	EditorLayer::EditorLayer()
		: _cameraController(1280.0f / 720.0f), _squareColor({0.2f, 0.3f, 0.8f, 1.0f})
	{
	}

	void EditorLayer::OnAttach()
	{
		_iconPlay = Texture2D::Create("PlayButton", "Resources/Icons/PlayButton.png");
		_iconStop = Texture2D::Create("StopButton", "Resources/Icons/StopButton.png");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		_framebuffer = Framebuffer::Create(fbSpec);

		_activeProjectScene = std::make_shared<ProjectScene>();
		_activeScene = _activeProjectScene;
		_activeProjectScene->ReloadGameCode();

		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			std::string sceneFilePath = commandLineArgs[1];
			ProjectSerializer serializer(_activeProjectScene);
			serializer.DeserializeProject(sceneFilePath);
		}

		
		GameCodeSystem::InitImGui(ImGui::GetCurrentContext());

		_editorCamera = EditorCamera(30.0f, 1.778f, 0.001f, 1000.0f);

		_sceneHierarchyPanel.SetContext(_activeScene);

		/*std::unordered_map<FBXAssetType, std::unordered_map<std::string, std::string>> map;
		FBXImporter::Import("../Sandbox/assets/fbx/BaseCharacter.fbx", map);
		for (auto filePair : map[FBXAssetType::MESH])
		{
			Ref<Mesh> m = Mesh::CreateMeshFromMystAsset(filePair.second);
		}*/
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(float deltaTime)
	{
		// Resize
		if (FramebufferSpecification spec = _framebuffer->GetSpecification();
			_viewportSize.x > 0.0f && _viewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != _viewportSize.x || spec.Height != _viewportSize.y))
		{
			_framebuffer->Resize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
			_cameraController.OnResize(_viewportSize.x, _viewportSize.y);
			_editorCamera.SetViewportSize(_viewportSize.x, _viewportSize.y);
			_activeScene->OnViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
		}

		// OnRender
		Renderer2D::ResetStats();
		_framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.3f, 1 });
		RenderCommand::Clear();

		// Clear our entity ID attachment to -1
		_framebuffer->ClearAttachment(1, -1);

		switch (_sceneState)
		{
			case SceneState::Edit:
			{
				if (_viewportFocused)
					_cameraController.OnUpdate(deltaTime);

				_editorCamera.OnUpdate(deltaTime);

				_activeProjectScene->OnUpdate(deltaTime);
				_activeProjectScene->OnRender(_editorCamera);
				break;
			}
			case SceneState::Play:
			{
				_activeRuntimeScene->OnUpdate(deltaTime);
				_activeRuntimeScene->OnRender();
				break;
			}
		}

		//ImGui::SetCurrentContext(&singleton<ServiceLocator>().GetImGuiContext());
		auto[mx, my] = ImGui::GetMousePos();
		mx -= _viewportBounds[0].x;
		my -= _viewportBounds[0].y;
		glm::vec2 viewportSize = _viewportBounds[1] - _viewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = _framebuffer->ReadPixel(1, mouseX, mouseY);
			_hoveredEntity = pixelData == -1 ? Entity() : _activeScene->GetEntity((entt::entity)pixelData);
		}

		_framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		//ImGui::SetCurrentContext(&singleton<ServiceLocator>().GetImGuiContext());

		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);1
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Exit"))
					Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		_sceneHierarchyPanel.OnImGuiRender();
		//_contentBrowserPanel.OnImGuiRender();

		ImGui::Begin("Stats");

		std::string name = "None";
		if (_hoveredEntity)
			name = _activeScene->EntityGetComponent<TagComponent>(_hoveredEntity.EntId).Tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		_viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		_viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		_viewportFocused = ImGui::IsWindowFocused();
		_viewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!_viewportFocused && !_viewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = _framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ _viewportSize.x, _viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				const std::wstring wstrPath(path);
				const std::string strPath(wstrPath.begin(), wstrPath.end());
				const std::string fullStrPath = g_AssetPath + "/" + strPath;
				OpenScene(fullStrPath);
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmos
		Entity selectedEntity = _sceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && _gizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(_viewportBounds[0].x, _viewportBounds[0].y, _viewportBounds[1].x - _viewportBounds[0].x, _viewportBounds[1].y - _viewportBounds[0].y);

			// Camera
			
			// Runtime camera from entity
			// auto cameraEntity = _activeScene->GetPrimaryCameraEntity();
			// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			// const glm::mat4& cameraProjection = camera.GetProjection();
			// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor camera
			const glm::mat4& cameraProjection = _editorCamera.GetProjection();
			glm::mat4 cameraView = _editorCamera.GetViewMatrix();

			// Entity transform
			TransformComponent tc = _activeScene->_registry.get<TransformComponent>(selectedEntity.EntId);
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (_gizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)_gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			/*if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}*/
		}


		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

		ImGui::End();
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		Ref<Texture2D> icon = _sceneState == SceneState::Edit ? _iconPlay : _iconStop;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (_sceneState == SceneState::Edit)
				OnScenePlay();
			else if (_sceneState == SceneState::Play)
				OnSceneStop();
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		_cameraController.OnEvent(e);
		_editorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(MYST_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(MYST_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
					NewScene();

				break;
			}
			case Key::O:
			{
				if (control)
					OpenScene();

				break;
			}
			case Key::S:
			{
				if (control && shift)
					SaveSceneAs();

				break;
			}

			// Gizmos
			case Key::Q:
			{
				if (!ImGuizmo::IsUsing())
					_gizmoType = -1;
				break;
			}
			case Key::W:
			{
				if (!ImGuizmo::IsUsing())
					_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case Key::E:
			{
				if (!ImGuizmo::IsUsing())
					_gizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case Key::R:
			{
				if (!ImGuizmo::IsUsing())
					_gizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
		}
		return true;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (_viewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				_sceneHierarchyPanel.SetSelectedEntity(_hoveredEntity);
		}
		return false;
	}

	void EditorLayer::NewScene()
	{
		_activeScene = std::make_shared<Scene>();
		_activeScene->OnViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
		_sceneHierarchyPanel.SetContext(_activeScene);
	}

	void EditorLayer::OpenScene()
	{
		/*std::string filepath = FileDialogs::OpenFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!filepath.empty())
			OpenScene(filepath);*/
	}

	void EditorLayer::OpenScene(const std::string& path)
	{
		//if (path.extension().string() != ".hazel")
		//{
		//	//HZ_WARN("Could not load {0} - not a scene file", path.filename().string());
		//	return;
		//}
		
		Ref<ProjectScene> newScene = std::make_shared<ProjectScene>();
		ProjectSerializer serializer(newScene);
		if (serializer.DeserializeProject(path))
		{
			_activeScene = newScene;
			_activeScene->OnViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
			_sceneHierarchyPanel.SetContext(_activeScene);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string sceneFilePath;
		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			sceneFilePath = commandLineArgs[1];
		}
		else
		{
			sceneFilePath = "UntitledScene.myst";
		}

		ProjectSerializer serializer(_activeProjectScene);
		serializer.SerializeProject(sceneFilePath);
		
	}

	void EditorLayer::OnScenePlay()
	{
		_sceneState = SceneState::Play;
		_activeRuntimeScene = _activeProjectScene->CreateRuntimeScene();
		_activeScene = _activeRuntimeScene;
	}

	void EditorLayer::OnSceneStop()
	{
		_sceneState = SceneState::Edit;
		_activeScene = _activeProjectScene;
	}

}
