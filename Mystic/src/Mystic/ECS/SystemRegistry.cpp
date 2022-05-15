#include "SystemRegistry.h"

namespace Mystic
{
	std::unordered_map<std::string, std::pair<Ref<System>, bool>> SystemRegistry::s_systems;
}