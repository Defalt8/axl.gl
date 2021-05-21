#include <axl.gl/gfx/ui/elements/TextInput.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {
namespace elements {

TextInput::TextInput(axl::gl::Context* ptr_context,
			axl::gl::gfx::ui::Container* container,
			const axl::math::Vec3f& position,
			const axl::math::Vec2i& size,
			const axl::math::Vec4f& padding,
			axl::gl::gfx::ui::Layout::Size layout_width,
			axl::gl::gfx::ui::Layout::Size layout_height) :
	axl::gl::gfx::ui::Element(axl::gl::gfx::ui::Element::TEXT_INPUT, ptr_context, container, position, size, padding, layout_width, layout_height),
	text_input_horizontal_text_alignment(HAL_CENTER),
	text_input_vertical_text_alignment(VAL_CENTER),
	text_input_text(ptr_context),
	text_input_cursor(ptr_context),
	m_draw_cursor(false),
	m_cursor_index(0)
{}
TextInput::~TextInput()
{
	this->destroy();
}

void TextInput::setContext(axl::gl::Context* ptr_context)
{
	axl::gl::gfx::ui::Element::setContext(ptr_context);
	text_input_text.setContext(ptr_context);
	text_input_cursor.setContext(ptr_context);
}
bool TextInput::isValid() const
{
	return axl::gl::gfx::ui::Element::isValid() && text_input_text.isValid();
}
const axl::util::WString& TextInput::getText() const
{
	return text_input_text.getText();
}
bool TextInput::setText(const axl::util::WString& text)
{
	if(!text_input_text.setText(text)) return false;
	axl::util::size_t text_length = text.length();
	this->onModify();
	return true;
}
void TextInput::setFont(const axl::gl::gfx::Font* font)
{
	text_input_text.setFont(font);
	text_input_cursor.setFont(font);
}
bool TextInput::setTextProgram(const axl::gl::gfx::Text::Program* text_input_program)
{
	return text_input_text.setTextProgram(text_input_program) &&
		text_input_cursor.setTextProgram(text_input_program);
}
bool TextInput::setSize(const axl::math::Vec2i& size)
{
	if(!axl::gl::gfx::ui::Element::setSize(size))
		return false;
		this->onAlignmentChange();
	return true;
}

void TextInput::setHorizontalAlignment(HorizontalTextAlignment horizontal_text_alignment)
{
	text_input_horizontal_text_alignment = horizontal_text_alignment;
	this->onAlignmentChange();
}
void TextInput::setVerticalAlignment(VerticalTextAlignment vertical_text_alignment)
{
	text_input_vertical_text_alignment = vertical_text_alignment;
	this->onAlignmentChange();
}
void TextInput::setForegroundColor(const axl::math::Vec4f& foreground_color)
{
	axl::gl::gfx::ui::Element::setForegroundColor(foreground_color);
	text_input_text.setColor(this->component_foreground_color);
}

TextInput::HorizontalTextAlignment TextInput::getHorizontalAlignment() const
{
	return text_input_horizontal_text_alignment;
}
TextInput::VerticalTextAlignment TextInput::getVerticalAlignment() const
{
	return text_input_vertical_text_alignment;
}

bool TextInput::iCreate()
{
	if(!axl::gl::gfx::ui::Element::iCreate() || !text_input_text.create() || !text_input_cursor.create()) return false;
	this->setHorizontalAlignment(HAL_LEFT);
	text_input_cursor.setText(L"|");
	text_input_cursor.setColor(axl::math::Vec4f(.1f,.1f,.2f,0.5f));
	text_input_cursor.setAlignment(axl::gl::gfx::Text::TAL_BOTTOM_LEFT);
	return true;
}
bool TextInput::iDestroy()
{
	return axl::gl::gfx::ui::Element::iDestroy() &&
		text_input_text.destroy() &&
		text_input_cursor.destroy();
}
bool TextInput::iRender(axl::gl::camera::Camera3Df* camera)
{
	if(m_draw_cursor)
	{
		text_input_cursor.render(camera);
	}
	return text_input_text.render(camera);
}
axl::math::Vec2i TextInput::getContentSize() const
{
	const axl::math::Vec2f& text_box = text_input_text.getBox();
	return axl::math::Vec2i((int)text_box.x, (int)text_box.y);
}

void TextInput::onAlignmentChange()
{
	// if(!axl::gl::gfx::ui::elements::TextInput::isValid())
	// 	return;
	axl::math::Vec2f text_input_size = text_input_text.getBox();
	axl::math::Vec3f position(0.f,0.f,0.1f);
	switch(text_input_horizontal_text_alignment)
	{
		case HAL_CENTER:
			text_input_text.setHorizontalAlignment(axl::gl::gfx::Text::TAL_HORIZONTAL_CENTER);
			position.x = (float)this->component_size.x / 2.f;
			break;
		case HAL_LEFT:
			text_input_text.setHorizontalAlignment(axl::gl::gfx::Text::TAL_LEFT);
			position.x = component_padding.x;
			break;
		case HAL_RIGHT:
			text_input_text.setHorizontalAlignment(axl::gl::gfx::Text::TAL_RIGHT);
			position.x = (float)this->component_size.x - component_padding.z;
			break;
	}
	switch(text_input_vertical_text_alignment)
	{
		case VAL_CENTER:
			text_input_text.setVerticalAlignment(axl::gl::gfx::Text::TAL_VERTICAL_CENTER);
			position.y = (float)this->component_size.y / 2.f;
			break;
		case VAL_BOTTOM:
			text_input_text.setVerticalAlignment(axl::gl::gfx::Text::TAL_BOTTOM);
			position.y = component_padding.y;
			break;
		case VAL_TOP:
			text_input_text.setVerticalAlignment(axl::gl::gfx::Text::TAL_TOP);
			position.y = (float)this->component_size.y - component_padding.w;
			break;
	}
	text_input_text.text_transform.setPosition(position);
	const axl::math::Vec2i& font_size = text_input_text.getFont()->size;
	text_input_cursor.text_transform.setPosition(axl::math::Vec3f(position.x - 0.3f * font_size.x, position.y - 0.3f * font_size.y,.0f));
}
bool TextInput::onInputFocusRequest() const
{
	return true;
}
void TextInput::onInputFocusGain()
{
	m_draw_cursor = true;
	this->setCursorIndex(this->text_input_text.getText().length()-1);
	this->onModify();
}
void TextInput::onInputFocusLost()
{
	m_draw_cursor = false;
	this->onModify();
}
void TextInput::onChar(axl::util::WString::char_t char_code)
{
	if(!this->isValid())
		return;
	switch(char_code)
	{
		case L'\n':
		case L'\0':
		case L'\r':
		case L'\t':
			return;
	}
	const axl::util::WString& wstring = text_input_text.getText();
	axl::util::size_t len = wstring.length();
	if(char_code == L'\b') // backspace char
	{
		if(m_cursor_index >= len || len == 0)
			return;
		if(len == 1)
		{
			this->setText(L"");
			m_cursor_index = -1;
			this->onCursorPositionChange();
		}
		else
		{
			axl::util::WString wstr_buffer(len-1);
			if(m_cursor_index != -1 && m_cursor_index >= 0)
			{
				wstr_buffer.scwCopy(wstring.cwstr(), wstr_buffer.wstr(), m_cursor_index);
				if(m_cursor_index < len - 1)
					wstr_buffer.scwCopy(wstring.cwstr(), wstr_buffer.wstr(), (len-m_cursor_index-1), m_cursor_index+1, m_cursor_index);
				wstr_buffer[len-1] = L'\0';
				wstr_buffer.length(true);
				this->setText(wstr_buffer);
				--m_cursor_index;
				this->onCursorPositionChange();
			}
		}
	}
	else
	{
		if(len == 0)
		{
			axl::util::WString wstr_buffer(1);
			wstr_buffer[0] = char_code;
			wstr_buffer[1] = L'\0';
			wstr_buffer.length(true);
			this->setText(wstr_buffer);
			m_cursor_index = 0;
			this->onCursorPositionChange();
		}
		else if(m_cursor_index != -1 && m_cursor_index > len)
			return;
		else
		{
			axl::util::WString wstr_buffer(len+1);
			if(m_cursor_index == -1)
			{
				wstr_buffer[0] = char_code;
				wstr_buffer.scwCopy(wstring.cwstr(), wstr_buffer.wstr(), (len-m_cursor_index-1), 0, 1);
				m_cursor_index = 0;
			}
			else
			{
				wstr_buffer.scwCopy(wstring.cwstr(), wstr_buffer.wstr(), m_cursor_index+1);
				wstr_buffer[m_cursor_index+1] = char_code;
				if(m_cursor_index < len-1)
					wstr_buffer.scwCopy(wstring.cwstr(), wstr_buffer.wstr(), (len-m_cursor_index), m_cursor_index+1, m_cursor_index+2);
				wstr_buffer[len+1] = L'\0';
				wstr_buffer.length(true);
				this->setText(wstr_buffer);
				++m_cursor_index;
			}
			this->onCursorPositionChange();
		}
	}
}
bool TextInput::setCursorIndex(axl::util::size_t index)
{
	if(!this->isValid())
		return false;
	axl::util::size_t len = text_input_text.getText().length();
	if(index != -1 && index >= len)
		return false;
	m_cursor_index = index;
	this->onCursorPositionChange();
	this->onModify();
	return true;
}
axl::util::size_t TextInput::getCursorIndex() const
{
	return m_cursor_index;
}

void TextInput::onCursorPositionChange()
{
	if(!this->isValid())
		return;
	if(m_cursor_index == -1)
	{
		axl::math::Vec3f text_position = text_input_text.text_transform.getPosition();
		const axl::math::Vec2i& font_size = text_input_text.getFont()->size;
		text_input_cursor.text_transform.setPosition(axl::math::Vec3f(text_position.x - 0.1f * font_size.x, text_position.y - 0.3f * font_size.y,.0f));
	}
	else
	{
		axl::math::Vec3f position = text_input_text.text_transform.getPosition();
		axl::math::Vec4f char_box = text_input_text.getCharBox(m_cursor_index);
		axl::math::Vec3f cursor_position(position.x + char_box.z, position.y + char_box.y - (char_box.w - char_box.y)*0.2f, 0.f);
		text_input_cursor.text_transform.setPosition(cursor_position);
	}
}

} // axl.gl.gfx.ui.elements
} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
