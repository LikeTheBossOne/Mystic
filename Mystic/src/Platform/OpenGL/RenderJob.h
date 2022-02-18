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
		RenderJob(std::shared_ptr<Model> model)
		{
			_model = model;
		}

		std::shared_ptr<Model> GetModel() const { return _model; }
	private:
		std::shared_ptr<Model> _model;
	};
}
