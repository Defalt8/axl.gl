#include <cmath>
#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/gfx/texture/Texture2D.hpp>
#include <axl.glfl/glCoreARB.hpp>

#define GLCLEARERROR() while(glGetError() != GL_NO_ERROR)

namespace axl {
namespace gl {
namespace gfx {

//////////////
// Texture2D
//

Texture::Dimensions Texture2D::dimensions() const
{
	return Texture::D_2D;
}

Texture2D::Texture2D(axl::gl::Context* ptr_context) :
	Texture(ptr_context)
{}

Texture2D::~Texture2D()
{
	this->destroy();
}
bool Texture2D::icreate()
{
	using namespace axl::glfl::core::GL1;
	if(!GL_VERSION_1_0 || this->isValid() || !this->ctx_context->makeCurrent()) return false;
	axl::glfl::GLuint tmp_id;
	GLCLEARERROR();
	glGenTextures(1, &tmp_id);
	if(tmp_id == -1) return false;
	if(glGetError() != GL_NO_ERROR)
	{
		glDeleteTextures(1, &tmp_id);
		return false;
	}
	glBindTexture(GL_TEXTURE_2D, tmp_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if(GL_VERSION_1_3)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	}
	else if(GL_VERSION_1_2)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	this->txr_id = tmp_id;
	return true;
}
bool Texture2D::idestroy()
{
	using namespace axl::glfl::core::GL1;
	if(this->unbind())
	{
		GLCLEARERROR();
		glDeleteTextures(1, &this->txr_id);
		if(glGetError() != GL_NO_ERROR) return false;
	}
	this->txr_id = -1;
	return true;
}
bool Texture2D::isValid() const
{
	return this->ctx_context && this->ctx_context->isValid() && this->txr_id != -1;
}
bool Texture2D::bind() const
{
	using namespace axl::glfl::core::GL1;
	if(this->isValid() || !this->ctx_context->makeCurrent()) return false;
	GLCLEARERROR();
	glBindTexture(GL_TEXTURE_2D, this->txr_id);
	return glGetError() == GL_NO_ERROR;
}
bool Texture2D::unbind() const
{
	using namespace axl::glfl::core::GL1;
	if(this->isValid() || !this->ctx_context->makeCurrent()) return false;
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}
bool Texture2D::setParami(axl::glfl::GLenum tex_param, axl::glfl::GLint value)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glTexParameteri(GL_TEXTURE_2D, tex_param, value);
	glBindTexture(GL_TEXTURE_2D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture2D::setParamf(axl::glfl::GLenum tex_param, axl::glfl::GLfloat value)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glTexParameterf(GL_TEXTURE_2D, tex_param, value);
	glBindTexture(GL_TEXTURE_2D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture2D::setParamiv(axl::glfl::GLenum tex_param, const axl::glfl::GLint* value)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glTexParameteriv(GL_TEXTURE_2D, tex_param, value);
	glBindTexture(GL_TEXTURE_2D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture2D::setParamfv(axl::glfl::GLenum tex_param, const axl::glfl::GLfloat* value)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glTexParameterfv(GL_TEXTURE_2D, tex_param, value);
	glBindTexture(GL_TEXTURE_2D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture2D::getParamiv(axl::glfl::GLenum tex_param, axl::glfl::GLint* value_ptr)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glGetTexParameteriv(GL_TEXTURE_2D, tex_param, value_ptr);
	glBindTexture(GL_TEXTURE_2D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture2D::getParamfv(axl::glfl::GLenum tex_param, axl::glfl::GLfloat* value_ptr)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glGetTexParameterfv(GL_TEXTURE_2D, tex_param, value_ptr);
	glBindTexture(GL_TEXTURE_2D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture2D::getLevelParamiv(axl::glfl::GLint level, axl::glfl::GLenum tex_param, axl::glfl::GLint* value_ptr)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, level, tex_param, value_ptr);
	glBindTexture(GL_TEXTURE_2D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture2D::getLevelParamfv(axl::glfl::GLint level, axl::glfl::GLenum tex_param, axl::glfl::GLfloat* value_ptr)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glGetTexLevelParameterfv(GL_TEXTURE_2D, level, tex_param, value_ptr);
	glBindTexture(GL_TEXTURE_2D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture2D::allocate(axl::glfl::GLsizei width, axl::glfl::GLsizei height, axl::glfl::GLint internal_format, axl::glfl::GLint border)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, border, GL_RGB, GL_UNSIGNED_BYTE, (const void*)0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, (axl::glfl::GLsizei)std::log2((float)width >= height ? width : height));
	glBindTexture(GL_TEXTURE_2D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture2D::setImage(axl::glfl::GLint level, axl::glfl::GLint x_offset, axl::glfl::GLint y_offset, axl::glfl::GLsizei width, axl::glfl::GLsizei height, axl::glfl::GLenum format, axl::glfl::GLenum data_type, const axl::glfl::GLvoid* image_data, axl::glfl::GLint pixel_alignment)
{
	using namespace axl::glfl::core::GL1;
	if(!image_data || !this->bind()) return false;
	glPixelStorei(GL_UNPACK_ALIGNMENT, pixel_alignment);
	if(glGetError() != GL_NO_ERROR) return false;
	glTexSubImage2D(GL_TEXTURE_2D, level, x_offset, y_offset, width, height, format, data_type, image_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture2D::getImage(axl::glfl::GLint level, axl::glfl::GLenum format, axl::glfl::GLenum data_type, axl::glfl::GLvoid* image_data, axl::glfl::GLint pixel_alignment)
{
	using namespace axl::glfl::core::GL1;
	if(!image_data || !this->bind()) return false;
	glPixelStorei(GL_PACK_ALIGNMENT, pixel_alignment);
	if(glGetError() != GL_NO_ERROR) return false;
	glGetTexImage(GL_TEXTURE_2D, level, format, data_type, image_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture2D::generateMipmaps()
{
	using namespace axl::glfl::core::GL1;
	using namespace axl::glfl::core::GL3;
	if(!GL_VERSION_3_0 || !this->bind()) return false;
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return glGetError() == GL_NO_ERROR;
}
axl::math::Vec2i Texture2D::getSize(axl::glfl::GLint level) const
{
	axl::math::Vec2i tex_size(-1,-1);
	using namespace axl::glfl::core::GL1;
	if(this->bind())
	{
		axl::glfl::GLint tex_width, tex_height;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, &tex_width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT, &tex_height);
		if(glGetError() == GL_NO_ERROR)
			tex_size.set(tex_width, tex_height);
		
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	return tex_size;
}

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
