#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/gfx/buffer/FrameBuffer.hpp>
#include <axl.gl/gfx/buffer/RenderBuffer.hpp>
#include <axl.glfl/gl.hpp>
#include <axl.glfl/glCoreARB.hpp>

#define GLCLEARERROR() while(axl::glfl::core::GL1::glGetError())

namespace GL {
using namespace axl::glfl::core::GL1;
using namespace axl::glfl::core::GL2;
using namespace axl::glfl::core::GL3;
}

namespace axl {
namespace gl {
namespace gfx {

RenderBuffer::RenderBuffer(axl::gl::Context* ptr_context) :
	ContextObject(ptr_context),
	rb_id(0)
{}
RenderBuffer::~RenderBuffer()
{
	this->destroy();
}
bool RenderBuffer::iCreate()
{
	using namespace GL;
	if(!GL_VERSION_3_0 || this->isValid() || !(this->ctx_context && (this->ctx_context->isCurrent() || this->ctx_context->makeCurrent()))) return false;
	axl::glfl::GLuint tmp_id;
	GLCLEARERROR();
	glGenRenderbuffers(1, &tmp_id);
	if(tmp_id == 0) return false;
	if(glGetError() != GL_NO_ERROR)
	{
		glDeleteRenderbuffers(1, &tmp_id);
		return false;
	}
	this->rb_id = tmp_id;
	return true;
}
bool RenderBuffer::iDestroy()
{
	using namespace GL;
	if(this->unbind() || (this->ctx_context && (this->ctx_context->isCurrent() || this->ctx_context->makeCurrent())))
	{
		GLCLEARERROR();
		glDeleteRenderbuffers(1, &this->rb_id);
		if(glGetError() != GL_NO_ERROR) return false;
		this->rb_id = 0;
		return true;
	}
	return false;
}
bool RenderBuffer::isValid() const
{
	return this->ctx_context && this->ctx_context->isValid() && this->rb_id != 0;
}
axl::glfl::GLuint RenderBuffer::getId() const
{
	return this->rb_id;
}
bool RenderBuffer::bind() const
{
	using namespace GL;
	if(!this->isValid() || !(this->ctx_context->isCurrent() || this->ctx_context->makeCurrent())) return false;
	GLCLEARERROR();
	glBindRenderbuffer(GL_RENDERBUFFER, this->rb_id);
	return glGetError() == GL_NO_ERROR;
}
bool RenderBuffer::unbind() const
{
	using namespace GL;
	if(!this->isValid() || !(this->ctx_context->isCurrent() || this->ctx_context->makeCurrent())) return false;
	GLCLEARERROR();
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	return glGetError() == GL_NO_ERROR;
}
bool RenderBuffer::allocate(axl::glfl::GLsizei width, axl::glfl::GLsizei height, axl::glfl::GLenum internal_format, axl::glfl::GLsizei samples)
{
	using namespace GL;
	if(!this->bind()) return false;
	if(samples > 0)
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internal_format, width, height);
	else
		glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	return glGetError() == GL_NO_ERROR;
}

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
