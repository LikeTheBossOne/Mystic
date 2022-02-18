#pragma once
#include "Layer.h"

namespace Mystic
{
	class MYSTIC_API EditorLayer :
		public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() override;

		void OnEvent(Event& e) override;
	};
}
