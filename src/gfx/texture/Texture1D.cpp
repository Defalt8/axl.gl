#include <cmath>
#include <axl.gl/View.hpp>
#include <axl.gl/Context.hpp>
#include <axl.gl/gfx/texture/Texture1D.hpp>
#include <axl.glfl/gl.hpp>
#include <axl.glfl/glCoreARB.hpp>

#define GLCLEARERROR() while(axl::glfl::core::GL1::glGetError())

namespace GL {
	using namespace axl::glfl::core::GL1;
	using namespace axl::glfl::core::GL2;
	using namespace axl::glfl::core::GL3;
	using namespace axl::glfl::core::GL4;
}

namespace axl {
namespace gl {
namespace gfx {

//////////////
// Texture1D
//

Texture::Type Texture1D::type() const
{
	return Texture::TT_1D;
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
	using namespace GL;
	if(!GL_VERSION_1_0 || this->txr_id != -1 || !this->ctx_context || !this->ctx_context->makeCurrent())
		return false;
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
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::idestroy()
{
	using namespace GL;
	if(this->unbind())
	{
		GLCLEARERROR();
		glDeleteTextures(1, &this->txr_id);
		if(glGetError() != GL_NO_ERROR) return false;
	}
	this->txr_id = -1;
	return true;
}
axl::glfl::GLuint Texture1D::getId() const
{
	return this->txr_id;
}
bool Texture1D::isValid() const
{
	return this->ctx_context && this->ctx_context->isValid() && this->txr_id != -1;
}
bool Texture1D::bind() const
{
	using namespace GL;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return false;
	GLCLEARERROR();
	glBindTexture(GL_TEXTURE_1D, this->txr_id);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::bind(axl::glfl::GLuint texture_slot) const
{
	using namespace GL;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return false;
	GLCLEARERROR();
	glActiveTexture(GL_TEXTURE0 + texture_slot);
	if(glGetError() != GL_NO_ERROR) return false;
	glBindTexture(GL_TEXTURE_2D, this->txr_id);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::unbind() const
{
	using namespace GL;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return false;
	glBindTexture(GL_TEXTURE_1D, 0);
	return true;
}
bool Texture1D::unbind(axl::glfl::GLuint texture_slot) const
{
	using namespace GL;
	if(!this->isValid() || !this->ctx_context->makeCurrent()) return false;
	glActiveTexture(GL_TEXTURE0 + texture_slot);
	if(glGetError() != GL_NO_ERROR) return false;
	glBindTexture(GL_TEXTURE_1D, 0);
	return true;
}
bool Texture1D::setParami(axl::glfl::GLenum tex_param, axl::glfl::GLint value)
{
	using namespace GL;
	if(!this->bind()) return false;
	glTexParameteri(GL_TEXTURE_1D, tex_param, value);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::setParamf(axl::glfl::GLenum tex_param, axl::glfl::GLfloat value)
{
	using namespace GL;
	if(!this->bind()) return false;
	glTexParameterf(GL_TEXTURE_1D, tex_param, value);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::setParamiv(axl::glfl::GLenum tex_param, const axl::glfl::GLint* value)
{
	using namespace GL;
	if(!this->bind()) return false;
	glTexParameteriv(GL_TEXTURE_1D, tex_param, value);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::setParamfv(axl::glfl::GLenum tex_param, const axl::glfl::GLfloat* value)
{
	using namespace GL;
	if(!this->bind()) return false;
	glTexParameterfv(GL_TEXTURE_1D, tex_param, value);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::getParamiv(axl::glfl::GLenum tex_param, axl::glfl::GLint* value_ptr)
{
	using namespace GL;
	if(!this->bind()) return false;
	glGetTexParameteriv(GL_TEXTURE_1D, tex_param, value_ptr);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::getParamfv(axl::glfl::GLenum tex_param, axl::glfl::GLfloat* value_ptr)
{
	using namespace GL;
	if(!this->bind()) return false;
	glGetTexParameterfv(GL_TEXTURE_1D, tex_param, value_ptr);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::getLevelParamiv(axl::glfl::GLint level, axl::glfl::GLenum tex_param, axl::glfl::GLint* value_ptr)
{
	using namespace GL;
	if(!this->bind()) return false;
	glGetTexLevelParameteriv(GL_TEXTURE_1D, level, tex_param, value_ptr);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::getLevelParamfv(axl::glfl::GLint level, axl::glfl::GLenum tex_param, axl::glfl::GLfloat* value_ptr)
{
	using namespace GL;
	if(!this->bind()) return false;
	glGetTexLevelParameterfv(GL_TEXTURE_1D, level, tex_param, value_ptr);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::allocate(axl::glfl::GLint level, axl::glfl::GLsizei width, axl::glfl::GLint internal_format, axl::glfl::GLint border)
{
	using namespace GL;
	if(!this->bind()) return false;
	axl::glfl::GLenum format;
	switch(internal_format)
	{
		default: format = GL_RED; break;
		case GL_DEPTH_COMPONENT32F:
		case GL_DEPTH_COMPONENT32:
		case GL_DEPTH_COMPONENT24:
		case GL_DEPTH_COMPONENT16: format = GL_DEPTH_COMPONENT; break;
		case GL_RGBA32F:
		case GL_RGBA32I:
		case GL_RGBA32UI:
		case GL_RGBA16:
		case GL_RGBA16F:
		case GL_RGBA16I:
		case GL_RGBA16UI:
		case GL_RGBA8:
		case GL_RGBA8UI:
		case GL_SRGB8_ALPHA8:
		case GL_RGB10_A2:
		case GL_RGB10_A2UI:
		case GL_R11F_G11F_B10F:
		case GL_RG32F:
		case GL_RG32I:
		case GL_RG32UI:
		case GL_RG16:
		case GL_RG16F:
		case GL_RGB16I:
		case GL_RGB16UI:
		case GL_RG8:
		case GL_RG8I:
		case GL_RG8UI:
		case GL_R32F:
		case GL_R32I:
		case GL_R32UI:
		case GL_R16F:
		case GL_R16I:
		case GL_R16UI:
		case GL_R8:
		case GL_R8I:
		case GL_R8UI:
		case GL_RGBA16_SNORM:
		case GL_RGBA8_SNORM:
		case GL_RGB32F:
		case GL_RGB32I:
		case GL_RGB32UI:
		case GL_RGB16_SNORM:
		case GL_RGB16F:
		case GL_RGB16:
		case GL_RGB8_SNORM:
		case GL_RGB8:
		case GL_RGB8I:
		case GL_RGB8UI:
		case GL_SRGB8:
		case GL_RGB9_E5:
		case GL_RG16_SNORM:
		case GL_RG8_SNORM:
		case GL_COMPRESSED_RG_RGTC2:
		case GL_COMPRESSED_SIGNED_RG_RGTC2:
		case GL_R16_SNORM:
		case GL_R8_SNORM:
		case GL_COMPRESSED_RED_RGTC1:
		case GL_COMPRESSED_SIGNED_RED_RGTC1: format = GL_RED; break;
	}
	glTexImage1D(GL_TEXTURE_1D, level, internal_format, width, border, format, GL_UNSIGNED_BYTE, (const void*)0);
	if(level == 0)
	{
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_LOD, 0);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LOD, (axl::glfl::GLsizei)std::log2((double)width));
	}
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::setImage(axl::glfl::GLint level, axl::glfl::GLint x_offset, axl::glfl::GLsizei width, axl::glfl::GLenum format, axl::glfl::GLenum data_type, const axl::glfl::GLvoid* image_data, axl::glfl::GLint pixel_alignment)
{
	using namespace GL;
	if(!image_data || !this->bind()) return false;
	glPixelStorei(GL_UNPACK_ALIGNMENT, pixel_alignment);
	if(glGetError() != GL_NO_ERROR) return false;
	glTexSubImage1D(GL_TEXTURE_1D, level, x_offset, width, format, data_type, image_data);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::getImage(axl::glfl::GLint level, axl::glfl::GLenum format, axl::glfl::GLenum data_type, axl::glfl::GLvoid* image_data, axl::glfl::GLint pixel_alignment)
{
	using namespace GL;
	if(!image_data || !this->bind()) return false;
	glPixelStorei(GL_PACK_ALIGNMENT, pixel_alignment);
	if(glGetError() != GL_NO_ERROR) return false;
	glGetTexImage(GL_TEXTURE_1D, level, format, data_type, image_data);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
bool Texture1D::generateMipmaps()
{
	using namespace GL;
	using namespace axl::glfl::core::GL3;
	if(!GL_VERSION_3_0 || !this->bind()) return false;
	glGenerateMipmap(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_1D, 0);
	return glGetError() == GL_NO_ERROR;
}
int Texture1D::getSize(axl::glfl::GLint level) const
{
	int tex_size = -1;
	using namespace GL;
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
