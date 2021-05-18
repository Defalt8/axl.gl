#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/gfx/shader/Shader.hpp>
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

Shader::Shader(axl::gl::Context* ptr_context, axl::glfl::GLenum p_shader_type) :
	ContextObject(ptr_context),
	shader_id(0),
	shader_type(p_shader_type)
{}

Shader::~Shader()
{
	this->destroy();
}

bool Shader::iCreate()
{
	using namespace GL;
	if(!GL_VERSION_3_0 || this->isValid() || !(this->ctx_context && this->ctx_context->makeCurrent())) return false;
	axl::glfl::GLuint tmp_id;
	GLCLEARERROR();
	tmp_id = glCreateShader(Shader::shader_type);
	if(tmp_id == 0) return false;
	if(glGetError() != GL_NO_ERROR)
	{
		glDeleteShader(tmp_id);
		return false;
	}
	this->shader_id = tmp_id;
	return true;
}

bool Shader::iDestroy()
{
	using namespace GL;
	if(this->isValid() && this->ctx_context->makeCurrent())
	{
		GLCLEARERROR();
		glDeleteShader(Shader::shader_id);
		if(glGetError() != GL_NO_ERROR) return false;
		this->shader_id = 0;
		return true;
	}
	return false;
}

bool Shader::isValid() const
{
	return this->ctx_context && this->ctx_context->isValid() && this->shader_id != 0;
}

axl::glfl::GLuint Shader::getId() const
{
	return this->shader_id;
}

axl::glfl::GLenum Shader::type() const
{
	return this->shader_type;
}


bool Shader::setSource(const axl::util::String& shader_code)
{
	using namespace GL;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return false;
	GLCLEARERROR();
	const GLchar* code = shader_code.cstr();
	GLint length = shader_code.length();
	glShaderSource(this->shader_id, 1, (GLchar*const*)&code, &length);
	return glGetError() == GL_NO_ERROR;
}

axl::util::String Shader::getSource() const
{
	using namespace GL;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return axl::util::String();
	GLCLEARERROR();
	GLint source_length;
	glGetShaderiv(this->shader_id, GL_SHADER_SOURCE_LENGTH, &source_length);
	if(source_length <= 0) return axl::util::String();
	axl::util::String source(source_length);
	if(source.size() == 0) return axl::util::String();
	glGetShaderSource(this->shader_id, source_length, (GLsizei*)0, source.str());
	if(glGetError() != GL_NO_ERROR) return axl::util::String();
	source.length(true);
	return source;
}

bool Shader::compile() const
{
	using namespace GL;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return false;
	GLCLEARERROR();
	glCompileShader(this->shader_id);
	GLint status;
	glGetShaderiv(this->shader_id, GL_COMPILE_STATUS, &status);
	return glGetError() == GL_NO_ERROR && status == GL_TRUE;
}

bool Shader::isCompiled() const
{
	using namespace GL;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return false;
	GLCLEARERROR();
	GLint status;
	glGetShaderiv(this->shader_id, GL_COMPILE_STATUS, &status);
	return glGetError() == GL_NO_ERROR && status == GL_TRUE;
}

axl::util::String Shader::getInfoLog() const
{
	using namespace GL;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return axl::util::String();
	GLCLEARERROR();
	GLint info_log_len;
	glGetShaderiv(this->shader_id, GL_INFO_LOG_LENGTH, &info_log_len);
	if(glGetError() != GL_NO_ERROR || info_log_len <= 0) return axl::util::String();
	axl::util::String info_log(info_log_len);
	if(info_log.size() == 0) return axl::util::String();
	glGetShaderInfoLog(this->shader_id, (GLsizei)info_log_len, (GLsizei*)0, info_log.str());
	if(glGetError() != GL_NO_ERROR) return axl::util::String();
	info_log.length(true);
	return info_log;
}

bool Shader::attach(const Program& program) const
{
	using namespace GL;
	if(!this->isValid() || !program.isValid() || !this->ctx_context->makeCurrent()) return false;
	GLCLEARERROR();
	glAttachShader(program.getId(), this->shader_id);
	return glGetError() == GL_NO_ERROR;
}
bool Shader::detach(const Program& program) const
{
	using namespace GL;
	if(!this->isValid() || !program.isValid() || !this->ctx_context->makeCurrent()) return false;
	GLCLEARERROR();
	glDetachShader(program.getId(), this->shader_id);
	return glGetError() == GL_NO_ERROR;
}

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
