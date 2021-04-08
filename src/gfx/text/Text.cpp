#include <axl.gl/Context.hpp>
#include <axl.gl/ContextObject.hpp>
#include <axl.gl/gfx/text/Font.hpp>
#include <axl.gl/gfx/text/Text.hpp>
#include <axl.math/vec.hpp>
#include <axl.math/mat.hpp>
#include <axl.glfl/gl.hpp>
#include <axl.glfl/glCoreARB.hpp>

#define GLCLEARERROR() while(axl::glfl::core::GL1::glGetError() != axl::glfl::core::GL1::GL_NO_ERROR)

namespace GL {
	using namespace axl::glfl;
	using namespace axl::glfl::core::GL;
}

namespace axl {
namespace gl {
namespace gfx {

Text::Text(axl::gl::Context* ptr_context) :
	text_transform(),
	text_color(0.0f, 0.0f, 0.0f, 1.0f),
	text_box(0.0f, 0.0f),
	text_offset(0.0f, 0.0f),
	text_spacing(0.0f, 0.0f),
	text_wstring(),
	text_font(),
	text_alignment(Text::Alignment::TAL_CENTER),
	text_program(),
	actual_text_length(0),
	vertex_array_id(-1),
	vertex_buffer_id(-1),
	element_array_id(-1),
	attribute_location_position(-1),
	attribute_location_UV(-1),
	uniform_location_projection_matrix(-1),
	uniform_location_view_matrix(-1),
	uniform_location_model_matrix(-1),
	uniform_location_text_offset(-1),
	uniform_location_text_color(-1)
{
	this->updateAlignment();
}
Text::~Text()
{
	this->destroy();
}
bool Text::icreate()
{
	using namespace GL;
	if(!GL_VERSION_3_0 || !this->ctx_context || this->ctx_context->config.major_version < 3 || !this->text_font || !this->text_program || 
	  !this->text_font->isValid() || !this->text_program->isValid() || !this->ctx_context->isValid() || !this->ctx_context->makeCurrent()
	)
		return false;
	GLCLEARERROR();
	glGenVertexArrays(1, &this->vertex_array_id);
	if(this->vertex_array_id == -1 || glGetError() != GL_NO_ERROR) return false;
	glGenBuffers(1, &this->vertex_buffer_id);
	if(this->vertex_buffer_id == -1 || glGetError() != GL_NO_ERROR)
	{
		glDeleteVertexArrays(1, &this->vertex_array_id);
		return false;
	}
	glGenBuffers(1, &this->element_array_id);
	if(this->element_array_id == -1 || glGetError() != GL_NO_ERROR)
	{
		glDeleteBuffers(1, &this->vertex_buffer_id);
		glDeleteVertexArrays(1, &this->vertex_array_id);
		return false;
	}
	this->setProgram(this->text_program);
	return glGetError() == GL_NO_ERROR;
}
bool Text::idestroy()
{
	using namespace GL;
	if(this->ctx_context && this->ctx_context->isValid() && this->ctx_context->makeCurrent())
	{
		GLCLEARERROR();
		if(this->element_array_id != -1)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, &this->element_array_id);
		}
		if(this->vertex_buffer_id != -1)
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, &this->vertex_buffer_id);
		}
		if(this->vertex_array_id != -1)
		{
			glBindVertexArray(0);
			glDeleteVertexArrays(1, &this->vertex_array_id);
		}
		return glGetError() == GL_NO_ERROR;
	}
	this->element_array_id = -1;
	this->vertex_buffer_id = -1;
	this->vertex_array_id = -1;
	return true;
}
bool Text::isValid() const
{
	return this->ctx_context && this->text_font && this->text_program && 
		   this->ctx_context->isValid() && this->text_font->isValid() && this->text_program->isValid() && 
		   this->vertex_array_id != -1 && this->vertex_buffer_id != -1 && this->element_array_id != -1 &&
		   this->attribute_location_position != -1 && this->attribute_location_UV != -1;
}
bool Text::render(const axl::gl::camera::Camera3Df* camera)
{
	using namespace GL;
	if(!this->isValid()) return false;
	if(this->rec_font_size.notEquals(this->text_font->size))
		this->updateBuffers(this->text_wstring, false, true);
	if(actual_text_length > 0)
	{
		if(camera)
		{
			if(camera->projection)
				this->text_program->setUniformMat4fv(uniform_location_projection_matrix, camera->projection->matrix.values);
			this->text_program->setUniformMat4fv(uniform_location_view_matrix, camera->view_transform.values);
		}
		this->text_program->setUniformMat4fv(uniform_location_model_matrix, this->text_transform.getMatrix().values);
		this->text_program->setUniform2fv(uniform_location_text_offset, &this->text_offset.x);
		this->text_program->setUniform4fv(uniform_location_text_color, &this->text_color.x);
		if(!this->text_font->texture.bind() || !this->text_program->use())
			return false;
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
		GLCLEARERROR();
		glBindVertexArray(this->vertex_array_id);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->element_array_id);
		glDrawElements(GL_TRIANGLES, 6 * this->actual_text_length, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		if(glGetError() != GL_NO_ERROR)
		{
			this->text_font->texture.unbind();
			this->text_program->unuse();
			return false;
		}
		this->text_font->texture.unbind();
		this->text_program->unuse();
		return true;
	}
	return true;
}
void Text::updateBuffers()
{
	if(this->isValid()) this->updateBuffers(this->text_wstring, false, this->text_font ? this->text_font->size != this->rec_font_size : false);
}
void Text::updateAlignment()
{
	switch((this->text_alignment & 0x03))
	{
		default:
		case HorizontalAlignment::TAL_LEFT:
			this->text_offset.x = 0.0f;
			break;
		case HorizontalAlignment::TAL_RIGHT:
			this->text_offset.x = -this->text_box.x;
			break;
		case HorizontalAlignment::TAL_HORIZONTAL_CENTER:
			this->text_offset.x = -this->text_box.x / 2.0f;
			break;
	}
	switch((this->text_alignment & 0x0C))
	{
		default:
		case VerticalAlignment::TAL_TOP:
			this->text_offset.y = 0.0f;
			break;
		case VerticalAlignment::TAL_BOTTOM:
			this->text_offset.y = this->text_box.y;
			break;
		case VerticalAlignment::TAL_VERTICAL_CENTER:
			this->text_offset.y = this->text_box.y / 2.0f;
			break;
	}
}
// set methods
void Text::setColor(const axl::math::Vec4f& p_color)
{
	this->text_color = p_color;
}
bool Text::setSpacing(const axl::math::Vec2f& spacing)
{
	if(spacing.x >= 0.0f && spacing.y >= 0.0f)
	{
		this->text_spacing = spacing;
		this->updateBuffers(this->text_wstring, false, true);
		return true;
	}
	return false;
}
bool Text::setStorageSize(axl::util::size_t size)
{
	if(size == 0 || !this->isValid() || !this->text_wstring.resize(size - 1)) return false;
	return this->updateBuffers(this->text_wstring, true);
}
bool Text::setText(const axl::util::WString& p_wstring)
{
	using namespace GL;
	if(!this->isValid()) return false;
	axl::util::size_t new_length = p_wstring.length();
	const bool need_more_size = new_length >= this->text_wstring.size();
	if(need_more_size && !this->text_wstring.resize(new_length)) return false;
	axl::util::WString::scwCopy(p_wstring.cwstr(), this->text_wstring.wstr(), new_length+1, 0, 0);
	this->text_wstring.length(true);
	return this->updateBuffers(p_wstring, need_more_size);
}
void Text::setFont(const Font* p_font)
{
	this->text_font = p_font;
	if(p_font)
		this->rec_font_size = p_font->size;
	this->updateBuffers(this->text_wstring, false, true);
}
void Text::setProgram(const axl::gl::gfx::Program* text_shader_program)
{
	this->text_program = text_shader_program;
	if(text_shader_program)
	{
		this->attribute_location_position = this->text_program->getAttributeLocation("in_Position");
		this->attribute_location_UV = this->text_program->getAttributeLocation("in_UV");
		this->uniform_location_projection_matrix = this->text_program->getUniformLocation("u_MatProjection");
		this->uniform_location_view_matrix = this->text_program->getUniformLocation("u_MatView");
		this->uniform_location_model_matrix = this->text_program->getUniformLocation("u_MatModel");
		this->uniform_location_text_offset = this->text_program->getUniformLocation("u_TextOffset");
		this->uniform_location_text_color = this->text_program->getUniformLocation("u_TextColor");
	}
	else
	{
		this->attribute_location_position = -1;
		this->attribute_location_UV = -1;
		this->uniform_location_projection_matrix = -1;
		this->uniform_location_view_matrix = -1;
		this->uniform_location_model_matrix = -1;
		this->uniform_location_text_offset = -1;
		this->uniform_location_text_color = -1;
	}
}
void Text::setAlignment(AlignmentFlag p_alignment_flags)
{
	this->text_alignment = p_alignment_flags;
	this->updateBuffers(this->text_wstring, false, true);
}
void Text::setHorizontalAlignment(HorizontalAlignment horizontal_alignment)
{
	this->text_alignment = (this->text_alignment & 0x0C) | horizontal_alignment;
	this->updateBuffers(this->text_wstring, false, true);
}
void Text::setVerticalAlignment(VerticalAlignment vertical_alignment)
{
	this->text_alignment = (this->text_alignment & 0x03) | vertical_alignment;
	this->updateBuffers(this->text_wstring, false, true);
}
// get methods
const axl::math::Vec4f& Text::getColor() const
{
	return this->text_color;
}
const axl::math::Vec2f& Text::getOffset() const
{
	return this->text_offset;
}
const axl::math::Vec2f& Text::getBox() const
{
	return this->text_box;
}
const axl::math::Vec2f& Text::getSpacing() const
{
	return this->text_spacing;
}
const axl::util::WString& Text::getText() const
{
	return this->text_wstring;
}
const axl::gl::gfx::Font* Text::getFont() const
{
	return this->text_font;
}
const axl::gl::gfx::Program* Text::getProgram() const
{
	return this->text_program;
}
Text::AlignmentFlag Text::getAlignment() const
{
	return this->text_alignment;
}
Text::HorizontalAlignment Text::getHorizontalAlignment() const
{
	switch((this->text_alignment & 0x03))
	{
		default:
		case HorizontalAlignment::TAL_HORIZONTAL_CENTER: return HorizontalAlignment::TAL_HORIZONTAL_CENTER;
		case HorizontalAlignment::TAL_LEFT: return HorizontalAlignment::TAL_LEFT;
		case HorizontalAlignment::TAL_RIGHT: return HorizontalAlignment::TAL_RIGHT;
	}
}
Text::VerticalAlignment Text::getVerticalAlignment() const
{
	switch((this->text_alignment & 0x0C))
	{
		default:
		case VerticalAlignment::TAL_VERTICAL_CENTER: return VerticalAlignment::TAL_VERTICAL_CENTER;
		case VerticalAlignment::TAL_TOP: return VerticalAlignment::TAL_TOP;
		case VerticalAlignment::TAL_BOTTOM: return VerticalAlignment::TAL_BOTTOM;
	}
}
bool Text::updateBuffers(const axl::util::WString& p_wstring, bool text_size_changed, bool font_attribute_altered)
{
	using namespace GL;
	if(!this->isValid()) return false;
	const bool text_changed = &p_wstring != &this->text_wstring;
	unsigned int text_length = this->text_wstring.length(), actual_length = 0, line_count = 0;
	float max_line_width = 0.0f, max_line_height = 0.0f, line_width_accum = 0.0f;
	axl::util::ds::Array<float, axl::util::ds::Allocators::Malloc<float>> line_widths((text_length) / 2 + 1);
	for(unsigned int i = 0; i <= text_length; ++i)
	{
		axl::util::WString::char_t wchr = p_wstring[i];
		switch(wchr)
		{
			case L'\n':
			case L'\0':
				{
					if(line_count >= line_widths.count())
						line_widths.resize(line_count + (text_length-i) / 2);
					if(line_width_accum > max_line_width) max_line_width = line_width_accum;
					line_widths[line_count] = line_width_accum;
					line_width_accum = 0.0f;
					++line_count;
				}
			case L'\r':
			case L'\t':
				continue;
			default:
				{
					unsigned int index = this->text_font->getCharIndex(wchr);
					if(index != -1)
					{
						const axl::gl::gfx::GlyphData& glyph = this->text_font->glyphs[index];
						line_width_accum += ((float)glyph.horiAdvance + this->text_spacing.x);
						if(glyph.height > max_line_height) max_line_height = glyph.height;
					}
				}
				++actual_length;
				continue;
		}
	}
	this->text_box.set(((float)max_line_width - this->text_spacing.x), ((float)((line_count - 1) * (this->text_font->size.y + this->text_spacing.y)) - this->text_spacing.y));
	this->updateAlignment();
	this->actual_text_length = actual_length;
	const unsigned int text_count = this->text_wstring.size() - 1;
	const unsigned int vertex_count = 16 * text_count, indeces_count = 6 * text_count;
	GLCLEARERROR();
	glBindVertexArray(this->vertex_array_id);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->element_array_id);
	if(text_size_changed)
	{
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat) * vertex_count), 0, GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint) * indeces_count), 0, GL_DYNAMIC_DRAW);
		if(glGetError() != GL_NO_ERROR)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			return false;
		}
		const GLsizei stride = (GLsizei)(sizeof(GLfloat) * 4);
		glEnableVertexAttribArray(this->attribute_location_position);
		glEnableVertexAttribArray(this->attribute_location_UV);
		glVertexAttribPointer(this->attribute_location_position, 2, GL_FLOAT, GL_FALSE, stride, 0);
		glVertexAttribPointer(this->attribute_location_UV, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(GLfloat) * 2));
	}
	if((text_changed && text_length > 0) || font_attribute_altered)
	{
		GLfloat* buffer = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		GLuint* indeces = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		if(!buffer || !indeces)
		{
			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			return false;
		}
		GLfloat horizontal_advance = 0.0f, vertical_advance = (GLfloat)-this->text_font->size.y;
		const unsigned int space_glyph_index = this->text_font->getCharIndex(L' ');
		const GLfloat space_advance = (GLfloat)(space_glyph_index == -1 ? this->text_font->size.x : this->text_font->glyphs[space_glyph_index].horiAdvance);
		const GLfloat tab_advance = space_advance * 4.0f;
		unsigned int skipped_chars_count = 0;
		line_count = 0;
		for(unsigned int i = 0; i <= text_length; ++i)
		{
			switch(this->text_wstring[i])
			{
				case L'\n':
				case L'\0':
					horizontal_advance = 0.0f;
					vertical_advance -= this->text_font->size.y + this->text_spacing.y;
					++skipped_chars_count;
					++line_count;
					continue;
				case L'\r':
					horizontal_advance = 0.0f;
					++skipped_chars_count;
					continue;
				case L'\t':
					horizontal_advance += tab_advance;
					++skipped_chars_count;
					continue;
			}
			const unsigned int i_actual = i - skipped_chars_count;
			const unsigned int base_vertex_index = i_actual * 16, base_indeces_index = i_actual * 6, vertex_indeces_index = i_actual * 4;
			const unsigned int glyph_index = this->text_font->getCharIndex(this->text_wstring[i]);
			const axl::gl::gfx::GlyphData& glyph = this->text_font->glyphs[glyph_index];
			axl::math::Vec2f offset((float)(horizontal_advance + glyph.horiBearingX), (float)(vertical_advance + glyph.horiBearingY));
			switch((this->text_alignment & 0x03))
			{
				default:
				case HorizontalAlignment::TAL_LEFT:
					break;
				case HorizontalAlignment::TAL_RIGHT:
					offset.x += (max_line_width - line_widths[line_count]);
					break;
				case HorizontalAlignment::TAL_HORIZONTAL_CENTER:
					offset.x += (max_line_width - line_widths[line_count]) / 2.0f;
					break;
			}
			switch((this->text_alignment & 0x0C))
			{
				default:
				case VerticalAlignment::TAL_TOP:
					break;
				case VerticalAlignment::TAL_BOTTOM:
					offset.y += (this->text_font->size.y * 1.25f);
					break;
				case VerticalAlignment::TAL_VERTICAL_CENTER:
					offset.y += (this->text_font->size.y * 0.75f);
					break;
			}
			const axl::math::Vec2f offsetted_size((float)(offset.x + glyph.width), (float)(offset.y + glyph.height));
			const axl::math::Vec4f& UV = glyph.UV;
			horizontal_advance += (GLfloat)glyph.horiAdvance + this->text_spacing.x;

			buffer[base_vertex_index] = offset.x;
			buffer[base_vertex_index + 1] = offset.y;
			buffer[base_vertex_index + 2] = UV.x;
			buffer[base_vertex_index + 3] = UV.y;

			buffer[base_vertex_index + 4] = offsetted_size.x;
			buffer[base_vertex_index + 5] = offset.y;
			buffer[base_vertex_index + 6] = UV.z;
			buffer[base_vertex_index + 7] = UV.y;

			buffer[base_vertex_index + 8] = offsetted_size.x;
			buffer[base_vertex_index + 9] = offsetted_size.y;
			buffer[base_vertex_index + 10] = UV.z;
			buffer[base_vertex_index + 11] = UV.w;

			buffer[base_vertex_index + 12] = offset.x;
			buffer[base_vertex_index + 13] = offsetted_size.y;
			buffer[base_vertex_index + 14] = UV.x;
			buffer[base_vertex_index + 15] = UV.w;
			
			indeces[base_indeces_index] = vertex_indeces_index;
			indeces[base_indeces_index + 1] = vertex_indeces_index + 1;
			indeces[base_indeces_index + 2] = vertex_indeces_index + 2;
			indeces[base_indeces_index + 3] = vertex_indeces_index + 2;
			indeces[base_indeces_index + 4] = vertex_indeces_index + 3;
			indeces[base_indeces_index + 5] = vertex_indeces_index;
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return glGetError() == GL_NO_ERROR;
}

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
