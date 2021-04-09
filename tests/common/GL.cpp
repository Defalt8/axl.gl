#include <cstdio>
#include <axl.util/File.hpp>
#include "GL.hpp"

namespace GL {

axl::glfl::GLenum _gl_error;

bool loadVFShaders(axl::gl::gfx::Program& program, const axl::util::String& vertex_shader_file, const axl::util::String& fragment_shader_file)
{
	axl::gl::Context *context = program.getContext();
	if(!axl::util::File::exists(vertex_shader_file))
	{
		fprintf(stderr, "File does not exist! \"%s\"\n", vertex_shader_file.cstr());
		return false;
	}
	if(!axl::util::File::exists(fragment_shader_file))
	{
		fprintf(stderr, "File does not exist! \"%s\"\n", fragment_shader_file.cstr());
		return false;
	}
	if(!context || !context->isValid()) return false;
	// VERTEX_SHADER
	axl::gl::gfx::Shader vertex_shader(context, axl::glfl::core::GL::GL_VERTEX_SHADER);
	if(vertex_shader.create())
	{
		if(!vertex_shader.setSource(axl::util::File::getStringContent(vertex_shader_file).cstr()) ||
			!vertex_shader.compile()
		)
		{
			fprintf(stderr, "* Failed to compile vertex shader.\n");
			printf("VERTEX_SHADER_INFO_LOG:\n****\n%s****\n", vertex_shader.getInfoLog().cstr());
			return false;	
		}
	}
	// FRAGMENT_SHADER
	axl::gl::gfx::Shader fragment_shader(context, axl::glfl::core::GL::GL_FRAGMENT_SHADER);
	if(fragment_shader.create())
	{
		if(!fragment_shader.setSource(axl::util::File::getStringContent(fragment_shader_file).cstr()) ||
			!fragment_shader.compile()
		)
		{
			fprintf(stderr, "* Failed to compile fragment shader.\n");
			printf("FRAGMENT_SHADER_INFO_LOG:\n****\n%s****\n", fragment_shader.getInfoLog().cstr());
			return false;	
		}
	}
	// Program
	if(vertex_shader.isCompiled() && fragment_shader.isCompiled())
	{
		if(!program.isCreated()) program.create();
		if(!program.isValid()) return false;
		vertex_shader.attach(program);
		fragment_shader.attach(program);
		program.link();
		vertex_shader.detach(program);
		fragment_shader.detach(program);
		if(!program.isLinked())
		{
			printf("PROGRAM_INFO_LOG:\n****\n%s****\n", program.getInfoLog().cstr());
			return false;
		}
	}
	vertex_shader.destroy();
	fragment_shader.destroy();
	return true;
}

int Error(int err_code, const char* category, const char* err_msg, bool fatal)
{
	fprintf(stderr, "%s ERROR #%d: %s: %s\n", (fatal ? "FATAL" : ""), err_code, category, err_msg);
	if(fatal) axl::gl::Application::exit(err_code);
	return err_code;
}

void clearGLError()
{
	while (axl::glfl::core::GL::glGetError());
}

void logGLError(axl::glfl::GLenum error, const char* file, int line)
{
	using namespace axl::glfl::core::GL;
	const char* err_str;
	switch (error)
	{
	case GL_NO_ERROR: err_str = "NO_ERROR"; break;
	case GL_INVALID_ENUM: err_str = "INVALID_ENUM"; break;
	case GL_INVALID_VALUE: err_str = "INVALID_VALUE"; break;
	case GL_INVALID_OPERATION: err_str = "INVALID_OPERATION"; break;
	case GL_INVALID_FRAMEBUFFER_OPERATION: err_str = "INVALID_FRAMEBUFFER_OPERATION"; break;
	case GL_INVALID_INDEX: err_str = "INVALID_INDEX"; break;
	case GL_STACK_OVERFLOW: err_str = "STACK_OVERFLOW"; break;
	case GL_STACK_UNDERFLOW: err_str = "STACK_UNDERFLOW"; break;
	case GL_OUT_OF_MEMORY: err_str = "OUT_OF_MEMORY"; break;
	default: err_str = "UNKNOWN_ERROR"; break;
	}
	fprintf(stderr, "OpenGL Error #%d: %s in file(%s:%d)\n", (int)error, err_str, file, line);
}

}
