#include <cstdio>
#include <axl.gl/lib.hpp>
#include <axl.gl/gfx/UI/TextView.hpp>
#include <axl.glfl/glCoreARB.hpp>

#define GLCLEARERROR() while(axl::glfl::core::GL1::glGetError() != axl::glfl::core::GL1::GL_NO_ERROR)

namespace GL {
	using namespace axl::glfl;
	using namespace axl::glfl::core::GL;
}

namespace axl {
namespace gl {
namespace gfx {
namespace ui {

TextView::TextView(axl::gl::Context* ptr_context, axl::gl::gfx::Font* ptr_font, axl::gl::gfx::Program* ptr_program, axl::math::Vec2i size) :
	axl::gl::gfx::UIElement(UIElement::T_TextView, ptr_context),
	enable_text_shadow(false),
	txtv_wstring(L"TextView"),
	txtv_background_color(axl::math::Vec4f(0.9f,0.9f,0.9f,0.0f)),
	txtv_text_color(axl::math::Vec4f(0.1f,0.1f,0.1f,1.0f)),
	txtv_shadow_color(axl::math::Vec4f(0.0f,0.0f,0.0f,0.69f)),
	txtv_shadow_offset(axl::math::Vec2f(0.0f,0.0f)),
	txtv_spacing(0.0f,0.0f),
	txtv_origin(0.5f,0.5f),
	txtv_padding(4.0f,4.0f,4.0f,4.0f),
	txtv_offset(0.0f,0.0f),
	txtv_box_size(0,0),
	txtv_font(ptr_font),
	txtv_program(ptr_program),
	txtv_alignment(axl::gl::gfx::ui::TextView::TAL_LEFT_CENTER),
	txtv_transform(),
	m_actual_text_length(0),
	m_rec_font_size(-1,-1),
	m_vertex_array_id(-1),
	m_vertex_buffer_id(-1),
	m_element_array_id(-1),
	m_attribute_location_position(-1),
	m_attribute_location_UV(-1),
	m_uloc_projection_matrix(-1),
	m_uloc_view_matrix(-1),
	m_uloc_model_matrix(-1),
	m_uloc_text_offset(-1),
	m_uloc_text_color(-1),
	m_uloc_depth(-1)
{
	axl::gl::gfx::UIElement::setSize(size);
	this->updateAlignment();
}
TextView::~TextView()
{
	axl::gl::gfx::UIElement::destroy();
}
bool TextView::isValid() const
{
	return axl::gl::gfx::UIElement::isValid() && this->txtv_font &&  this->txtv_program && 
		   this->ctx_context->isValid() && this->txtv_font->isValid() && this->txtv_program->isLinked() && 
		   this->m_vertex_array_id != -1 && this->m_vertex_buffer_id != -1 && this->m_element_array_id != -1 &&
		   this->m_attribute_location_position != -1 && this->m_attribute_location_UV != -1;
}
void TextView::setContext(axl::gl::Context* ptr_context)
{
	axl::gl::gfx::UIElement::setContext(ptr_context);
}
bool TextView::icreate()
{
	using namespace GL;
	if(!GL_VERSION_3_0 || !this->ctx_context || !this->ctx_context->isValid() || this->ctx_context->config.major_version < 3 ||
		!axl::gl::gfx::UIElement::icreate() || !this->txtv_font || !this->txtv_font->isValid() ||
		!this->txtv_program->isLinked() || !this->ctx_context->makeCurrent()
	)
		return false;
	GLCLEARERROR();
	glGenVertexArrays(1, &this->m_vertex_array_id);
	if(this->m_vertex_array_id == -1 || glGetError() != GL_NO_ERROR) return false;
	glGenBuffers(1, &this->m_vertex_buffer_id);
	if(this->m_vertex_buffer_id == -1 || glGetError() != GL_NO_ERROR)
	{
		glDeleteVertexArrays(1, &this->m_vertex_array_id);
		return false;
	}
	glGenBuffers(1, &this->m_element_array_id);
	if(this->m_element_array_id == -1 || glGetError() != GL_NO_ERROR)
	{
		glDeleteBuffers(1, &this->m_vertex_buffer_id);
		glDeleteVertexArrays(1, &this->m_vertex_array_id);
		return false;
	}
	if(glGetError() != GL_NO_ERROR)
		return false;
	this->setProgram(this->txtv_program);
	if(!this->updateBuffers(this->txtv_wstring, true, true))
		return false;
	this->updateAlignment();
	return true;
}
bool TextView::idestroy()
{
	using namespace GL;
	if(this->ctx_context && this->ctx_context->isValid() && this->ctx_context->makeCurrent())
	{
		GLCLEARERROR();
		if(this->m_element_array_id != -1)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, &this->m_element_array_id);
		}
		if(this->m_vertex_buffer_id != -1)
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, &this->m_vertex_buffer_id);
		}
		if(this->m_vertex_array_id != -1)
		{
			glBindVertexArray(0);
			glDeleteVertexArrays(1, &this->m_vertex_array_id);
		}
		return glGetError() == GL_NO_ERROR;
	}
	this->m_element_array_id = -1;
	this->m_vertex_buffer_id = -1;
	this->m_vertex_array_id = -1;
	this->m_attribute_location_position = -1;
	this->m_attribute_location_UV = -1;
	this->m_uloc_projection_matrix = -1;
	this->m_uloc_view_matrix = -1;
	this->m_uloc_model_matrix = -1;
	this->m_uloc_text_offset = -1;
	this->m_uloc_text_color = -1;
	this->m_uloc_depth = -1;
	return axl::gl::gfx::UIElement::idestroy();
}

