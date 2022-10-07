#pragma once
#include "Mystic/Core/Core.h"
#include <string>

namespace Mystic {

	class MYSTIC_API Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool IsLoaded() const = 0;

		virtual const std::string& GetName() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class MYSTIC_API Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& name, uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& name, const std::string& path);
	};

	class Texture2DLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Texture2D>& texture);
		void Add(const Ref<Texture2D>& texture);
		Ref<Texture2D> Load(const std::string& name, const std::string& filepath);

		Ref<Texture2D> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Texture2D>> _textures;
	};
}
