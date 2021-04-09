#pragma once
#include <axl.gl/everything.hpp>
#include <axl.glfl/gl.hpp>
#include <axl.glfl/glCoreARB.hpp>

#define GLC(X) GL::clearGLError(); X; while((GL::_gl_error = axl::glfl::core::GL::glGetError())) GL::logGLError(GL::_gl_error, __FILE__, __LINE__);

namespace GL {

extern axl::glfl::GLenum _gl_error;

bool loadVFShaders(axl::gl::gfx::Program& program, const axl::util::String& vertex_shader_file, const axl::util::String& fragment_shader_file);

void clearGLError();
void logGLError(axl::glfl::GLenum error, const char* file, int line);


}
