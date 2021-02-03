#pragma once
#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/gfx/buffer/FrameBuffer.hpp>
#include <axl.glfl/glCoreARB.hpp>
#include <stdio.h>

#define GLCLEARERROR() while(axl::glfl::core::GL1::glGetError())

namespace GL {
using namespace axl::glfl::core::GL1;
using namespace axl::glfl::core::GL2;
using namespace axl::glfl::core::GL3;
}

namespace axl {
namespace gl {
namespace gfx {

FrameBuffer::Binding::Binding() :
	type(),
	pointer()
{}

FrameBuffer::Binding::Binding(FrameBuffer::Binding::Type p_type, void* p_ptr) :
	type(p_type),
	pointer()
{
	switch(p_type)
	{
		case Type::BT_TEXTURE: *((Texture**)&pointer) = (Texture*)p_ptr; break; 
		case Type::BT_RENDER_BUFFER: *((RenderBuffer**)&pointer) = (RenderBuffer*)p_ptr; break;
		default: break;
	}
}

FrameBuffer::FrameBuffer(axl::gl::Context* ptr_context) :
	ContextObject(ptr_context),
	fb_id(-1),
	fb_bindings()
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
	this->fb_id = tmp_id;
	printf("> FrameBuffer[%d].create()\n", this->fb_id);
	return true;
}
bool FrameBuffer::idestroy()
{
	using namespace GL;
	while(!this->fb_bindings.isEmpty())
	{
		Binding binding = this->fb_bindings.removeFirst();
		switch(binding.type)
		{
			case Binding::Type::BT_TEXTURE: binding.pointer.texture->destroy(); break;
			case Binding::Type::BT_RENDER_BUFFER: binding.pointer.render_buffer->destroy(); break;
			default: break;
		}
	}
	if(this->unbind())
	{
		GLCLEARERROR();
		glDeleteFramebuffers(1, &this->fb_id);
		if(glGetError() != GL_NO_ERROR) return false;
		printf("> FrameBuffer[%d].destroy()\n", this->fb_id);
		this->fb_id = -1;
		return true;
	}
	return false;
}
bool FrameBuffer::isValid() const
{
	return this->ctx_context && this->ctx_context->isValid() && this->fb_id != -1;
}
axl::glfl::GLuint FrameBuffer::getId() const
{
	return this->fb_id;
}
bool FrameBuffer::bind(Target p_target) const
{
	using namespace GL;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return false;
	axl::glfl::GLenum fb_target = p_target == Target::FBT_READ ? GL_READ_FRAMEBUFFER : (p_target == Target::FBT_DRAW ? GL_DRAW_FRAMEBUFFER : GL_FRAMEBUFFER);
	GLCLEARERROR();
	glBindFramebuffer(fb_target, this->fb_id);
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
	if(glGetError() != GL_NO_ERROR) return false;
	this->fb_bindings.insertLast(Binding(Binding::Type::BT_RENDER_BUFFER, render_buffer));
	return true;
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
	if(glGetError() != GL_NO_ERROR) return false;
	this->fb_bindings.insertLast(Binding(Binding::Type::BT_TEXTURE, texture));
	return true;
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
