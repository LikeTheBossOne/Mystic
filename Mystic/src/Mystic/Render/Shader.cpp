#include "mystpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Mystic {

	Ref<Shader> Shader::Create(const std::string& filepath, BufferLayout& bufferLayout)
	{
		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		std::string name = filepath.substr(lastSlash, count);

		return Create(name, filepath, bufferLayout);
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& filepath, BufferLayout& bufferLayout)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    Log::Assert(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(name, filepath, bufferLayout);
		}

		Log::Assert(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, BufferLayout& bufferLayout)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    Log::Assert(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc, bufferLayout);
		}

		Log::Assert(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		Log::Assert(!Exists(name), "Shader already exists!");
		_shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath, BufferLayout& bufferLayout)
	{
		auto shader = Shader::Create(filepath, bufferLayout);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath, BufferLayout& bufferLayout)
	{
		auto shader = Shader::Create(filepath, bufferLayout);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		Log::Assert(Exists(name), "Shader not found!");
		return _shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return _shaders.find(name) != _shaders.end();
	}

}