#pragma once
#include <memory>
#include <string>
#include "../Core.h"

namespace Mystic
{
	class InstEditor
	{
	public:
		virtual ~InstEditor() = default;
		
		virtual void Init(int windowWidth, int windowHeight, std::string windowTitle) = 0;

		virtual void OnStart() = 0;
		virtual void OnEnd() = 0;
		
		virtual void PreRender() = 0;
		virtual void Render() = 0;
		virtual void PostRender() = 0;
		
		virtual bool ShouldClose() = 0;
		virtual bool PollEvents() = 0;
	};
	
	class MYSTIC_API Editor
	{
	public:
		static void Init(int windowWidth, int windowHeight, std::string title);
		static void Start();

	private:
		static void PreRender();
		static void Render();
		static void PostRender();

	private:
		static std::unique_ptr<InstEditor> s_editor;
	};
}
