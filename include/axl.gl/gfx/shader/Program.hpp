#pragma once
#include "../../lib.hpp"
#include "../../ContextObject.hpp"
#include "Shader.hpp"
#include <axl.glfl/gl.hpp>
#include <axl.util/String.hpp>

namespace axl {
namespace gl {

class AXLGLCXXAPI Context;

namespace gfx {

class AXLGLCXXAPI Program : public ContextObject
{
	public:
		AXLGL_ENUM_CLASS Type { TT_1D = 1, TT_2D = 2, TT_3D = 3 };
	public:
		Program(axl::gl::Context* ptr_context = (axl::gl::Context*)0);
		virtual ~Program();
		virtual bool icreate();
		virtual bool idestroy();
		virtual bool isValid() const;
		axl::glfl::GLuint getId() const;
		bool link() const;
		bool isLinked() const;
		axl::util::String getInfoLog() const;
		bool use() const;
		bool unuse() const;
		axl::glfl::GLint getUniformLocation(const axl::util::String& uniform_name) const;
		
		bool getUniformfv(axl::glfl::GLint location, axl::glfl::GLfloat* out_ptr) const;
		bool getUniformiv(axl::glfl::GLint location, axl::glfl::GLint* out_ptr) const;
		bool getUniformuiv(axl::glfl::GLint location, axl::glfl::GLuint* out_ptr) const;

		bool setUniform1f(axl::glfl::GLint location, const axl::glfl::GLfloat value) const;
		bool setUniform2f(axl::glfl::GLint location, axl::glfl::GLfloat v0, axl::glfl::GLfloat v1) const;
		bool setUniform3f(axl::glfl::GLint location, axl::glfl::GLfloat v0, axl::glfl::GLfloat v1, axl::glfl::GLfloat v2) const;
		bool setUniform4f(axl::glfl::GLint location, axl::glfl::GLfloat v0, axl::glfl::GLfloat v1, axl::glfl::GLfloat v2, axl::glfl::GLfloat v3) const;
		bool setUniform1i(axl::glfl::GLint location, const axl::glfl::GLint value) const;
		bool setUniform2i(axl::glfl::GLint location, axl::glfl::GLint v0, axl::glfl::GLint v1) const;
		bool setUniform3i(axl::glfl::GLint location, axl::glfl::GLint v0, axl::glfl::GLint v1, axl::glfl::GLint v2) const;
		bool setUniform4i(axl::glfl::GLint location, axl::glfl::GLint v0, axl::glfl::GLint v1, axl::glfl::GLint v2, axl::glfl::GLint v3) const;
		bool setUniform1ui(axl::glfl::GLint location, const axl::glfl::GLuint value) const;
		bool setUniform2ui(axl::glfl::GLint location, axl::glfl::GLuint v0, axl::glfl::GLuint v1) const;
		bool setUniform3ui(axl::glfl::GLint location, axl::glfl::GLuint v0, axl::glfl::GLuint v1, axl::glfl::GLuint v2) const;
		bool setUniform4ui(axl::glfl::GLint location, axl::glfl::GLuint v0, axl::glfl::GLuint v1, axl::glfl::GLuint v2, axl::glfl::GLuint v3) const;
		bool setUniformfv(axl::glfl::GLint location, const axl::glfl::GLfloat* values_ptr, axl::glfl::GLsizei count = 1) const;
		bool setUniform2fv(axl::glfl::GLint location, const axl::glfl::GLfloat* vector2_ptr, axl::glfl::GLsizei count = 1) const;
		bool setUniform3fv(axl::glfl::GLint location, const axl::glfl::GLfloat* vector3_ptr, axl::glfl::GLsizei count = 1) const;
		bool setUniform4fv(axl::glfl::GLint location, const axl::glfl::GLfloat* vector4_ptr, axl::glfl::GLsizei count = 1) const;
		bool setUniformiv(axl::glfl::GLint location, const axl::glfl::GLint* values_ptr, axl::glfl::GLsizei count = 1) const;
		bool setUniform2iv(axl::glfl::GLint location, const axl::glfl::GLint* vector2_ptr, axl::glfl::GLsizei count = 1) const;
		bool setUniform3iv(axl::glfl::GLint location, const axl::glfl::GLint* vector3_ptr, axl::glfl::GLsizei count = 1) const;
		bool setUniform4iv(axl::glfl::GLint location, const axl::glfl::GLint* vector4_ptr, axl::glfl::GLsizei count = 1) const;
		bool setUniformuiv(axl::glfl::GLint location, const axl::glfl::GLuint* values_ptr, axl::glfl::GLsizei count = 1) const;
		bool setUniform2uiv(axl::glfl::GLint location, const axl::glfl::GLuint* vector2_ptr, axl::glfl::GLsizei count = 1) const;
		bool setUniform3uiv(axl::glfl::GLint location, const axl::glfl::GLuint* vector3_ptr, axl::glfl::GLsizei count = 1) const;
		bool setUniform4uiv(axl::glfl::GLint location, const axl::glfl::GLuint* vector4_ptr, axl::glfl::GLsizei count = 1) const;
		bool setUniformMat2fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix2_ptr, axl::glfl::GLboolean transpose = 0, axl::glfl::GLsizei count = 1) const;
		bool setUniformMat3fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix3_ptr, axl::glfl::GLboolean transpose = 0, axl::glfl::GLsizei count = 1) const;
		bool setUniformMat4fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix4_ptr, axl::glfl::GLboolean transpose = 0, axl::glfl::GLsizei count = 1) const;
		bool setUniformMat2x3fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix2x3_ptr, axl::glfl::GLboolean transpose = 0, axl::glfl::GLsizei count = 1) const;
		bool setUniformMat3x2fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix3x2_ptr, axl::glfl::GLboolean transpose = 0, axl::glfl::GLsizei count = 1) const;
		bool setUniformMat2x4fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix2x4_ptr, axl::glfl::GLboolean transpose = 0, axl::glfl::GLsizei count = 1) const;
		bool setUniformMat4x2fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix4x2_ptr, axl::glfl::GLboolean transpose = 0, axl::glfl::GLsizei count = 1) const;
		bool setUniformMat3x4fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix3x4_ptr, axl::glfl::GLboolean transpose = 0, axl::glfl::GLsizei count = 1) const;
		bool setUniformMat4x3fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix4x3_ptr, axl::glfl::GLboolean transpose = 0, axl::glfl::GLsizei count = 1) const;
		bool setUniformMat4dv(axl::glfl::GLint location, const axl::glfl::GLdouble* matrix4_ptr, axl::glfl::GLboolean transpose = 0, axl::glfl::GLsizei count = 1) const;
	protected:
		axl::glfl::GLuint program_id;
		axl::glfl::GLenum program_type;
};

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
