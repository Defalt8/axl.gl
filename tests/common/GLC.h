#pragma once
#include <axl.glfl/glCoreARB.hpp>

extern axl::glfl::GLenum _gl_error;
void clearGLError();
void logGLError(axl::glfl::GLenum error, const char* file, int line);
#define GLC(X) clearGLError(); X; while((_gl_error = axl::glfl::core::GL::glGetError())) logGLError(_gl_error, __FILE__, __LINE__);
