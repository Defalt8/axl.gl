#pragma once
#include "../../lib.hpp"
#include "../../ContextObject.hpp"
#include <axl.glfl/gl.hpp>
#include <axl.math/vec/Vec2i.hpp>

namespace axl {
namespace gl {

class AXLGLCXXAPI Context;

namespace gfx {

class AXLGLCXXAPI Texture : public ContextObject
{
	public:
		AXLGL_ENUM_CLASS Type { TT_1D = 1, TT_2D = 2, TT_3D = 3 };
	public:
		Texture(axl::gl::Context* ptr_context = (axl::gl::Context*)0);
		virtual ~Texture();
		virtual Type type() const = 0;
	protected:
		virtual bool icreate() = 0;
		virtual bool idestroy() = 0;
	public:
		virtual bool isValid() const = 0;
		virtual axl::glfl::GLuint getId() const = 0;
		virtual bool bind() const = 0;
		virtual bool unbind() const = 0;
		virtual bool setParami(axl::glfl::GLenum tex_param, axl::glfl::GLint value) = 0;
		virtual bool setParamf(axl::glfl::GLenum tex_param, axl::glfl::GLfloat value) = 0;
		virtual bool setParamiv(axl::glfl::GLenum tex_param, const axl::glfl::GLint* value) = 0;
		virtual bool setParamfv(axl::glfl::GLenum tex_param, const axl::glfl::GLfloat* value) = 0;
		virtual bool getParamiv(axl::glfl::GLenum tex_param, axl::glfl::GLint* value) = 0;
		virtual bool getParamfv(axl::glfl::GLenum tex_param, axl::glfl::GLfloat* value) = 0;
		virtual bool getLevelParamiv(axl::glfl::GLint level, axl::glfl::GLenum tex_param, axl::glfl::GLint* value) = 0;
		virtual bool getLevelParamfv(axl::glfl::GLint level, axl::glfl::GLenum tex_param, axl::glfl::GLfloat* value) = 0;
	protected:
		axl::glfl::GLuint txr_id;
};

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
