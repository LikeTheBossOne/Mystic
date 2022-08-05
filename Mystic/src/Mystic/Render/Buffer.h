#pragma once
#include <cassert>
#include <vector>
#include <string>

#include "Mystic/Core/Core.h"

namespace Mystic {

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Mat3:     return 4 * 3 * 3;
		case ShaderDataType::Mat4:     return 4 * 4 * 4;
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		case ShaderDataType::Bool:     return 1;
		}

		assert(false, "Unknown ShaderDataType!");
		return 0;
	}

	static std::string ShaderDataTypeName(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return "Float";
		case ShaderDataType::Float2:   return "Float2";
		case ShaderDataType::Float3:   return "Float3";
		case ShaderDataType::Float4:   return "Float4";
		case ShaderDataType::Mat3:     return "Mat3";
		case ShaderDataType::Mat4:     return "Mat4";
		case ShaderDataType::Int:      return "Int";
		case ShaderDataType::Int2:     return "Int2";
		case ShaderDataType::Int3:     return "Int3";
		case ShaderDataType::Int4:	   return "Int4";
		case ShaderDataType::Bool:     return "Bool";
		}

		assert(false, "Unknown ShaderDataType!");
		return "null";
	}

	static ShaderDataType ShaderDataTypeFromName(std::string name)
	{
		if (name == "Float")
			return ShaderDataType::Float;
		if (name == "Float2")
			return ShaderDataType::Float2;
		if (name == "Float3")
			return ShaderDataType::Float3;
		if (name == "Float4")
			return ShaderDataType::Float4;
		if (name == "Mat3")
			return ShaderDataType::Mat3;
		if (name == "Mat4")
			return ShaderDataType::Mat4;
		if (name == "Int")
			return ShaderDataType::Int;
		if (name == "Int2")
			return ShaderDataType::Int2;
		if (name == "Int3")
			return ShaderDataType::Int3;
		if (name == "Int4")
			return ShaderDataType::Int4;
		if (name == "Bool")
			return ShaderDataType::Bool;

		return ShaderDataType::None;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		size_t Offset;
		bool Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3; // 3* float3
			case ShaderDataType::Mat4:    return 4; // 4* float4
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			}

			assert(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		BufferLayout(std::vector<BufferElement> elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		uint32_t GetStride() const { return m_Stride; }
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};

	// Currently Mystic only supports 32-bit index buffers
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};
}