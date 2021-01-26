#pragma once
#include "../lib.hpp"
#include <axl.math/vec/Vec2i.hpp>

namespace axl {
namespace gl {
namespace gfx {

class AXLGLCXXAPI Context;

class AXLGLCXXAPI Camera
{
	public:
		Camera(Context* context, const axl::math::Vec2i& viewport_position, const axl::math::Vec2i& viewport_size);
		virtual ~Camera();
	public:
		Context* const& context;
		const axl::math::Vec2i& viewport_position;
		const axl::math::Vec2i& viewport_size;
	private:
		Context* m_context;
		axl::math::Vec2i m_viewport_position;
		axl::math::Vec2i m_viewport_size;
};


} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
