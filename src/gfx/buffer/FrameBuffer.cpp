#pragma once
#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/gfx/buffer/FrameBuffer.hpp>
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
	using namespace GL;
	if(!GL_VERSION_3_0 || this->isValid() || !this->ctx_context->makeCurrent()) return false;
	axl::glfl::GLuint tmp_id;
	GLCLEARERROR();
	glGenFramebuffers(1, &tmp_id);
	if(tmp_id == -1) return false;
	if(glGetError() != GL_NO_ERROR)
	{
		glDeleteFramebuffers(1, &tmp_id);
		return false;
	}
	this->fbo_id = tmp_id;
	return glGetError() == GL_NO_ERROR;
}
bool FrameBuffer::idestroy()
{
	using namespace GL;
	if(this->unbind())
	{
		GLCLEARERROR();
		glDeleteFramebuffers(1, &this->fbo_id);
		if(glGetError() != GL_NO_ERROR) return false;
	}
	this->fbo_id = -1;
	return true;
}
bool FrameBuffer::isValid() const
{
	return this->ctx_context && this->ctx_context->isValid() && this->fbo_id != -1;
}
axl::glfl::GLuint FrameBuffer::getId() const
{
	return this->fbo_id;
}
bool FrameBuffer::bind(Target p_target) const
{
	using namespace GL;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return false;
	axl::glfl::GLenum fb_target = p_target == Target::FBT_READ ? GL_READ_FRAMEBUFFER : (p_target == Target::FBT_DRAW ? GL_DRAW_FRAMEBUFFER : GL_FRAMEBUFFER);
	GLCLEARERROR();
	glBindFramebuffer(fb_target, this->fbo_id);
	return glGetError() == GL_NO_ERROR;
}
bool FrameBuffer::unbind(Target p_target) const
{
	using namespace GL;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return false;
	axl::glfl::GLenum fb_target = p_target == Target::FBT_READ ? GL_READ_FRAMEBUFFER : (p_target == Target::FBT_DRAW ? GL_DRAW_FRAMEBUFFER : GL_FRAMEBUFFER);
	GLCLEARERROR();
	glBindFramebuffer(fb_target, 0);
	return glGetError() == GL_NO_ERROR;
}

bool FrameBuffer::isComplete() const
{
	using namespace GL;
	if(!this->bind(Target::FBT_BOTH)) return false;
	GLCLEARERROR();
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return glGetError() == GL_NO_ERROR;
}
bool FrameBuffer::attachRenderBuffer(axl::glfl::GLenum attachment_target, RenderBuffer* render_buffer, Target p_target)
{
	using namespace GL;
	if(!this->bind(Target::FBT_BOTH)) return false;
	axl::glfl::GLuint render_buffer_id = render_buffer && render_buffer->isValid() ? render_buffer->getId() : 0;
	axl::glfl::GLenum fb_target = p_target == Target::FBT_READ ? GL_READ_FRAMEBUFFER : (p_target == Target::FBT_DRAW ? GL_DRAW_FRAMEBUFFER : GL_FRAMEBUFFER);
	GLCLEARERROR();
	if(p_target == Target::FBT_BOTH)
	{
		glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, attachment_target, GL_RENDERBUFFER, render_buffer_id);
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, attachment_target, GL_RENDERBUFFER, render_buffer_id);
	}
	else
		glFramebufferRenderbuffer(fb_target, attachment_target, GL_RENDERBUFFER, render_buffer_id);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return glGetError() == GL_NO_ERROR;
}
bool FrameBuffer::attachTexture2D(axl::glfl::GLenum attachment_target, Texture2D* texture, Target p_target)
{
	using namespace GL;
	if(!this->bind(Target::FBT_BOTH)) return false;
	axl::glfl::GLuint texture_id = texture && texture->isValid() ? texture->getId() : 0;
	axl::glfl::GLenum fb_target = p_target == Target::FBT_READ ? GL_READ_FRAMEBUFFER : (p_target == Target::FBT_DRAW ? GL_DRAW_FRAMEBUFFER : GL_FRAMEBUFFER);
	GLCLEARERROR();if(p_target == Target::FBT_BOTH)
	{
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, attachment_target, GL_TEXTURE_2D, texture_id, 0);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment_target, GL_TEXTURE_2D, texture_id, 0);
	}
	else
		glFramebufferTexture2D(fb_target, attachment_target, GL_TEXTURE_2D, texture_id, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return glGetError() == GL_NO_ERROR;
}
bool FrameBuffer::blit(const FrameBuffer* draw_framebuffer, axl::glfl::GLint srcX0, axl::glfl::GLint srcY0, axl::glfl::GLint srcX1, axl::glfl::GLint srcY1, axl::glfl::GLint dstX0, axl::glfl::GLint dstY0, axl::glfl::GLint dstX1, axl::glfl::GLint dstY1, axl::glfl::GLbitfield mask, axl::glfl::GLenum filter) const
{
	using namespace GL;
	if(!this->isComplete()) return false;
	if(!draw_framebuffer) glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	else if(!draw_framebuffer->isComplete()) return false;
	else
	{
		GLCLEARERROR();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw_framebuffer->getId());
		if(glGetError() != GL_NO_ERROR) return false;
	}
	GLCLEARERROR();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->getId());
	glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return glGetError() == GL_NO_ERROR;
}

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
