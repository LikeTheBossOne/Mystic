#pragma once
#include "Mystic/Core/Core.h"

#define Log(format, ...) _Log(__FILE__, __LINE__, format, __VA_ARGS__)
#define Info(format, ...) _Info(__FILE__, __LINE__, format, __VA_ARGS__)
#define Warning(format, ...) _Warning(__FILE__, __LINE__, format, __VA_ARGS__)
#define MError(format, ...) _Error(__FILE__, __LINE__, format, __VA_ARGS__)
#define Assert(condition, format, ...) _Assert(__FILE__, __LINE__, condition, format, __VA_ARGS__)

namespace Mystic
{
	namespace Log
	{
		MYSTIC_API void _Log(const char* filename, int line, const char* format, ...);
		MYSTIC_API void _Info(const char* filename, int line, const char* format, ...);
		MYSTIC_API void _Warning(const char* filename, int line, const char* format, ...);
		MYSTIC_API void _Error(const char* filename, int line, const char* format, ...);
		MYSTIC_API void _Assert(const char* filename, int line, int condition, const char* format, ...);
	}
}