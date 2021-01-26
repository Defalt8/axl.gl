#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/gfx/RenderTarget.hpp>

namespace axl {
namespace gl {
namespace gfx {

RenderTarget::RenderTarget() :
	context(m_context),
	type(m_type),
	size(m_size),
	m_context(),
	m_type(),
	m_size()
{}

RenderTarget::~RenderTarget()
{}

bool RenderTarget::create(axl::gl::Context* context, Type type, const axl::math::Vec2i& size)
{
	if(!context || !context->isValid()) return false;
	return false;
}

bool RenderTarget::destroy()
{
	return false;
}


} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
