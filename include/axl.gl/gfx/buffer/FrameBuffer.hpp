#pragma once
#include "../../lib.hpp"
#include "../../ContextObject.hpp"
#include "../texture/Texture2D.hpp"
#include "RenderBuffer.hpp"
#include <axl.util/ds/List.hpp>
#include <axl.glfl/gl.hpp>

namespace axl {
namespace gl {

class AXLGLCXXAPI Context;

namespace gfx {

class AXLGLCXXAPI RenderBuffer;

struct AXLGLCXXAPI FrameBufferBinding
{
	AXLGL_ENUM_CLASS Type { BT_TEXTURE, BT_RENDER_BUFFER } type;
	union Pointer
	{
		Texture* texture;
		RenderBuffer* render_buffer;
	} pointer;
	axl::glfl::GLenum attachment_target;
	FrameBufferBinding();
	FrameBufferBinding(Type type, axl::glfl::GLenum attachment_target, void* ptr);
	bool operator==(const FrameBufferBinding& binding) const;
};

} // axl.gl.gfx
} // axl.gl

namespace util {
namespace ds {

template class AXLGLCXXAPI UniList<axl::gl::gfx::FrameBufferBinding>;
#ifndef __GNUC__
template class AXLGLCXXAPI UniList<axl::gl::gfx::FrameBufferBinding>::Iterator;
#endif

} // axl.util.ds
} // axl.util

namespace gl {
namespace gfx {

class AXLGLCXXAPI FrameBuffer : public ContextObject
{
	public:
		AXLGL_ENUM_CLASS Target { FBT_BOTH = 0, FBT_READ, FBT_DRAW };
	public:
		FrameBuffer(axl::gl::Context* ptr_context = (axl::gl::Context*)0);
		virtual ~FrameBuffer();
	protected:
		bool iCreate();
		bool iDestroy();
	public:
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
		axl::util::ds::UniList<FrameBufferBinding> fb_bindings;
		friend class AXLGLCXXAPI RenderBuffer;
};

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
