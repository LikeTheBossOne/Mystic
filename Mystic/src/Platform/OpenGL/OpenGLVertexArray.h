#pragma once

#include "Mystic/GFX/VertexArray.h"

namespace Mystic {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return _vertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const { return _indexBuffer; }
	private:
		uint32_t _rendererID;
		uint32_t _vertexBufferIndex = 0;
		std::vector<Ref<VertexBuffer>> _vertexBuffers;
		Ref<IndexBuffer> _indexBuffer;
	};

}