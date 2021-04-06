#include <axl.gl/lib.hpp>
#include <axl.gl/gfx/UI/UIFrameBuffer.hpp>
#include <axl.glfl/glCoreARB.hpp>

#define GLCLEARERROR() while(GL::glGetError() != GL_NO_ERROR)

namespace GL {
	using namespace axl::glfl;
	using namespace axl::glfl::core::GL;
}

namespace axl {
namespace gl {
namespace gfx {

UIFrameBuffer::UIFrameBuffer(axl::gl::Context* ptr_context) :
	axl::gl::gfx::FrameBuffer(ptr_context),
	uifb_render_buffer_depth(),
	uifb_size(0,0)
{}
UIFrameBuffer::~UIFrameBuffer()
{
	this->destroy();
}
bool UIFrameBuffer::isValid() const
{
	return axl::gl::gfx::FrameBuffer::isValid() &&
		this->uifb_render_buffer_depth.isValid();
}
void UIFrameBuffer::setContext(axl::gl::Context* ptr_context)
{
	axl::gl::gfx::FrameBuffer::setContext(ptr_context);
	this->uifb_render_buffer_depth.setContext(ptr_context);
}
bool UIFrameBuffer::icreate()
{
	using namespace GL;
	if(!axl::gl::gfx::FrameBuffer::icreate() || !this->uifb_render_buffer_depth.create())
	{
		this->destroy();
		return false;
	}
	return this->uifb_render_buffer_depth.allocate(this->uifb_size.x, this->uifb_size.y, GL_DEPTH_COMPONENT24, 0) &&
		this->attachRenderBuffer(GL_DEPTH_ATTACHMENT, &this->uifb_render_buffer_depth);
}
bool UIFrameBuffer::idestroy()
{
	using namespace GL;
	return this->attachTexture2D(GL_COLOR_ATTACHMENT0, 0) &&
		this->attachRenderBuffer(GL_DEPTH_ATTACHMENT, 0) &&
		this->uifb_render_buffer_depth.destroy() &&
		axl::gl::gfx::FrameBuffer::idestroy();
}
bool UIFrameBuffer::setSize(const axl::math::Vec2i& size)
{
	using namespace GL;
	if(size.x < 0 || size.y < 0)
		return false;
	if(this->isValid())
	{
		if(!this->uifb_render_buffer_depth.allocate(size.x, size.y, GL_DEPTH_COMPONENT24, 0))
			return false;
	}
	this->uifb_size = size;
	return true;
}
const axl::math::Vec2i& UIFrameBuffer::getSize() const
{
	return this->uifb_size;
}

} // axl.gl.gfx
} // axl.gl
} // axl
