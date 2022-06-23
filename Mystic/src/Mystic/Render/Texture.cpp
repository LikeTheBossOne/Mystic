#include "mystpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Mystic {

	Ref<Texture2D> Texture2D::Create(const std::string& name, uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    assert(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(name, width, height);
		}

		assert(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& name, const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    assert(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(name, path);
		}

		assert(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void Texture2DLibrary::Add(const std::string& name, const Ref<Texture2D>& texture)
	{
		assert(!Exists(name), "Texture already exists!");
		_textures[name] = texture;
	}

	void Texture2DLibrary::Add(const Ref<Texture2D>& texture)
	{
		auto& name = texture->GetName();
		Add(name, texture);
	}


	Ref<Texture2D> Texture2DLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto texture = Texture2D::Create(name, filepath);
		Add(name, texture);
		return texture;
	}

	Ref<Texture2D> Texture2DLibrary::Get(const std::string& name)
	{
		assert(Exists(name), "Shader not found!");
		return _textures[name];
	}

	bool Texture2DLibrary::Exists(const std::string& name) const
	{
		return _textures.find(name) != _textures.end();
	}
}
