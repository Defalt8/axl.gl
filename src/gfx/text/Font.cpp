#include <axl.gl/Context.hpp>
#include <axl.gl/ContextObject.hpp>
#include <axl.gl/gfx/text/Font.hpp>
#include <axl.gl/gfx/text/Text.hpp>
#include <axl.glfl/gl.hpp>
#include <axl.glfl/glCoreARB.hpp>
#include <axl.util/String.hpp>
#include <axl.math/basic.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace GL {
	using namespace axl::glfl;
	using namespace axl::glfl::core::GL1;
	using namespace axl::glfl::core::GL2;
	using namespace axl::glfl::core::GL3;
}

namespace axl {
namespace gl {
namespace gfx {

bool ftlib_initialized = false;
FT_Library ftlib_library = (FT_Library)0;

struct FontData
{
	FT_Face face;
	unsigned long glyph_count;
	unsigned long x_count;
	unsigned long y_count;
};

//
// Font
//

Font::Font(axl::gl::Context* ptr_context) :
	texture(font_texture),
	glyphs(font_glyphs),
	size(font_size),
	font_texture(),
	font_size()
{
	this->font_reserved = (void*)new FontData();
	if(this->font_reserved)
	{
		FontData* font_data = (FontData*)font_reserved;
		font_data->face = (FT_Face)0;
		font_data->glyph_count = 0;
		font_data->x_count = 0;
		font_data->y_count = 0;
	}
}

Font::~Font()
{
	this->destroy();
	if(this->font_reserved)
	{
		delete (FontData*)font_reserved;
		this->font_reserved = (void*)0;
	}
}

bool Font::icreate()
{
	if(!Font::init() || !this->font_reserved) return false;
	this->font_texture.setContext(this->ctx_context);
	return this->font_texture.create() && this->font_texture.setParami(GL::GL_TEXTURE_MIN_FILTER, GL::GL_LINEAR) && this->font_texture.setParami(GL::GL_TEXTURE_MAG_FILTER, GL::GL_LINEAR);
}

bool Font::idestroy()
{
	if(!this->isValid() || !this->font_reserved) return false;
	FontData* font_data = (FontData*)font_reserved;
	if(font_data->face)
	{
		FT_Done_Face(font_data->face);
		font_data->face = (FT_Face)0;
		font_data->glyph_count = 0;
		font_data->x_count = 0;
		font_data->y_count = 0;
	}
	this->font_glyphs.destroy();
	return this->font_texture.destroy();
}

bool Font::isValid() const
{
	return this->font_texture.isValid() && this->font_reserved && (this->font_glyphs.count() == ((FontData*)font_reserved)->glyph_count);
}

bool Font::loadFromFile(const axl::util::String& filepath, const axl::math::Vec2i& font_size)
{
	if(font_size.x <= 0 || font_size.y <= 0 || !Font::init() || !this->isValid()) return false;
	FontData* font_data = (FontData*)font_reserved;
	FT_Face ftlib_face;
	if(FT_New_Face(ftlib_library, filepath.cstr(), 0, &ftlib_face) != FT_Err_Ok || !ftlib_face) return false;
	if(font_data->face)
	{
		FT_Done_Face(font_data->face);
		font_data->glyph_count = 0;
		font_data->x_count = 0;
		font_data->y_count = 0;
	}
	font_data->face = ftlib_face;
	if(this->loadGlyphs(font_size, 0, -1))
	{
		GL::GLint max_lod = (GL::GLint)axl::math::log2((float)font_size.x) / 2 + 1;
		this->font_size = font_size;
		this->font_texture.setParami(GL::GL_TEXTURE_MIN_LOD, 0);
		this->font_texture.setParami(GL::GL_TEXTURE_MAX_LOD, max_lod);
		if(this->font_texture.generateMipmaps())
		{
			this->font_texture.setParami(GL::GL_TEXTURE_MIN_FILTER, GL::GL_LINEAR_MIPMAP_NEAREST);
			this->font_texture.setParami(GL::GL_TEXTURE_MAG_FILTER, GL::GL_LINEAR);
		}
		return true;
	}
	return false;
}

bool Font::setSize(const axl::math::Vec2i& font_size)
{
	if(font_size.x <= 0 || font_size.y <= 0 || !Font::init() || !this->isValid()) return false;
	FontData* font_data = (FontData*)font_reserved;
	if(!font_data->face) return false;
	if(this->loadGlyphs(font_size, 0, -1))
	{
		GL::GLint max_lod = (GL::GLint)axl::math::log2((float)font_size.x) / 2 + 1;
		this->font_size = font_size;
		this->font_texture.setParami(GL::GL_TEXTURE_MIN_LOD, 0);
		this->font_texture.setParami(GL::GL_TEXTURE_MAX_LOD, max_lod);
		if(this->font_texture.generateMipmaps())
		{
			this->font_texture.setParami(GL::GL_TEXTURE_MIN_FILTER, GL::GL_LINEAR_MIPMAP_NEAREST);
			this->font_texture.setParami(GL::GL_TEXTURE_MAG_FILTER, GL::GL_LINEAR);
		}
		return true;
	}
	return false;
}

unsigned int Font::getCharIndex(axl::util::WString::char_t unicode_char) const
{
	unsigned int index = -1;
	if(!Font::init() || !this->isValid()) return index;
	FontData* font_data = (FontData*)font_reserved;
	if(!font_data->face) return index;
	FT_UInt glyph_index = FT_Get_Char_Index(font_data->face, unicode_char);
	if(glyph_index <= font_data->glyph_count)
		index = (unsigned int)glyph_index;
	return index;
}

bool Font::loadGlyphs(const axl::math::Vec2i& font_size, int level, unsigned long count)
{
	if(!Font::init() || !this->isValid()) return false;
	FontData* font_data = (FontData*)font_reserved;
	if(!font_data->face) return false;
	FT_Long num_glyphs;
	int x_count;
	int y_count;
	if(level == 0)
	{
		num_glyphs = count <= (unsigned long)font_data->face->num_glyphs ? count : font_data->face->num_glyphs;
		if(!this->font_glyphs.resize(num_glyphs)) return false;
		const int root = (int)axl::math::sqrt((float)num_glyphs);
		const int sq = root * root;
		x_count = sq < num_glyphs ? (root+1) : root;
		y_count = x_count;
	}
	else
	{
		num_glyphs = font_data->glyph_count;
		x_count = font_data->x_count;
		y_count = font_data->y_count;
	}
	int level_factor = (int)axl::math::pow(2.0f, (float)level);
	if(FT_Set_Pixel_Sizes(font_data->face, font_size.x / level_factor, font_size.y / level_factor) != FT_Err_Ok) return false;
	GL::GLsizei width = (GL::GLsizei)(x_count * (font_size.x / level_factor + 2));
	GL::GLsizei height = (GL::GLsizei)(y_count * (font_size.y / level_factor + 2));
	unsigned long glyph_size_x = (unsigned long)(font_size.x / level_factor + 2);
	unsigned long glyph_size_y = (unsigned long)(font_size.y / level_factor + 2);
	GL::GLsizei glyph_size = (GL::GLsizei)(glyph_size_x * glyph_size_y);
	if(!this->font_texture.allocate(level, width, height, GL::GL_R8)) return false;
	FT_Long glyph_index = 0;
	GL::GLubyte *glyph = new GL::GLubyte[glyph_size];
	if(!glyph) return false;
	for(int j = 0; j < y_count; ++j)
	{
		for(int i = 0; i < x_count; ++i)
		{
			if(glyph_index >= num_glyphs) break;
			if(FT_Load_Glyph(font_data->face, glyph_index, FT_LOAD_TARGET_LIGHT|FT_LOAD_IGNORE_TRANSFORM|FT_LOAD_RENDER) == FT_Err_Ok)
			{
				if(level == 0)
				{
					this->font_glyphs[glyph_index].width = (short)(font_data->face->glyph->metrics.width / 64);
					this->font_glyphs[glyph_index].height = (short)(font_data->face->glyph->metrics.height / 64);
					this->font_glyphs[glyph_index].horiBearingX = (short)(font_data->face->glyph->metrics.horiBearingX / 64);
					this->font_glyphs[glyph_index].horiBearingY = (short)(font_data->face->glyph->metrics.horiBearingY / 64);
					this->font_glyphs[glyph_index].vertBearingX = (short)(font_data->face->glyph->metrics.vertBearingX / 64);
					this->font_glyphs[glyph_index].vertBearingY = (short)(font_data->face->glyph->metrics.vertBearingY / 64);
					this->font_glyphs[glyph_index].horiAdvance = (short)(font_data->face->glyph->metrics.horiAdvance / 64);
					this->font_glyphs[glyph_index].vertAdvance = (short)(font_data->face->glyph->metrics.vertAdvance / 64);
					this->font_glyphs[glyph_index].UV.x = (float)(glyph_index % x_count) / x_count - (float)1.f / ((float)font_size.x * width);
					this->font_glyphs[glyph_index].UV.y = (float)(glyph_index / x_count) / y_count - (float)1.f / ((float)font_size.y * height);
					this->font_glyphs[glyph_index].UV.z = this->font_glyphs[glyph_index].UV.x + (float)(this->font_glyphs[glyph_index].width + 4) / (width);
					this->font_glyphs[glyph_index].UV.w = this->font_glyphs[glyph_index].UV.y + (float)(this->font_glyphs[glyph_index].height + 4) / (height);
				}
				if(font_data->face->glyph->format == FT_GLYPH_FORMAT_BITMAP)
				{
					unsigned long row_size = font_data->face->glyph->bitmap.width;
					unsigned long column_size = font_data->face->glyph->bitmap.rows;
					unsigned long bmp_w = font_data->face->glyph->bitmap.width;
					unsigned long bmp_h = font_data->face->glyph->bitmap.rows;
					for(unsigned long gj = 0; gj < (unsigned long)glyph_size_y; ++gj)
					{
						for(unsigned long gi = 0; gi < (unsigned long)glyph_size_x; ++gi)
						{
							unsigned long glyph_index = gj * glyph_size_x + gi;
							if(gi == 0 || gj == 0 || gi == glyph_size_x-1  || gj == glyph_size_y-1)
							{
								glyph[glyph_index] = 0;
							}
							else if(gi-1 < bmp_w && gj-1 < bmp_h)
							{
								glyph[glyph_index] = font_data->face->glyph->bitmap.buffer[((column_size-gj) * row_size) + gi-1];
							}
							else
							{
								glyph[glyph_index] = 0;
							}
						}
					}
					this->font_texture.setImage(level, i * glyph_size_x, j * glyph_size_y, glyph_size_x, glyph_size_y, GL::GL_RED, GL::GL_UNSIGNED_BYTE, glyph, 1);
				}
			}
			++glyph_index;
		}
		if(glyph_index >= num_glyphs) break;
	}
	delete[] glyph;
	if(level == 0)
	{
		font_data->glyph_count = num_glyphs;
		font_data->x_count = x_count;
		font_data->y_count = y_count;
	}
	return true;
}

// static

bool Font::init()
{
	if(ftlib_initialized && ftlib_library) return true;
	ftlib_initialized = false;
	if(FT_Init_FreeType(&ftlib_library) != FT_Err_Ok) return false;
	return (ftlib_initialized = true);
}

bool Font::cleanup()
{
	if(!ftlib_library) return true;
	if(FT_Done_FreeType(ftlib_library) != FT_Err_Ok) return false;
	ftlib_library = (FT_Library)0;
	return true;
}


} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
