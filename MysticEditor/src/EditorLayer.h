#pragma once

#include "Mystic/Core/Application.h"
#include "Mystic/Core/Layer.h"
#include "Mystic/Events/KeyEvent.h"

#include "Mystic/Render/EditorCamera.h"
#include "Mystic/Render/Framebuffer.h"
#include "Mystic/Render/OrthographicCameraController.h"
#include "Mystic/Render/Shader.h"
#include "Mystic/Render/Texture.h"
#include "Mystic/Render/VertexArray.h"
#include "Mystic/Scene/ProjectScene.h"
#include "Mystic/Scene/ProjectSerializer.h"
#include "Panels/SceneHierarchyPanel.h"

namespace Mystic {
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(float deltaTime) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::string& path);
		void SaveSceneAs();
		void Reload();

		void OnScenePlay();
		void OnSceneStop();

		// UI Panels
		void UI_Toolbar();
	private:
		SceneHierarchyPanel _sceneHierarchyPanel;

		OrthographicCameraController _cameraController;

		// Temp
		Ref<VertexArray> _squareVA;
		Ref<Shader> _flatColorShader;
		Ref<Framebuffer> _framebuffer;

		Ref<ProjectSerializer> _projectSerializer;
		Ref<Scene> _activeScene;

		Entity _squareEntity;
		Entity _cameraEntity;
		Entity _secondCamera;
		
		Entity _hoveredEntity;

		bool _primaryCamera = true;

		EditorCamera _editorCamera;

		bool _viewportFocused = false, _viewportHovered = false;
		glm::vec2 _viewportSize = { 0.0f, 0.0f };
		glm::vec2 _viewportBounds[2];

		glm::vec4 _squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		int _gizmoType = -1;

		enum class SceneState
		{
			Edit = 0, Play = 1
		};
		SceneState _sceneState = SceneState::Edit;

		// Editor resources
		Ref<Texture2D> _iconPlay, _iconStop;
	};

}