// set methods

bool TextView::setSize(const axl::math::Vec2i& size)
{
	if(!axl::gl::gfx::UIElement::setSize(size))
		return false;
	this->updateOffset();
	return true;
}
void TextView::setBackgroundColor(const axl::math::Vec4f& bg_color)
{
	this->txtv_background_color = bg_color;
}
void TextView::setTextColor(const axl::math::Vec4f& text_color)
{
	this->txtv_text_color = text_color;
}
void TextView::setTextShadowColor(const axl::math::Vec4f& shadow_color)
{
	this->txtv_shadow_color = shadow_color;
}
void TextView::setTextShadowOffset(const axl::math::Vec2f& shadow_offset)
{
	this->txtv_shadow_offset = shadow_offset;
}
bool TextView::setSpacing(const axl::math::Vec2f& spacing)
{
	if(spacing.x < 0.0f || spacing.y < 0.0f)
		return false;
	this->txtv_spacing = spacing;
	this->updateBuffers(this->txtv_wstring, false, true);
	return true;
}
bool TextView::setOrigin(const axl::math::Vec2f& origin)
{
	this->txtv_origin = origin;
	this->updateOffset();
	return true;
}
bool TextView::setPadding(const axl::math::Vec4f& padding)
{
	this->txtv_padding = padding;
	this->updateOffset();
	return true;
}
bool TextView::setStorageSize(axl::util::size_t size)
{
	if(size == 0 || !this->isValid() || !this->txtv_wstring.resize(size)) return false;
	return this->updateBuffers(this->txtv_wstring, true);
}
bool TextView::setText(const axl::util::WString& wstring)
{
	if(!this->isValid()) return false;
	axl::util::size_t new_length = wstring.length();
	const bool need_more_size = new_length >= this->txtv_wstring.size();
	if(need_more_size && !this->txtv_wstring.resize(new_length)) return false;
	axl::util::WString::scwCopy(wstring.cwstr(), this->txtv_wstring.wstr(), new_length+1, 0, 0);
	this->txtv_wstring.length(true);
	return this->updateBuffers(wstring, need_more_size);
}
void TextView::setFont(const Font* font)
{
	this->txtv_font = font;
	if(font)
		this->m_rec_font_size = font->size;
	this->updateBuffers(this->txtv_wstring, false, true);
}
bool TextView::setProgram(const axl::gl::gfx::Program* text_shader_program)
{
	this->txtv_program = text_shader_program;
	if(text_shader_program)
	{
		this->m_attribute_location_position = this->txtv_program->getAttributeLocation("in_Position");
		this->m_attribute_location_UV = this->txtv_program->getAttributeLocation("in_UV");
		this->m_uloc_projection_matrix = this->txtv_program->getUniformLocation("u_MatProjection");
		this->m_uloc_view_matrix = this->txtv_program->getUniformLocation("u_MatView");
		this->m_uloc_model_matrix = this->txtv_program->getUniformLocation("u_MatModel");
		this->m_uloc_text_offset = this->txtv_program->getUniformLocation("u_TextOffset");
		this->m_uloc_text_color = this->txtv_program->getUniformLocation("u_TextColor");
		this->m_uloc_depth = this->txtv_program->getUniformLocation("u_Depth");
		return this->m_attribute_location_position != -1 &&
			this->m_attribute_location_UV != -1 &&
			this->m_uloc_projection_matrix != -1 &&
			this->m_uloc_view_matrix != -1 &&
			this->m_uloc_model_matrix != -1 &&
			this->m_uloc_text_offset != -1 &&
			this->m_uloc_text_color != -1 &&
			this->m_uloc_depth != -1;
	}
	else
	{
		this->m_attribute_location_position = -1;
		this->m_attribute_location_UV = -1;
		this->m_uloc_projection_matrix = -1;
		this->m_uloc_view_matrix = -1;
		this->m_uloc_model_matrix = -1;
		this->m_uloc_text_offset = -1;
		this->m_uloc_text_color = -1;
		this->m_uloc_depth = -1;
		return false;
	}
}
void TextView::setAlignment(AlignmentFlag alignment_flags)
{
	this->txtv_alignment = alignment_flags;
	this->updateBuffers(this->txtv_wstring, false, true);
}
void TextView::setHorizontalAlignment(HorizontalAlignment horizontal_alignment)
{
	this->txtv_alignment = (this->txtv_alignment & 0x0C) | horizontal_alignment;
	this->updateBuffers(this->txtv_wstring, false, true);
}
void TextView::setVerticalAlignment(VerticalAlignment vertical_alignment)
{
	this->txtv_alignment = (this->txtv_alignment & 0x03) | vertical_alignment;
	this->updateBuffers(this->txtv_wstring, false, true);
}

