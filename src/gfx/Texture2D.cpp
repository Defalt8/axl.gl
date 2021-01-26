#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/gfx/Texture2D.hpp>
#include <axl.glfl/glCoreARB.hpp>

#define GLCLEARERROR() while(glGetError())

namespace axl {
namespace gl {
namespace gfx {

////////
// TextureParams
//

TextureParams::TextureParams(const TextureParams& tex_params) :
	min_filter(tex_params.min_filter),
	mag_filter(tex_params.mag_filter),
	wrap_s(tex_params.wrap_s),
	wrap_t(tex_params.wrap_t)
{}

TextureParams::TextureParams(axl::glfl::GLint p_min_filter, axl::glfl::GLint p_mag_filter, axl::glfl::GLint p_wrap_s, axl::glfl::GLint p_wrap_t) :
	min_filter(p_min_filter),
	mag_filter(p_mag_filter),
	wrap_s(p_wrap_s),
	wrap_t(p_wrap_t)
{}

TextureParams TextureParams::Default(axl::glfl::core::GL1::GL_NEAREST, axl::glfl::core::GL1::GL_NEAREST, axl::glfl::core::GL1::GL_CLAMP_TO_EDGE, axl::glfl::core::GL1::GL_CLAMP_TO_EDGE);
const TextureParams TextureParams::Null(-1, -1, -1, -1);

//////////////
// Texture2D
//

Texture2D::Texture2D() :
	context(m_context),
	id(m_id),
	internal_format(m_internal_format),
	size(m_size),
	m_context(),
	m_id(),
	m_internal_format(0),
	m_size(0, 0)
{}

Texture2D::~Texture2D()
{}

bool Texture2D::create(axl::gl::Context* p_context, axl::glfl::GLint p_internal_format, const axl::math::Vec2i& p_size, const TextureParams& tex_params, axl::glfl::GLint p_border)
{
	if(!p_context || !p_context->isValid() || !axl::glfl::core::load() || !axl::glfl::core::GL1::GL_VERSION_1_1 || !p_context->makeCurrent()) return false;
	using namespace axl::glfl::core::GL1;
	axl::glfl::GLuint l_id = 0;
	GLCLEARERROR();
	glGenTextures(1, &l_id);
	if(!l_id || glGetError() != GL_NO_ERROR) return false;
	glBindTexture(GL_TEXTURE_2D, l_id);
	if(glGetError() != GL_NO_ERROR) return false;
	glTexImage2D(GL_TEXTURE_2D, 0, p_internal_format, p_size.x, p_size.y, p_border, GL_RED, GL_UNSIGNED_BYTE, (const void*)0);
	if(glGetError() != GL_NO_ERROR) return false;
	if(tex_params.min_filter != -1) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex_params.min_filter);
	if(tex_params.mag_filter != -1) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex_params.mag_filter);
	if(tex_params.wrap_s != -1) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tex_params.wrap_s);
	if(tex_params.wrap_t != -1) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tex_params.wrap_t);
	if(glGetError() != GL_NO_ERROR)
	{
		glDeleteTextures(1, &l_id);
		return false;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	this->m_context = p_context;
	this->m_internal_format = p_internal_format;
	this->m_size = p_size;
	this->m_id = l_id;
	return true;
}

bool Texture2D::destroy()
{
	if(!this->modify()) return false;
	using namespace axl::glfl::core::GL1;
	GLCLEARERROR();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &this->m_id);
	this->m_context = 0;
	this->m_id = -1;
	this->m_internal_format = 0;
	this->m_size.set(0, 0);
	return glGetError() == GL_NO_ERROR;
}

bool Texture2D::isValid() const
{
	return this->m_context && this->m_context->isValid() && this->m_id != -1;
}

bool Texture2D::setImage(axl::glfl::GLint p_level, const axl::math::Vec2i& p_offset, const axl::math::Vec2i& p_size, axl::glfl::GLenum p_format, axl::glfl::GLenum p_type, const void* p_data, axl::glfl::GLint p_pixel_alignment)
{
	if(!this->modify() || !p_data) return false;
	using namespace axl::glfl::core::GL1;
	GLCLEARERROR();
	glBindTexture(GL_TEXTURE_2D, this->m_id);
	if(glGetError() != GL_NO_ERROR) return false;
	glPixelStorei(GL_UNPACK_ALIGNMENT, p_pixel_alignment);
	glTexSubImage2D(GL_TEXTURE_2D, p_level, p_offset.x, p_offset.y, p_size.x, p_size.y, p_format, p_type, p_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return glGetError() == GL_NO_ERROR;
}

bool Texture2D::bind() const
{
	if(!this->isValid() || !axl::glfl::core::load() || !axl::glfl::core::GL1::GL_VERSION_1_1 || !this->m_context->makeCurrent()) return false;
	using namespace axl::glfl::core::GL1;
	GLCLEARERROR();
	glBindTexture(GL_TEXTURE_2D, this->m_id);
	return glGetError() == GL_NO_ERROR;
}

bool Texture2D::unbind() const
{
	if(!this->modify()) return false;
	using namespace axl::glfl::core::GL1;
	GLCLEARERROR();
	glBindTexture(GL_TEXTURE_2D, 0);
	return glGetError() == GL_NO_ERROR;
}

bool Texture2D::setParams(const TextureParams& tex_params)
{
	if(!this->modify()) return false;
	using namespace axl::glfl::core::GL1;
	GLCLEARERROR();
	glBindTexture(GL_TEXTURE_2D, this->m_id);
	if(glGetError() != GL_NO_ERROR) return false;
	if(tex_params.min_filter != -1) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex_params.min_filter);
	if(tex_params.mag_filter != -1) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex_params.mag_filter);
	if(tex_params.wrap_s != -1) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tex_params.wrap_s);
	if(tex_params.wrap_t != -1) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tex_params.wrap_t);
	glBindTexture(GL_TEXTURE_2D, 0);
	return glGetError() == GL_NO_ERROR;
}

bool Texture2D::setParami(axl::glfl::GLenum tex_param, axl::glfl::GLint value)
{
	if(!this->modify()) return false;
	using namespace axl::glfl::core::GL1;
	GLCLEARERROR();
	glBindTexture(GL_TEXTURE_2D, this->m_id);
	if(glGetError() != GL_NO_ERROR) return false;
	glTexParameteri(GL_TEXTURE_2D, tex_param, value);
	return glGetError() == GL_NO_ERROR;
}

bool Texture2D::setParamf(axl::glfl::GLenum tex_param, axl::glfl::GLfloat value)
{
	if(!this->modify()) return false;
	using namespace axl::glfl::core::GL1;
	GLCLEARERROR();
	glBindTexture(GL_TEXTURE_2D, this->m_id);
	if(glGetError() != GL_NO_ERROR) return false;
	glTexParameterf(GL_TEXTURE_2D, tex_param, value);
	return glGetError() == GL_NO_ERROR;
}

bool Texture2D::setParamiv(axl::glfl::GLenum tex_param, const axl::glfl::GLint* pvalue)
{
	if(!this->modify()) return false;
	using namespace axl::glfl::core::GL1;
	GLCLEARERROR();
	glBindTexture(GL_TEXTURE_2D, this->m_id);
	if(glGetError() != GL_NO_ERROR) return false;
	glTexParameteriv(GL_TEXTURE_2D, tex_param, pvalue);
	return glGetError() == GL_NO_ERROR;
}

bool Texture2D::setParamfv(axl::glfl::GLenum tex_param, const axl::glfl::GLfloat* pvalue)
{
	if(!this->modify()) return false;
	using namespace axl::glfl::core::GL1;
	GLCLEARERROR();
	glBindTexture(GL_TEXTURE_2D, this->m_id);
	if(glGetError() != GL_NO_ERROR) return false;
	glTexParameterfv(GL_TEXTURE_2D, tex_param, pvalue);
	return glGetError() == GL_NO_ERROR;
}


////

bool Texture2D::modify() const
{
	return this->isValid() && axl::glfl::core::load() && axl::glfl::core::GL1::GL_VERSION_1_1 && this->m_context->makeCurrent();
}

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
