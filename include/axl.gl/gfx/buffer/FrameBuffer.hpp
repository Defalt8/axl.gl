#pragma once
#include "../../lib.hpp"
#include "../../ContextObject.hpp"
#include <axl.glfl/glCoreARB.hpp>

namespace axl {
namespace gl {

class AXLGLCXXAPI Context;

namespace gfx {

class AXLGLCXXAPI FrameBuffer : public ContextObject
{
	public:
		AXLGL_ENUM_CLASS Target { FBT_BOTH = 0, FBT_READ, FBT_WRITE };
	public:
		FrameBuffer(axl::gl::Context* ptr_context = (axl::gl::Context*)0);
		virtual ~FrameBuffer();
		bool icreate();
		bool idestroy();
		bool isValid() const;
		axl::glfl::GLuint getId() const;
		bool bind(Target target = Target::FBT_BOTH) const;
		bool unbind(Target target = Target::FBT_BOTH) const;
	protected:
		axl::glfl::GLuint fbo_id;
};

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
