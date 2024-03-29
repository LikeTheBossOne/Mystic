#pragma once

#include <cstdint>

#include "Mystic/Render/Buffer.h"

namespace Mystic {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const BufferLayout& GetLayout() const override { return _layout; }
		virtual void SetLayout(const BufferLayout& layout) override { _layout = layout; }
	private:
		uint32_t _rendererID;
		BufferLayout _layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer() override;

		void Bind() const override;
		void Unbind() const override;

		virtual uint32_t GetCount() const override { return _count; }
	private:
		uint32_t _rendererID;
		uint32_t _count;
	};
}