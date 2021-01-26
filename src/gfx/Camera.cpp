#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/gfx/Camera.hpp>

namespace axl {
namespace gl {
namespace gfx {

Camera::Camera(Context* p_context, const axl::math::Vec2i& p_viewport_position, const axl::math::Vec2i& p_viewport_size) :
	context(m_context),
	viewport_position(m_viewport_position),
	viewport_size(m_viewport_size),
	m_viewport_position(),
	m_viewport_size()
{}

Camera::~Camera()
{}

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
