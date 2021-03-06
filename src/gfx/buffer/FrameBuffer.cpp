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

FrameBufferBinding::FrameBufferBinding() :
	type(),
	pointer()
{}

FrameBufferBinding::FrameBufferBinding(FrameBufferBinding::Type p_type, axl::glfl::GLenum p_attachment_target, void* p_ptr) :
	type(p_type),
	attachment_target(p_attachment_target),
	pointer()
{
	switch(p_type)
	{
		case Type::BT_TEXTURE: *((Texture**)&pointer) = (Texture*)p_ptr; break; 
		case Type::BT_RENDER_BUFFER: *((RenderBuffer**)&pointer) = (RenderBuffer*)p_ptr; break;
		default: break;
	}
}

bool FrameBufferBinding::operator==(const FrameBufferBinding& binding) const
{
	if(this->type != binding.type || this->attachment_target != binding.attachment_target) return false;
	switch(this->type)
	{
		case Type::BT_TEXTURE: return this->pointer.render_buffer == binding.pointer.render_buffer;
		case Type::BT_RENDER_BUFFER: return this->pointer.texture == binding.pointer.texture;
	}
	return false;
}

////////

FrameBuffer::FrameBuffer(axl::gl::Context* ptr_context) :
	ContextObject(ptr_context),
	fb_id(0),
	fb_bindings()
{}
FrameBuffer::~FrameBuffer()
{
	this->destroy();
}
bool FrameBuffer::iCreate()
{
	using namespace GL;
	if(!GL_VERSION_3_0 || this->isValid() || !(this->ctx_context && (this->ctx_context->isCurrent() || this->ctx_context->makeCurrent()))) return false;
	axl::glfl::GLuint tmp_id;
	GLCLEARERROR();
	glGenFramebuffers(1, &tmp_id);
	if(tmp_id == 0) return false;
	if(glGetError() != GL_NO_ERROR)
	{
		glDeleteFramebuffers(1, &tmp_id);
		return false;
	}
	this->fb_id = tmp_id;
	return true;
}
bool FrameBuffer::iDestroy()
{
	using namespace GL;
	if(this->unbind())
	{
		while(!this->fb_bindings.isEmpty())
		{
			FrameBufferBinding binding = this->fb_bindings.removeFirst();
			glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, binding.attachment_target, GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, binding.attachment_target, GL_RENDERBUFFER, 0);
		}
		GLCLEARERROR();
		glDeleteFramebuffers(1, &this->fb_id);
		if(glGetError() != GL_NO_ERROR) return false;
		this->fb_id = 0;
		return true;
	}
	return false;
}
bool FrameBuffer::isValid() const
{
	return this->ctx_context && this->ctx_context->isValid() && this->fb_id != 0;
}
axl::glfl::GLuint FrameBuffer::getId() const
{
	return this->fb_id;
}
bool FrameBuffer::bind(Target p_target) const
{
	using namespace GL;
	if(!this->isValid() || !(this->ctx_context->isCurrent() || this->ctx_context->makeCurrent())) return false;
	axl::glfl::GLenum fb_target = p_target == Target::FBT_READ ? GL_READ_FRAMEBUFFER : (p_target == Target::FBT_DRAW ? GL_DRAW_FRAMEBUFFER : GL_FRAMEBUFFER);
	GLCLEARERROR();
	glBindFramebuffer(fb_target, this->fb_id);
	return glGetError() == GL_NO_ERROR;
}
bool FrameBuffer::unbind(Target p_target) const
{
	using namespace GL;
	if(!this->isValid() || !(this->ctx_context->isCurrent() || this->ctx_context->makeCurrent())) return false;
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
	this->fb_bindings.insertFirst(FrameBufferBinding(FrameBufferBinding::Type::BT_RENDER_BUFFER, attachment_target, render_buffer));
	return true;
}
bool FrameBuffer::attachTexture2D(axl::glfl::GLenum attachment_target, Texture2D* texture, Target p_target)
{
	using namespace GL;
	if(!this->isValid()) return false;
	axl::glfl::GLuint texture_id = texture && texture->isValid() ? texture->getId() : 0;
	axl::glfl::GLenum fb_target = p_target == Target::FBT_READ ? GL_READ_FRAMEBUFFER : (p_target == Target::FBT_DRAW ? GL_DRAW_FRAMEBUFFER : GL_FRAMEBUFFER);
	GLCLEARERROR();
	glBindFramebuffer(fb_target, this->fb_id);
	if(p_target == Target::FBT_BOTH)
	{
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, attachment_target, GL_TEXTURE_2D, texture_id, 0);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment_target, GL_TEXTURE_2D, texture_id, 0);
	}
	else
	{
		glFramebufferTexture2D(fb_target, attachment_target, GL_TEXTURE_2D, texture_id, 0);
	}
	glBindFramebuffer(fb_target, 0);
	if(glGetError() != GL_NO_ERROR) return false;
	this->fb_bindings.insertFirst(FrameBufferBinding(FrameBufferBinding::Type::BT_TEXTURE, attachment_target, texture));
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
