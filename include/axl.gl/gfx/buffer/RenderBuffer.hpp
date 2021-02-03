#pragma once
#include "../../lib.hpp"
#include "../../ContextObject.hpp"
#include <axl.glfl/gl.hpp>
#include <axl.glfl/glCoreARB.hpp>

namespace axl {
namespace gl {

class AXLGLCXXAPI Context;

namespace gfx {

class AXLGLCXXAPI RenderBuffer : public ContextObject
{
	public:
		RenderBuffer(axl::gl::Context* ptr_context = (axl::gl::Context*)0);
		virtual ~RenderBuffer();
		bool icreate();
		bool idestroy();
		bool isValid() const;
		axl::glfl::GLuint getId() const;
		bool bind() const;
		bool unbind() const;
		bool allocate(axl::glfl::GLsizei samples, axl::glfl::GLenum internal_format, axl::glfl::GLsizei width, axl::glfl::GLsizei height);
	protected:
		axl::glfl::GLuint rbo_id;
};

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl