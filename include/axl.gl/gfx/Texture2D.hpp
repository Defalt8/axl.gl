#pragma once
#include "../lib.hpp"
#include <axl.math/vec/Vec2i.hpp>
#include <axl.glfl/gl.hpp>

namespace axl {
namespace gl {

class AXLGLCXXAPI Context;

namespace gfx {

struct AXLGLCXXAPI TextureParams
{
	public:
		TextureParams(const TextureParams& tex_params);
		TextureParams(axl::glfl::GLint min_filter = Default.min_filter, axl::glfl::GLint mag_filter = Default.mag_filter, axl::glfl::GLint wrap_s = Default.wrap_s, axl::glfl::GLint wrap_t = Default.wrap_t);
	public:
		axl::glfl::GLint min_filter;
		axl::glfl::GLint mag_filter;
		axl::glfl::GLint wrap_s;
		axl::glfl::GLint wrap_t;
		static TextureParams Default;
		const static TextureParams Null;
};

class AXLGLCXXAPI Texture2D
{
	public:
		Texture2D();
		virtual ~Texture2D();
		bool create(axl::gl::Context* context, axl::glfl::GLint internal_format, const axl::math::Vec2i& size, const TextureParams& tex_params = TextureParams(), axl::glfl::GLint border = 0);
		bool destroy();
		bool isValid() const;
		bool setImage(axl::glfl::GLint level, const axl::math::Vec2i& offset, const axl::math::Vec2i& size, axl::glfl::GLenum format, axl::glfl::GLenum type, const void* data, axl::glfl::GLint pixel_alignment = 1);
		bool bind() const;
		bool unbind() const;
		bool setParams(const TextureParams& tex_params);
		bool setParami(axl::glfl::GLenum tex_param, axl::glfl::GLint value);
		bool setParamf(axl::glfl::GLenum tex_param, axl::glfl::GLfloat value);
		bool setParamiv(axl::glfl::GLenum tex_param, const axl::glfl::GLint* value);
		bool setParamfv(axl::glfl::GLenum tex_param, const axl::glfl::GLfloat* value);
	protected:
		bool modify() const;
	public:
		axl::gl::Context* const& context;
		const axl::glfl::GLuint& id;
		const axl::glfl::GLint& internal_format;
		const axl::math::Vec2i& size;
	private:
		axl::gl::Context* m_context;
		axl::glfl::GLuint m_id;
		axl::glfl::GLint m_internal_format;
		axl::math::Vec2i m_size;
};

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
