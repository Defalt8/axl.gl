#pragma once
#include "../../lib.hpp"
#include "Texture.hpp"
#include <axl.glfl/gl.hpp>

namespace axl {
namespace gl {
namespace gfx {

class AXLGLCXXAPI Texture2D : public Texture
{
	public:
		Texture2D(axl::gl::Context* ptr_context = (axl::gl::Context*)0);
		virtual ~Texture2D();
		Type type() const;
		bool icreate();
		bool idestroy();
		bool isValid() const;
		axl::glfl::GLuint getId() const;
		bool bind() const;
		bool unbind() const;
		bool setParami(axl::glfl::GLenum tex_param, axl::glfl::GLint value);
		bool setParamf(axl::glfl::GLenum tex_param, axl::glfl::GLfloat value);
		bool setParamiv(axl::glfl::GLenum tex_param, const axl::glfl::GLint* value);
		bool setParamfv(axl::glfl::GLenum tex_param, const axl::glfl::GLfloat* value);
		bool getParamiv(axl::glfl::GLenum tex_param, axl::glfl::GLint* value_ptr);
		bool getParamfv(axl::glfl::GLenum tex_param, axl::glfl::GLfloat* value_ptr);
		bool getLevelParamiv(axl::glfl::GLint level, axl::glfl::GLenum tex_param, axl::glfl::GLint* value_ptr);
		bool getLevelParamfv(axl::glfl::GLint level, axl::glfl::GLenum tex_param, axl::glfl::GLfloat* value_ptr);
		bool allocate(axl::glfl::GLsizei width, axl::glfl::GLsizei height, axl::glfl::GLint internal_format, axl::glfl::GLint border = 0);
		bool setImage(axl::glfl::GLint level, axl::glfl::GLint x_offset, axl::glfl::GLint y_offset, axl::glfl::GLsizei width, axl::glfl::GLsizei height, axl::glfl::GLenum format, axl::glfl::GLenum data_type, const axl::glfl::GLvoid* in_image_data, axl::glfl::GLint pixel_alignment = 4);
		bool getImage(axl::glfl::GLint level, axl::glfl::GLenum format, axl::glfl::GLenum data_type, axl::glfl::GLvoid* out_image_data, axl::glfl::GLint pixel_alignment = 4);
		bool generateMipmaps();
		axl::math::Vec2i getSize(axl::glfl::GLint level = 0) const;
};

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
