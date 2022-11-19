#pragma once

#include "Mystic/Render/Texture.h"

#include <glad/glad.h>

namespace Mystic {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& name, uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& name, const std::string& path);
		virtual ~OpenGLTexture2D();

		uint32_t GetWidth() const override { return _width;  }
		uint32_t GetHeight() const override { return _height; }
		uint32_t GetRendererID() const override { return _rendererID; }
		
		void SetData(void* data, uint32_t size) override;

		void Bind(uint32_t slot = 0) const override;

		bool IsLoaded() const override { return _isLoaded; }

		const std::string& GetName() const override { return _name; }

		bool operator==(const Texture& other) const override
		{
			return _rendererID == ((OpenGLTexture2D&)other)._rendererID;
		}

	private:
		std::string _path;
		std::string _name;

		bool _isLoaded = false;
		uint32_t _width, _height;
		uint32_t _rendererID;
		GLenum _internalFormat, _dataFormat;
	};

}
