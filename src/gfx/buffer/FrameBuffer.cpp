#pragma once
#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/gfx/buffer/FrameBuffer.hpp>
#include <axl.glfl/glCoreARB.hpp>

namespace axl {
namespace gl {
namespace gfx {

FrameBuffer::FrameBuffer(axl::gl::Context* ptr_context) :
	ContextObject(ptr_context),
	fbo_id(-1)
{}
FrameBuffer::~FrameBuffer()
{
	this->destroy();
}
bool FrameBuffer::icreate()
{
	return false;
}
bool FrameBuffer::idestroy()
{
	return false;
}
bool FrameBuffer::isValid() const
{
	return this->ctx_context && this->ctx_context->isValid() && this->fbo_id != -1;
}
axl::glfl::GLuint FrameBuffer::getId() const
{
	return this->fbo_id;
}
bool FrameBuffer::bind() const
{
	return false;
}
bool FrameBuffer::unbind() const
{
	return false;
}

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
