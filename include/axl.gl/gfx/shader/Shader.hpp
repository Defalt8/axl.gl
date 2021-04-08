#pragma once
#include "../../lib.hpp"
#include "../../ContextObject.hpp"
#include <axl.glfl/gl.hpp>
#include <axl.util/String.hpp>

namespace axl {
namespace gl {

class AXLGLCXXAPI Context;

namespace gfx {

class AXLGLCXXAPI Program;

class AXLGLCXXAPI Shader : public ContextObject
{
	public:
		AXLGL_ENUM_CLASS Type { TT_1D = 1, TT_2D = 2, TT_3D = 3 };
	public:
		Shader(axl::gl::Context* ptr_context = (axl::gl::Context*)0, axl::glfl::GLenum shader_type = 0);
		virtual ~Shader();
	protected:
		virtual bool icreate();
		virtual bool idestroy();
	public:
		virtual bool isValid() const;
		axl::glfl::GLuint getId() const;
		axl::glfl::GLenum type() const;
		bool setSource(const axl::util::String& shader_code);
		axl::util::String getSource() const;
		bool compile() const;
		bool isCompiled() const;
		axl::util::String getInfoLog() const;
		bool attach(const Program& program) const;
		bool detach(const Program& program) const;
	protected:
		axl::glfl::GLuint shader_id;
		axl::glfl::GLenum shader_type;
};

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
