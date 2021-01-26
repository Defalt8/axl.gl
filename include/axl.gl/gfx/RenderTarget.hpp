#pragma once
#include "../lib.hpp"
#include <axl.math/vec/Vec2i.hpp>
#include <axl.glfl/gl.hpp>
#include "Texture2D.hpp"

namespace axl {
namespace gl {

class AXLGLCXXAPI Context;

namespace gfx {

class AXLGLCXXAPI RenderTarget
{
	public:
		AXLGL_ENUM_CLASS Type { RTT_DEFAULT = 0, RTT_VIEW = 0, RTT_BUFFER, RTT_TEXTURE };
		union Target
		{
			axl::glfl::GLuint id;
		};
	public:
		RenderTarget();
		virtual ~RenderTarget();
		bool create(axl::gl::Context* context, Type type, const axl::math::Vec2i& size);
		bool destroy();
	public:
		axl::gl::Context* const& context;
		const Type& type;
		const axl::math::Vec2i& size;
	private:
		axl::gl::Context* m_context;
		Type m_type;
		axl::math::Vec2i m_size;
		Target m_target;
};


} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
