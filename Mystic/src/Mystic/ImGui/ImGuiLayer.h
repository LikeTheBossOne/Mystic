#pragma once
#include "../Core/Core.h"
#include "Mystic/Core/Layer.h"

namespace Mystic
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& event) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { _blockEvents = block; }

	private:
		bool _blockEvents;
	};
}
