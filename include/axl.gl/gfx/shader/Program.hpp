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
		bool useNull() const;
		axl::glfl::GLint getUniformLocation(const axl::util::String& uniform_name) const;
		bool setUniformMat4f(axl::glfl::GLint location, const axl::glfl::GLfloat* matrix_ptr, axl::glfl::GLboolean transpose = 0);
		bool setUniformMat4d(axl::glfl::GLint location, const axl::glfl::GLdouble* matrix_ptr, axl::glfl::GLboolean transpose = 0);
	protected:
		axl::glfl::GLuint program_id;
		axl::glfl::GLenum program_type;
};

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
