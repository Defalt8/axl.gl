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

bool Program::icreate()
{
	using namespace GL;
	if(!GL_VERSION_3_0 || this->isValid() || !(this->ctx_context && this->ctx_context->makeCurrent())) return false;
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

bool Program::idestroy()
{
	using namespace GL;
	if(this->isValid() && this->ctx_context->makeCurrent())
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
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return false;
	GLCLEARERROR();
	glLinkProgram(this->program_id);
	GLint status;
	glGetProgramiv(this->program_id, GL_LINK_STATUS, &status);
	return glGetError() == GL_NO_ERROR && status == GL_TRUE;
}

bool Program::isLinked() const
{
	using namespace GL;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return false;
	GLCLEARERROR();
	GLint status;
	glGetProgramiv(this->program_id, GL_LINK_STATUS, &status);
	return glGetError() == GL_NO_ERROR && status == GL_TRUE;
}

axl::util::String Program::getInfoLog() const
{
	using namespace GL;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return axl::util::String();
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
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return false;
	GLCLEARERROR();
	glUseProgram(this->program_id);
	return glGetError() == GL_NO_ERROR;
}

bool Program::useNull() const
{
	using namespace GL;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return false;
	GLCLEARERROR();
	glUseProgram(0);
	return glGetError() == GL_NO_ERROR;
}

axl::glfl::GLint Program::getUniformLocation(const axl::util::String& uniform_name) const
{
	using namespace GL;
	if(uniform_name.length() == 0 || !this->isLinked()) return -1;
	GLCLEARERROR();
	axl::glfl::GLint location = glGetUniformLocation(this->program_id, (const axl::glfl::GLchar*)uniform_name.cstr());
	if(glGetError() != GL_NO_ERROR) return -1;
	return location;
}

bool Program::setUniformMat4f(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix_ptr, axl::glfl::GLboolean transpose)
{
	using namespace GL;
	if(location == -1 || !matrix_ptr || !this->use()) return false;
	GLCLEARERROR();
	if(glGetError() != GL_NO_ERROR) return false;
	glUniformMatrix4fv(location, 1, transpose, matrix_ptr);
	GLenum gl_err = glGetError();
	glUseProgram(0);
	return gl_err == GL_NO_ERROR && glGetError() == GL_NO_ERROR;
}

bool Program::setUniformMat4d(axl::glfl::GLint location, const axl::glfl::GLdouble* matrix_ptr, axl::glfl::GLboolean transpose)
{
	using namespace GL;
	if(!GL::GL_VERSION_4_0 || !(this->ctx_context && this->ctx_context->config.major_version >= 4) || location == -1 || !matrix_ptr || !this->use()) return false;
	GLCLEARERROR();
	if(glGetError() != GL_NO_ERROR) return false;
	glUniformMatrix4dv(location, 1, transpose, matrix_ptr);
	GLenum gl_err = glGetError();
	glUseProgram(0);
	return gl_err == GL_NO_ERROR && glGetError() == GL_NO_ERROR;
}

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
