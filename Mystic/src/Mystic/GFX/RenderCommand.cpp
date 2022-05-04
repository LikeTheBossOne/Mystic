#include "mystpch.h"
#include "RenderCommand.h"

namespace Mystic {

	Unique<RendererAPI> RenderCommand::s_rendererAPI = RendererAPI::Create();

}