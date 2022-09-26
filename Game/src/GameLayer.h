#pragma once

#include "Mystic/Core/Layer.h"
#include "Mystic/GameCode/GameCodeLoader.h"
#include "Mystic/Scene/RuntimeScene.h"

namespace Game
{
	class GameLayer : public Mystic::Layer
	{
	public:
		GameLayer();
		virtual ~GameLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(float deltaTime) override;
	private:
		Mystic::Ref<Mystic::RuntimeScene> _scene;
		Mystic::Ref<Mystic::GameCodeLoader> _gameCodeLoader;
	};
}
