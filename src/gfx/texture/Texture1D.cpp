#include <cmath>
#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/gfx/texture/Texture1D.hpp>
#include <axl.glfl/glCoreARB.hpp>

#define GLCLEARERROR() while(axl::glfl::core::GL1::glGetError())

namespace axl {
namespace gl {
namespace gfx {

//////////////
// Texture1D
//

Texture::Dimensions Texture1D::dimensions() const
{
	return Texture::D_1D;
}

Texture1D::Texture1D(axl::gl::Context* ptr_context) :
	Texture(ptr_context)
{}

Texture1D::~Texture1D()
{
	this->idestroy();
}

bool Texture1D::icreate()
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
	glBindTexture(GL_TEXTURE_1D, tmp_id);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if(GL_VERSION_1_3)
	{
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	}
	else if(GL_VERSION_1_2)
	{
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	}
	glBindTexture(GL_TEXTURE_1D, 0);
	this->txr_id = tmp_id;
}
bool Texture1D::idestroy()
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
bool Texture1D::isValid() const
{
	return this->ctx_context && this->ctx_context->isValid() && this->txr_id != -1;
}
bool Texture1D::bind() const
{
	using namespace axl::glfl::core::GL1;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return false;
	GLCLEARERROR();
	glBindTexture(GL_TEXTURE_1D, this->txr_id);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::unbind() const
{
	using namespace axl::glfl::core::GL1;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return false;
	glBindTexture(GL_TEXTURE_1D, 0);
	return true;
}
bool Texture1D::setParami(axl::glfl::GLenum tex_param, axl::glfl::GLint value)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glTexParameteri(GL_TEXTURE_1D, tex_param, value);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::setParamf(axl::glfl::GLenum tex_param, axl::glfl::GLfloat value)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glTexParameterf(GL_TEXTURE_1D, tex_param, value);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::setParamiv(axl::glfl::GLenum tex_param, const axl::glfl::GLint* value)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glTexParameteriv(GL_TEXTURE_1D, tex_param, value);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::setParamfv(axl::glfl::GLenum tex_param, const axl::glfl::GLfloat* value)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glTexParameterfv(GL_TEXTURE_1D, tex_param, value);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::getParamiv(axl::glfl::GLenum tex_param, axl::glfl::GLint* value_ptr)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glGetTexParameteriv(GL_TEXTURE_1D, tex_param, value_ptr);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::getParamfv(axl::glfl::GLenum tex_param, axl::glfl::GLfloat* value_ptr)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glGetTexParameterfv(GL_TEXTURE_1D, tex_param, value_ptr);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::getLevelParamiv(axl::glfl::GLint level, axl::glfl::GLenum tex_param, axl::glfl::GLint* value_ptr)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glGetTexLevelParameteriv(GL_TEXTURE_1D, level, tex_param, value_ptr);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::getLevelParamfv(axl::glfl::GLint level, axl::glfl::GLenum tex_param, axl::glfl::GLfloat* value_ptr)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glGetTexLevelParameterfv(GL_TEXTURE_1D, level, tex_param, value_ptr);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::allocate(axl::glfl::GLsizei width, axl::glfl::GLint internal_format, axl::glfl::GLint border)
{
	using namespace axl::glfl::core::GL1;
	if(!this->bind()) return false;
	glTexImage1D(GL_TEXTURE_1D, 0, internal_format, width, border, GL_RGB, GL_UNSIGNED_BYTE, (const void*)0);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_LOD, 0);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LOD, (axl::glfl::GLsizei)std::log2((double)width));
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::setImage(axl::glfl::GLint level, axl::glfl::GLint x_offset, axl::glfl::GLsizei width, axl::glfl::GLenum format, axl::glfl::GLenum data_type, const axl::glfl::GLvoid* image_data, axl::glfl::GLint pixel_alignment)
{
	using namespace axl::glfl::core::GL1;
	if(!image_data || !this->bind()) return false;
	glPixelStorei(GL_UNPACK_ALIGNMENT, pixel_alignment);
	if(glGetError() != GL_NO_ERROR) return false;
	glTexSubImage1D(GL_TEXTURE_1D, level, x_offset, width, format, data_type, image_data);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::getImage(axl::glfl::GLint level, axl::glfl::GLenum format, axl::glfl::GLenum data_type, axl::glfl::GLvoid* image_data, axl::glfl::GLint pixel_alignment)
{
	using namespace axl::glfl::core::GL1;
	if(!image_data || !this->bind()) return false;
	glPixelStorei(GL_PACK_ALIGNMENT, pixel_alignment);
	if(glGetError() != GL_NO_ERROR) return false;
	glGetTexImage(GL_TEXTURE_1D, level, format, data_type, image_data);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::generateMipmaps()
{
	using namespace axl::glfl::core::GL1;
	using namespace axl::glfl::core::GL3;
	if(!GL_VERSION_3_0 || !this->bind()) return false;
	glGenerateMipmap(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
int Texture1D::getSize(axl::glfl::GLint level) const
{
	int tex_size = -1;
	using namespace axl::glfl::core::GL1;
	if(this->bind())
	{
		axl::glfl::GLint tex_width;
		glGetTexLevelParameteriv(GL_TEXTURE_1D, level, GL_TEXTURE_WIDTH, &tex_width);
		if(glGetError() == GL_NO_ERROR)
			tex_size = tex_width;
		glBindTexture(GL_TEXTURE_1D, 0);
	}
	return tex_size;
}

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
