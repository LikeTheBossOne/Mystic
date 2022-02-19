#pragma once
#include <algorithm>
#include <cstdint>
#include <memory>
#include <vector>

#include "Model.h"

namespace Mystic
{
	class MYSTIC_API RenderJob
	{
	public:
		RenderJob(Ref<Model> model)
		{
			_model = model;
		}

		Ref<Model> GetModel() const { return _model; }
	private:
		Ref<Model> _model;
	};
}