// get methods

const axl::math::Vec4f& TextView::getBackgroundColor() const
{
	return this->txtv_background_color;
}
const axl::math::Vec4f& TextView::getTextColor() const
{
	return this->txtv_text_color;
}
const axl::math::Vec4f& TextView::getTextShadowColor() const
{
	return this->txtv_shadow_color;
}
const axl::math::Vec2f& TextView::getTextShadowOffset() const
{
	return this->txtv_shadow_offset;
}
const axl::math::Vec2f& TextView::getSpacing() const
{
	return this->txtv_spacing;
}
const axl::math::Vec2f& TextView::getOrigin() const
{
	return this->txtv_origin;
}
const axl::math::Vec4f& TextView::getPadding() const
{
	return this->txtv_padding;
}
axl::math::Vec2i TextView::getMinSize() const
{
	return this->txtv_box_size;
}
axl::math::Vec2i TextView::getMaxSize() const
{
	return axl::math::Vec2i((int)((float)txtv_box_size.x + 1.0f * (txtv_padding.x + txtv_padding.z)), (int)((float)txtv_box_size.y + 1.0f * (txtv_padding.y + txtv_padding.w)));
}
const axl::util::WString& TextView::getText() const
{
	return this->txtv_wstring;
}
const axl::gl::gfx::Font* TextView::getFont() const
{
	return this->txtv_font;
}
const axl::gl::gfx::Program* TextView::getProgram() const
{
	return this->txtv_program;
}
TextView::AlignmentFlag TextView::getAlignment() const
{
	return this->txtv_alignment;
}
TextView::HorizontalAlignment TextView::getHorizontalAlignment() const
{
	switch((this->txtv_alignment & 0x03))
	{
		default:
		case HorizontalAlignment::TAL_HORIZONTAL_CENTER: return HorizontalAlignment::TAL_HORIZONTAL_CENTER;
		case HorizontalAlignment::TAL_LEFT: return HorizontalAlignment::TAL_LEFT;
		case HorizontalAlignment::TAL_RIGHT: return HorizontalAlignment::TAL_RIGHT;
	}
}
TextView::VerticalAlignment TextView::getVerticalAlignment() const
{
	switch((this->txtv_alignment & 0x0C))
	{
		default:
		case VerticalAlignment::TAL_VERTICAL_CENTER: return VerticalAlignment::TAL_VERTICAL_CENTER;
		case VerticalAlignment::TAL_TOP: return VerticalAlignment::TAL_TOP;
		case VerticalAlignment::TAL_BOTTOM: return VerticalAlignment::TAL_BOTTOM;
	}
}

