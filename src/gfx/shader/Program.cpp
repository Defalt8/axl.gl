#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/gfx/shader/Program.hpp>
#include <axl.glfl/gl.hpp>
#include <axl.glfl/glCoreARB.hpp>

#define GLCLEARERROR() while(axl::glfl::core::GL1::glGetError())

namespace GL {
using namespace axl::glfl;
using namespace axl::glfl::core::GL;
}

namespace axl {
namespace gl {
namespace gfx {

Program::Program(axl::gl::Context* ptr_context) :
	ContextObject(ptr_context),
	program_id(0)
{}

Program::~Program()
{
	this->destroy();
}

bool Program::iCreate()
{
	using namespace GL;
	if(!GL_VERSION_2_0 || !(this->ctx_context && this->ctx_context->config.major_version >= 2 && (this->ctx_context->isCurrent() || this->ctx_context->makeCurrent()))) return false;
	axl::glfl::GLuint tmp_id;
	GLCLEARERROR();
	tmp_id = glCreateProgram();
	if(tmp_id == 0) return false;
	if(glGetError() != GL_NO_ERROR)
	{
		glDeleteProgram(tmp_id);
		return false;
	}
	this->program_id = tmp_id;
	return true;
}

bool Program::iDestroy()
{
	using namespace GL;
	if(axl::gl::gfx::Program::isValid() && (this->ctx_context->isCurrent() || this->ctx_context->makeCurrent()))
	{
		GLCLEARERROR();
		glDeleteProgram(Program::program_id);
		if(glGetError() != GL_NO_ERROR) return false;
		this->program_id = 0;
		return true;
	}
	return false;
}

bool Program::isValid() const
{
	return this->ctx_context && this->ctx_context->isValid() && this->program_id != 0;
}

axl::glfl::GLuint Program::getId() const
{
	return this->program_id;
}

bool Program::link() const
{
	using namespace GL;
	if(!axl::gl::gfx::Program::isValid() || !(this->ctx_context->isCurrent() || this->ctx_context->makeCurrent())) return false;
	GLCLEARERROR();
	glLinkProgram(this->program_id);
	GLint status;
	glGetProgramiv(this->program_id, GL_LINK_STATUS, &status);
	return glGetError() == GL_NO_ERROR && status == GL_TRUE;
}

bool Program::isLinked() const
{
	using namespace GL;
	if(!axl::gl::gfx::Program::isValid() || !(this->ctx_context->isCurrent() || this->ctx_context->makeCurrent())) return false;
	GLCLEARERROR();
	GLint status;
	glGetProgramiv(this->program_id, GL_LINK_STATUS, &status);
	return glGetError() == GL_NO_ERROR && status == GL_TRUE;
}

axl::util::String Program::getInfoLog() const
{
	using namespace GL;
	if(!axl::gl::gfx::Program::isValid() || !(this->ctx_context->isCurrent() || this->ctx_context->makeCurrent())) return axl::util::String();
	GLCLEARERROR();
	GLint info_log_len;
	glGetProgramiv(this->program_id, GL_INFO_LOG_LENGTH, &info_log_len);
	if(glGetError() != GL_NO_ERROR || info_log_len <= 0) return axl::util::String();
	axl::util::String info_log(info_log_len);
	if(info_log.size() == 0) return axl::util::String();
	glGetProgramInfoLog(this->program_id, (GLsizei)info_log_len, (GLsizei*)0, info_log.str());
	if(glGetError() != GL_NO_ERROR) return axl::util::String();
	info_log.length(true);
	return info_log;
}

bool Program::use() const
{
	using namespace GL;
	if(!axl::gl::gfx::Program::isValid() || !(this->ctx_context->isCurrent() || this->ctx_context->makeCurrent())) return false;
	GLCLEARERROR();
	glUseProgram(this->program_id);
	return glGetError() == GL_NO_ERROR;
}

bool Program::unuse() const
{
	using namespace GL;
	if(!axl::gl::gfx::Program::isValid() || !(this->ctx_context->isCurrent() || this->ctx_context->makeCurrent())) return false;
	GLCLEARERROR();
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

axl::glfl::GLint Program::getAttributeLocation(const axl::util::String& attribute_name) const
{
	using namespace GL;
	if(attribute_name.length() == 0 || !this->isLinked() || !(this->ctx_context->isCurrent() || this->ctx_context->makeCurrent())) return -1;
	GLCLEARERROR();
	axl::glfl::GLint location = glGetAttribLocation(this->program_id, (const axl::glfl::GLchar*)attribute_name.cstr());
	if(glGetError() != GL_NO_ERROR) return -1;
	return location;
}

axl::glfl::GLint Program::getUniformLocation(const axl::util::String& uniform_name) const
{
	using namespace GL;
	if(uniform_name.length() == 0 || !this->isLinked() || !(this->ctx_context->isCurrent() || this->ctx_context->makeCurrent())) return -1;
	GLCLEARERROR();
	axl::glfl::GLint location = glGetUniformLocation(this->program_id, (const axl::glfl::GLchar*)uniform_name.cstr());
	if(glGetError() != GL_NO_ERROR) return -1;
	return location;
}

bool Program::getUniformfv(axl::glfl::GLint location, axl::glfl::GLfloat* out_ptr) const
{
	using namespace GL;
	if(location == -1 || !this->isLinked() || !(this->ctx_context->isCurrent() || this->ctx_context->makeCurrent())) return false;
	GLCLEARERROR();
	glGetUniformfv(this->program_id, location, out_ptr);
	return glGetError() == GL_NO_ERROR;
}

bool Program::getUniformiv(axl::glfl::GLint location, axl::glfl::GLint* out_ptr) const
{
	using namespace GL;
	if(location == -1 || !this->isLinked() || !(this->ctx_context->isCurrent() || this->ctx_context->makeCurrent())) return false;
	GLCLEARERROR();
	glGetUniformiv(this->program_id, location, out_ptr);
	return glGetError() == GL_NO_ERROR;
}

bool Program::getUniformuiv(axl::glfl::GLint location, axl::glfl::GLuint* out_ptr) const
{
	using namespace GL;
	if(location == -1 || !this->isLinked() || !(this->ctx_context->isCurrent() || this->ctx_context->makeCurrent())) return false;
	GLCLEARERROR();
	glGetUniformuiv(this->program_id, location, out_ptr);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform1f(axl::glfl::GLint location, const axl::glfl::GLfloat value) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !this->use()) return false;
	GLCLEARERROR();
	glUniform1f(location, value);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform2f(axl::glfl::GLint location, axl::glfl::GLfloat v0, axl::glfl::GLfloat v1) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !this->use()) return false;
	GLCLEARERROR();
	glUniform2f(location, v0, v1);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform3f(axl::glfl::GLint location, axl::glfl::GLfloat v0, axl::glfl::GLfloat v1, axl::glfl::GLfloat v2) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !this->use()) return false;
	GLCLEARERROR();
	glUniform3f(location, v0, v1, v2);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform4f(axl::glfl::GLint location, axl::glfl::GLfloat v0, axl::glfl::GLfloat v1, axl::glfl::GLfloat v2, axl::glfl::GLfloat v3) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !this->use()) return false;
	GLCLEARERROR();
	glUniform4f(location, v0, v1, v2, v3);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform1i(axl::glfl::GLint location, const axl::glfl::GLint value) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !this->use()) return false;
	GLCLEARERROR();
	glUniform1i(location, value);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform2i(axl::glfl::GLint location, axl::glfl::GLint v0, axl::glfl::GLint v1) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !this->use()) return false;
	GLCLEARERROR();
	glUniform2i(location, v0, v1);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform3i(axl::glfl::GLint location, axl::glfl::GLint v0, axl::glfl::GLint v1, axl::glfl::GLint v2) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !this->use()) return false;
	GLCLEARERROR();
	glUniform3i(location, v0, v1, v2);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform4i(axl::glfl::GLint location, axl::glfl::GLint v0, axl::glfl::GLint v1, axl::glfl::GLint v2, axl::glfl::GLint v3) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !this->use()) return false;
	GLCLEARERROR();
	glUniform4i(location, v0, v1, v2, v3);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform1ui(axl::glfl::GLint location, const axl::glfl::GLuint value) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !this->use()) return false;
	GLCLEARERROR();
	glUniform1ui(location, value);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform2ui(axl::glfl::GLint location, axl::glfl::GLuint v0, axl::glfl::GLuint v1) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !this->use()) return false;
	GLCLEARERROR();
	glUniform2ui(location, v0, v1);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform3ui(axl::glfl::GLint location, axl::glfl::GLuint v0, axl::glfl::GLuint v1, axl::glfl::GLuint v2) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !this->use()) return false;
	GLCLEARERROR();
	glUniform3ui(location, v0, v1, v2);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform4ui(axl::glfl::GLint location, axl::glfl::GLuint v0, axl::glfl::GLuint v1, axl::glfl::GLuint v2, axl::glfl::GLuint v3) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !this->use()) return false;
	GLCLEARERROR();
	glUniform4ui(location, v0, v1, v2, v3);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniformfv(axl::glfl::GLint location, const axl::glfl::GLfloat* values_ptr, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !values_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniform1fv(location, count, values_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform2fv(axl::glfl::GLint location, const axl::glfl::GLfloat* vector2_ptr, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !vector2_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniform2fv(location, count, vector2_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform3fv(axl::glfl::GLint location, const axl::glfl::GLfloat* vector3_ptr, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !vector3_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniform3fv(location, count, vector3_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform4fv(axl::glfl::GLint location, const axl::glfl::GLfloat* vector4_ptr, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !vector4_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniform4fv(location, count, vector4_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniformiv(axl::glfl::GLint location, const axl::glfl::GLint* values_ptr, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !values_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniform1iv(location, count, values_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform2iv(axl::glfl::GLint location, const axl::glfl::GLint* vector2_ptr, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !vector2_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniform2iv(location, count, vector2_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform3iv(axl::glfl::GLint location, const axl::glfl::GLint* vector3_ptr, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !vector3_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniform3iv(location, count, vector3_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform4iv(axl::glfl::GLint location, const axl::glfl::GLint* vector4_ptr, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !vector4_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniform4iv(location, count, vector4_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniformuiv(axl::glfl::GLint location, const axl::glfl::GLuint* values_ptr, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !values_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniform1uiv(location, count, values_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform2uiv(axl::glfl::GLint location, const axl::glfl::GLuint* vector2_ptr, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !vector2_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniform2uiv(location, count, vector2_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform3uiv(axl::glfl::GLint location, const axl::glfl::GLuint* vector3_ptr, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !vector3_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniform3uiv(location, count, vector3_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniform4uiv(axl::glfl::GLint location, const axl::glfl::GLuint* vector4_ptr, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !vector4_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniform4uiv(location, count, vector4_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}


bool Program::setUniformMat2fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix2_ptr, axl::glfl::GLboolean transpose, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !matrix2_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniformMatrix2fv(location, count, transpose, matrix2_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniformMat3fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix3_ptr, axl::glfl::GLboolean transpose, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !matrix3_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniformMatrix3fv(location, count, transpose, matrix3_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniformMat4fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix4_ptr, axl::glfl::GLboolean transpose, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !matrix4_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniformMatrix4fv(location, count, transpose, matrix4_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniformMat2x3fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix2x3_ptr, axl::glfl::GLboolean transpose, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !matrix2x3_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniformMatrix2x3fv(location, count, transpose, matrix2x3_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniformMat3x2fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix3x2_ptr, axl::glfl::GLboolean transpose, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !matrix3x2_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniformMatrix3x2fv(location, count, transpose, matrix3x2_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniformMat2x4fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix2x4_ptr, axl::glfl::GLboolean transpose, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !matrix2x4_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniformMatrix2x4fv(location, count, transpose, matrix2x4_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniformMat4x2fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix4x2_ptr, axl::glfl::GLboolean transpose, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !matrix4x2_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniformMatrix4x2fv(location, count, transpose, matrix4x2_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniformMat3x4fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix3x4_ptr, axl::glfl::GLboolean transpose, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !matrix3x4_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniformMatrix3x4fv(location, count, transpose, matrix3x4_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniformMat4x3fv(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix4x3_ptr, axl::glfl::GLboolean transpose, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(location == -1 || !this->ctx_context->isCurrent() || !matrix4x3_ptr || !this->use()) return false;
	GLCLEARERROR();
	glUniformMatrix4x3fv(location, count, transpose, matrix4x3_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

bool Program::setUniformMat4dv(axl::glfl::GLint location, const axl::glfl::GLdouble* matrix_ptr, axl::glfl::GLboolean transpose, axl::glfl::GLsizei count) const
{
	using namespace GL;
	if(!GL::GL_VERSION_4_0 || !(this->ctx_context && this->ctx_context->config.major_version >= 4) || location == -1 || !matrix_ptr || !this->ctx_context->isCurrent() || !this->use()) return false;
	GLCLEARERROR();
	glUniformMatrix4dv(location, count, transpose, matrix_ptr);
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}


} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
