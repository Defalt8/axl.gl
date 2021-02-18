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
	text_position(0.0f, 0.0f, 0.0f),
	text_scale(1.0f, 1.0f, 1.0f),
	text_rotation(0.0f, 0.0f, 0.0f),
	text_transform(),
	text_color(0.0f, 0.0f, 0.0f, 1.0f),
	text_wstring(),
	text_font(),
	text_orientation(Text::Orientation::TOR_HORIZONTAL),
	text_alignment(Text::Alignment::TAL_LEFT|Text::Alignment::TAL_BOTTOM),
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
	uniform_location_text_color(-1)
{
	this->updateTransform();
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
	if(this->attribute_location_position == -1 || this->attribute_location_UV == -1 || 
		this->uniform_location_projection_matrix == -1 || this->uniform_location_view_matrix == -1 || 
		this->uniform_location_model_matrix == -1
	)
	{
		this->idestroy();
		return false;
	}
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
		   this->vertex_array_id != -1 && this->vertex_buffer_id != -1 && this->element_array_id != -1;
}
bool Text::render(const axl::gl::camera::Camera3Df* camera) const
{
	using namespace GL;
	if(!this->isValid()) return false;
	if(actual_text_length > 0)
	{
		if(camera)
		{
			if(camera->projection)
				this->text_program->setUniformMat4fv(uniform_location_projection_matrix, camera->projection->matrix.values);
			this->text_program->setUniformMat4fv(uniform_location_view_matrix, camera->view_transform.values);
		}
		this->text_program->setUniformMat4fv(uniform_location_model_matrix, this->text_transform.values);
		this->text_program->setUniform4fv(uniform_location_text_color, &this->text_color.x);
		if(!this->text_program->use() || !this->text_font->texture.bind()) return false;
		GLCLEARERROR();
		glBindVertexArray(this->vertex_array_id);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->element_array_id);
		glDrawElements(GL_TRIANGLES, 6 * this->actual_text_length, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
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
void Text::updateTransform()
{
	this->text_transform = axl::math::Transform4::translateScale(this->text_position, this->text_scale) * axl::math::Transform4::rotateXYZ(this->text_rotation);
}
// set methods
void Text::setPosition(const axl::math::Vec3f& p_position)
{
	this->text_position = p_position;
	this->updateTransform();
}
void Text::setScale(const axl::math::Vec3f& p_scale)
{
	this->text_scale = p_scale;
	this->updateTransform();
}
void Text::setRotation(const axl::math::Vec3f& p_rotation)
{
	this->text_rotation = p_rotation;
	this->updateTransform();
}
void Text::setColor(const axl::math::Vec4f& p_color)
{
	this->text_color = p_color;
}
bool Text::setText(const axl::util::WString& p_wstring)
{
	using namespace GL;
	if(!this->isValid()) return false;
	unsigned int text_length = p_wstring.length(), actual_length = 0;
	if(text_length > 0)
	{
		for(unsigned int i = 0; i < text_length; ++i)
		{
			switch(p_wstring[i])
			{
				case L'\n':
				case L'\r':
				case L'\t':
				case L'\0':
					continue;
				default:
					++actual_length;
					continue;
			}
		}
		this->actual_text_length = actual_length;
		const unsigned int vertex_count = 16 * actual_length, indeces_count = 6 * actual_length;
		const GLsizei stride = sizeof(GLfloat) * 4;
		glBindVertexArray(this->vertex_array_id);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLfloat) * vertex_count), 0, GL_DYNAMIC_DRAW);
		if(glGetError() != GL_NO_ERROR)
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			return false;
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->element_array_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(GLuint) * indeces_count), 0, GL_DYNAMIC_DRAW);
		if(glGetError() != GL_NO_ERROR)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			return false;
		}
		GLfloat* buffer = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if(!buffer)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			return false;
		}
		GLuint* indeces = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		if(!indeces)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			return false;
		}
		GLfloat horizontal_advance = 0.0f, vertical_advance = 0.0f;
		const unsigned int space_glyph_index = this->text_font->getCharIndex(L' ');
		const GLfloat space_advance = (GLfloat)(space_glyph_index == -1 ? this->text_font->size.x : this->text_font->glyphs[space_glyph_index].horiAdvance);
		const GLfloat tab_advance = space_advance * 4.0f;
		unsigned int skipped_chars_count = 0;
		for(unsigned int i = 0; i < text_length; ++i)
		{
			switch(p_wstring[i])
			{
				case L'\n':
					horizontal_advance = 0;
					vertical_advance -= this->text_font->size.y;
					++skipped_chars_count;
					continue;
				case L'\r':
					horizontal_advance = 0;
					++skipped_chars_count;
					continue;
				case L'\t':
					horizontal_advance += tab_advance;
					++skipped_chars_count;
					continue;
			}
			const unsigned int i_actual = i - skipped_chars_count;
			const unsigned int base_vertex_index = i_actual * 16, base_indeces_index = i_actual * 6, vertex_indeces_index = i_actual * 4;
			const unsigned int glyph_index = this->text_font->getCharIndex(p_wstring[i]);
			const axl::gl::gfx::Font::GlyphData& glyph = this->text_font->glyphs[glyph_index];
			const axl::math::Vec2f offset((float)(horizontal_advance + glyph.horiBearingX), (float)(vertical_advance + glyph.horiBearingY - glyph.height));
			const axl::math::Vec2f offsetted_size((float)(offset.x + glyph.width), (float)(offset.y + glyph.height));
			const axl::math::Vec4f& UV = glyph.UV;
			horizontal_advance += (GLfloat)glyph.horiAdvance;

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

		glEnableVertexAttribArray(this->attribute_location_position);
		glEnableVertexAttribArray(this->attribute_location_UV);
		glVertexAttribPointer(this->attribute_location_position, 2, GL_FLOAT, GL_FALSE, stride, 0);
		glVertexAttribPointer(this->attribute_location_UV, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(GLfloat) * 2));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	this->text_wstring = p_wstring;
	return true;
}
void Text::setFont(const Font* p_font)
{
	this->text_font = p_font;
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
		this->uniform_location_text_color = this->text_program->getUniformLocation("u_TextColor");
	}
	else
	{
		this->attribute_location_position = -1;
		this->attribute_location_UV = -1;
		this->uniform_location_projection_matrix = -1;
		this->uniform_location_view_matrix = -1;
		this->uniform_location_model_matrix = -1;
		this->uniform_location_text_color = -1;
	}
}
void Text::setOrientation(Text::Orientation p_orientation)
{
	this->text_orientation = p_orientation;
}
void Text::setAlignment(AlignmentFlag p_alignment_flags)
{
	this->text_alignment = p_alignment_flags;
}
// get methods
const axl::math::Vec3f& Text::getPosition() const
{
	return this->text_position;
}
const axl::math::Vec3f& Text::getScale() const
{
	return this->text_scale;
}
const axl::math::Vec3f& Text::getRotation() const
{
	return this->text_rotation;
}
const axl::math::Mat4f& Text::getTransform() const
{
	return this->text_transform;
}
const axl::math::Vec4f& Text::getColor() const
{
	return this->text_color;
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
Text::Orientation Text::getOrientation() const
{
	return this->text_orientation;
}
Text::AlignmentFlag Text::getAlignment() const
{
	return this->text_alignment;
}

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