void TextView::updateOffset()
{
	float hor_padding, ver_padding;
	switch((this->txtv_alignment & 0x03))
	{
		default:
		case HorizontalAlignment::TAL_LEFT: hor_padding = this->txtv_padding.x; break;
		case HorizontalAlignment::TAL_RIGHT: hor_padding = -this->txtv_padding.z; break;
		case HorizontalAlignment::TAL_HORIZONTAL_CENTER: hor_padding = 0.0f; break;
	}
	switch((this->txtv_alignment & 0x0C))
	{
		default:
		case VerticalAlignment::TAL_TOP: ver_padding = -this->txtv_padding.w; break;
		case VerticalAlignment::TAL_BOTTOM: ver_padding = this->txtv_padding.y; break;
		case VerticalAlignment::TAL_VERTICAL_CENTER: ver_padding = 0.0f; break;
	}
	txtv_offset.set(
		float(int(hor_padding + (this->txtv_origin.x * (this->uielement_size.x - this->txtv_box_size.x)))),
		float(int(ver_padding + (this->txtv_origin.y * (this->uielement_size.y - this->txtv_box_size.y))))
		);
}
void TextView::updateAlignment()
{
	switch((this->txtv_alignment & 0x03))
	{
		default:
		case HorizontalAlignment::TAL_LEFT:
			this->setOrigin(axl::math::Vec2f(0.0f, this->txtv_origin.y));
			break;
		case HorizontalAlignment::TAL_RIGHT:
			this->setOrigin(axl::math::Vec2f(1.0f, this->txtv_origin.y));
			break;
		case HorizontalAlignment::TAL_HORIZONTAL_CENTER:
			this->setOrigin(axl::math::Vec2f(0.5f, this->txtv_origin.y));
			break;
	}
	switch((this->txtv_alignment & 0x0C))
	{
		default:
		case VerticalAlignment::TAL_TOP:
			this->setOrigin(axl::math::Vec2f(this->txtv_origin.x, 1.0f));
			break;
		case VerticalAlignment::TAL_BOTTOM:
			this->setOrigin(axl::math::Vec2f(this->txtv_origin.x, 0.0f));
			break;
		case VerticalAlignment::TAL_VERTICAL_CENTER:
			this->setOrigin(axl::math::Vec2f(this->txtv_origin.x, 0.5f));
			break;
	}
}
bool TextView::updateBuffers(const axl::util::WString& p_wstring, bool text_size_changed, bool font_attribute_altered)
{
	using namespace GL;
	if(!this->isValid())
		return false;
	const bool text_changed = &p_wstring != &this->txtv_wstring;
	unsigned int text_length = this->txtv_wstring.length(), actual_length = 0, line_count = 0;
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
					unsigned int index = this->txtv_font->getCharIndex(wchr);
					if(index != -1)
					{
						const axl::gl::gfx::GlyphData& glyph = this->txtv_font->glyphs[index];
						line_width_accum += ((float)glyph.horiAdvance + this->txtv_spacing.x);
						if(glyph.height > max_line_height) max_line_height = glyph.height;
					}
				}
				++actual_length;
				continue;
		}
	}
	this->txtv_box_size.set((int)((float)max_line_width - this->txtv_spacing.x), (int)((float)line_count * this->txtv_font->size.y + (line_count - 1) * this->txtv_spacing.y));
	this->updateAlignment();
	this->m_actual_text_length = actual_length;
	const unsigned int text_count = this->txtv_wstring.size() - 1;
	const unsigned int vertex_count = 16 * text_count, indeces_count = 6 * text_count;
	GLCLEARERROR();
	glBindVertexArray(this->m_vertex_array_id);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_vertex_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_element_array_id);
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
		glEnableVertexAttribArray(this->m_attribute_location_position);
		glEnableVertexAttribArray(this->m_attribute_location_UV);
		glVertexAttribPointer(this->m_attribute_location_position, 2, GL_FLOAT, GL_FALSE, stride, 0);
		glVertexAttribPointer(this->m_attribute_location_UV, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(GLfloat) * 2));
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
		GLfloat horizontal_advance = 0.0f, vertical_advance = (GLfloat)(line_count-1) * (this->txtv_font->size.y + this->txtv_spacing.y) + 0.2f * this->txtv_font->size.y;
		const unsigned int space_glyph_index = this->txtv_font->getCharIndex(L' ');
		const GLfloat space_advance = (GLfloat)(space_glyph_index == -1 ? this->txtv_font->size.x : this->txtv_font->glyphs[space_glyph_index].horiAdvance);
		const GLfloat tab_advance = space_advance * 4.0f;
		unsigned int skipped_chars_count = 0;
		line_count = 0;
		for(unsigned int i = 0; i <= text_length; ++i)
		{
			switch(this->txtv_wstring[i])
			{
				case L'\n':
				case L'\0':
					horizontal_advance = 0.0f;
					vertical_advance -= this->txtv_font->size.y + this->txtv_spacing.y;
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
			const unsigned int glyph_index = this->txtv_font->getCharIndex(this->txtv_wstring[i]);
			const axl::gl::gfx::GlyphData& glyph = this->txtv_font->glyphs[glyph_index];
			axl::math::Vec2f offset((float)(horizontal_advance + glyph.horiBearingX), (float)(vertical_advance + glyph.horiBearingY));
			switch((this->txtv_alignment & 0x03))
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
			const axl::math::Vec2f offsetted_size((float)(offset.x + glyph.width), (float)(offset.y + glyph.height));
			const axl::math::Vec4f& UV = glyph.UV;
			horizontal_advance += (GLfloat)glyph.horiAdvance + this->txtv_spacing.x;

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
bool TextView::irender(const axl::gl::camera::Camera3Df* camera)
{
	using namespace GL;
	if(this->m_rec_font_size.notEquals(this->txtv_font->size))
		this->updateBuffers(this->txtv_wstring, false, true);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	if(this->txtv_background_color.w > 0.0f || this->enable_text_shadow)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	glClearColor(this->txtv_background_color.x, this->txtv_background_color.y, this->txtv_background_color.z, this->txtv_background_color.w);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	// render text {
	if(m_actual_text_length > 0)
	{
		if(camera)
		{
			if(camera->projection)
				this->txtv_program->setUniformMat4fv(m_uloc_projection_matrix, camera->projection->matrix.values);
			this->txtv_program->setUniformMat4fv(m_uloc_view_matrix, camera->view_transform.values);
		}
		if(this->enable_text_shadow)
		{
			this->txtv_program->setUniform1f(m_uloc_depth, -1.f);
			this->txtv_program->setUniformMat4fv(m_uloc_model_matrix, this->txtv_transform.getMatrix().values);
			this->txtv_program->setUniform2f(m_uloc_text_offset, txtv_offset.x + txtv_shadow_offset.x, txtv_offset.y + txtv_shadow_offset.y);
			this->txtv_program->setUniform4fv(m_uloc_text_color, &this->txtv_shadow_color.x);
			if(this->txtv_font->texture.bind() && this->txtv_program->use())
			{
				GLCLEARERROR();
				glBindVertexArray(this->m_vertex_array_id);
				glBindBuffer(GL_ARRAY_BUFFER, this->m_vertex_buffer_id);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_element_array_id);
				glDrawElements(GL_TRIANGLES, 6 * this->m_actual_text_length, GL_UNSIGNED_INT, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
		}
		// Text
		this->txtv_program->setUniform1f(m_uloc_depth, 0.0f);
		this->txtv_program->setUniformMat4fv(m_uloc_model_matrix, this->txtv_transform.getMatrix().values);
		this->txtv_program->setUniform2fv(m_uloc_text_offset, &this->txtv_offset.x);
		this->txtv_program->setUniform4fv(m_uloc_text_color, &this->txtv_text_color.x);
		if(this->txtv_font->texture.bind() && this->txtv_program->use())
		{
			GLCLEARERROR();
			glBindVertexArray(this->m_vertex_array_id);
			glBindBuffer(GL_ARRAY_BUFFER, this->m_vertex_buffer_id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_element_array_id);
			glDrawElements(GL_TRIANGLES, 6 * this->m_actual_text_length, GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		this->txtv_font->texture.unbind();
		this->txtv_program->unuse();
	}
	// } render text
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	return glGetError() == GL_NO_ERROR;
}

//
// TextView::Program
//

TextView::Program::Program(axl::gl::Context* ptr_context) :
	axl::gl::gfx::Program(ptr_context)
{}
TextView::Program::~Program()
{
	this->destroy();
}
bool TextView::Program::icreate()
{
	using namespace GL;
	if(!axl::gl::gfx::Program::icreate()) return false;
	Shader vertex_shader(this->ctx_context, GL_VERTEX_SHADER), fragment_shader(this->ctx_context, GL_FRAGMENT_SHADER);
	if(!vertex_shader.create() || !fragment_shader.create()) return false;
	vertex_shader.setSource(
		"# version 330 core\n"
		"layout(location = 0) in vec2 in_Position;\n"
		"layout(location = 1) in vec2 in_UV;\n"
		"uniform mat4 u_MatProjection = mat4(1);\n"
		"uniform mat4 u_MatView = mat4(1);\n"
		"uniform mat4 u_MatModel = mat4(1);\n"
		"uniform vec2 u_TextOffset = vec2(0,0);\n"
		"uniform float u_Depth = 0.0;\n"
		"out vec2 v_TexCoord;"
		"void main() {\n"
		"	float depth = u_Depth + (in_Position.x * in_Position.y) / 999999.0;\n"
		"	gl_Position = u_MatProjection * u_MatView * u_MatModel * vec4((in_Position.x + u_TextOffset.x), (in_Position.y + u_TextOffset.y), depth, 1.0);"
		"	v_TexCoord = in_UV;\n"
		"}\n"
		);
	fragment_shader.setSource(
		"# version 330 core\n"
		"in vec2 v_TexCoord;\n"
		"uniform vec4 u_TextColor = vec4(0.5,0.5,0.5,1.0);\n"
		"uniform sampler2D texture;\n"
		"void main() {\n"
		"	vec4 sample = texture2D(texture, v_TexCoord);\n"
		"	gl_FragColor = vec4(u_TextColor.rgb, u_TextColor.a * sample.r);\n"
		"}\n"
		);
	if(!vertex_shader.compile() || !fragment_shader.compile())
	{
		printf("===== TextView::VIL:\n %s\n", vertex_shader.getInfoLog().cstr());
		printf("===== TextView::FIL:\n %s\n", fragment_shader.getInfoLog().cstr());
		return false;
	}
	vertex_shader.attach(*this);
	fragment_shader.attach(*this);
	if(!this->link()) return false;
	vertex_shader.detach(*this);
	fragment_shader.detach(*this);
	return true;
}
bool TextView::Program::idestroy()
{
	return axl::gl::gfx::Program::idestroy();
}

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
