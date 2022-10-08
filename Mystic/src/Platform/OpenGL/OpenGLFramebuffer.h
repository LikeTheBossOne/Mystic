#pragma once
#include "Mystic/Logging/Log.h"

#include "Mystic/Render/Framebuffer.h"

namespace Mystic {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { Log::Assert(index < _colorAttachments.size(), ""); return _colorAttachments[index]; }

		virtual const FramebufferSpecification& GetSpecification() const override { return _specification; }
	private:
		uint32_t _rendererID = 0;
		FramebufferSpecification _specification;

		std::vector<FramebufferTextureSpecification> _colorAttachmentSpecifications;
		FramebufferTextureSpecification _depthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> _colorAttachments;
		uint32_t _depthAttachment = 0;
	};

}