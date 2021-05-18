#pragma once
#include "../../lib.hpp"
#include "../../ContextObject.hpp"
#include <axl.glfl/gl.hpp>
#include <axl.glfl/gl.hpp>

namespace axl {
namespace gl {

class AXLGLCXXAPI Context;

namespace gfx {

class AXLGLCXXAPI FrameBuffer;

class AXLGLCXXAPI RenderBuffer : public ContextObject
{
	public:
		RenderBuffer(axl::gl::Context* ptr_context = (axl::gl::Context*)0);
		virtual ~RenderBuffer();
	protected:
		bool iCreate();
		bool iDestroy();
	public:
		bool isValid() const;
		axl::glfl::GLuint getId() const;
		bool bind() const;
		bool unbind() const;
		bool allocate(axl::glfl::GLsizei width, axl::glfl::GLsizei height, axl::glfl::GLenum internal_format, axl::glfl::GLsizei samples);
	protected:
		axl::glfl::GLuint rb_id;
};

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
