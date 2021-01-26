#include <stdio.h>
#include <stdlib.h>
#include "GLC.h"

axl::glfl::GLenum _gl_error;

int Error(int err_code, const char* category, const char* err_msg, bool fatal)
{
	fprintf(stderr, "%s ERROR #%d: %s: %s\n", (fatal ? "FATAL" : ""), err_code, category, err_msg);
	if(fatal) exit(err_code);
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