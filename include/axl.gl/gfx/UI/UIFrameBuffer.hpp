#pragma once
#include "../../lib.hpp"
#include "../../Context.hpp"
#include "../buffer/FrameBuffer.hpp"
#include <axl.math/vec/Vec2i.hpp>

namespace axl {
namespace gl {
namespace gfx {

class AXLGLCXXAPI UIFrameBuffer : public axl::gl::gfx::FrameBuffer
{
	public:
		UIFrameBuffer(axl::gl::Context* ptr_context = 0);
		~UIFrameBuffer();
		bool isValid() const;
		void setContext(axl::gl::Context* ptr_context);
	protected:
		bool icreate();
		bool idestroy();
	public:
		bool setSize(const axl::math::Vec2i& size);
		const axl::math::Vec2i& getSize() const;
	protected:
		axl::gl::gfx::RenderBuffer uifb_render_buffer_depth;
		axl::math::Vec2i uifb_size;
};

} // axl.gl.gfx
} // axl.gl
} // axl
