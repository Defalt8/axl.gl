#pragma once
#include "../../lib.hpp"
#include "../../ContextObject.hpp"
#include "RenderBuffer.hpp"
#include "../texture/Texture2D.hpp"
#include <axl.glfl/glCoreARB.hpp>

namespace axl {
namespace gl {

class AXLGLCXXAPI Context;

namespace gfx {

class AXLGLCXXAPI FrameBuffer : public ContextObject
{
		struct Binding
		{
			AXLGL_ENUM_CLASS Type { BT_TEXTURE, BT_RENDER_BUFFER } type;
			union Pointer
			{
				Texture* texture;
				RenderBuffer* render_buffer;
			} pointer;
			Binding();
			Binding(Type type, void* ptr);
		};
	public:
		AXLGL_ENUM_CLASS Target { FBT_BOTH = 0, FBT_READ, FBT_DRAW };
	public:
		FrameBuffer(axl::gl::Context* ptr_context = (axl::gl::Context*)0);
		virtual ~FrameBuffer();
		bool icreate();
		bool idestroy();
		bool isValid() const;
		axl::glfl::GLuint getId() const;
		bool bind(Target target = Target::FBT_BOTH) const;
		bool unbind(Target target = Target::FBT_BOTH) const;
		bool isComplete() const;
		bool attachRenderBuffer(axl::glfl::GLenum attachment_target, RenderBuffer* render_buffer, Target target = Target::FBT_BOTH);
		bool attachTexture2D(axl::glfl::GLenum attachment_target, Texture2D* texture, Target target = Target::FBT_BOTH);
		bool blit(const FrameBuffer* draw_framebuffer, axl::glfl::GLint srcX0, axl::glfl::GLint srcY0, axl::glfl::GLint srcX1, axl::glfl::GLint srcY1, axl::glfl::GLint dstX0, axl::glfl::GLint dstY0, axl::glfl::GLint dstX1, axl::glfl::GLint dstY1, axl::glfl::GLbitfield mask, axl::glfl::GLenum filter) const;
	protected:
		axl::glfl::GLuint fb_id;
		axl::util::ds::UniList<Binding> fb_bindings;
};

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
