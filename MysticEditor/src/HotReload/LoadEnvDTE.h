#pragma once

#include <windows.h>
#include <stdio.h>
#include <winnt.h>
#include <memory>

namespace Mystic
{
	namespace VSEnv
	{
		void Init();
		void AttachVS();
		void DetachVS(bool waitForBreakOrEnd);
		void DoThing();
	}
}