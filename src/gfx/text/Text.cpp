#include <axl.gl/Context.hpp>
#include <axl.gl/ContextObject.hpp>
#include <axl.gl/gfx/text/Font.hpp>
#include <axl.gl/gfx/text/Text.hpp>
#include <axl.glfl/gl.hpp>
#include <axl.glfl/glCoreARB.hpp>

namespace axl {
namespace gl {
namespace gfx {

Text::Text(axl::gl::Context* ptr_context) :
	text_position(),
	text_scale(),
	text_rotation(),
	text_transform(),
	text_box_size(),
	text_wstring(),
	text_font(),
	text_orientation(Text::Orientation::TOR_HORIZONTAL),
	text_alignment(Text::Alignment::TAL_LEFT|Text::Alignment::TAL_BOTTOM),
	text_program()
{}
Text::~Text()
{}
bool Text::icreate()
{
	return false;
}
bool Text::idestroy()
{
	return true;
}
bool Text::isValid() const
{
	return this->ctx_context && this->text_font && this->text_program && this->ctx_context->isValid() && this->text_font->isValid() && this->text_program->isValid();
}
bool Text::render() const
{
	return false;
}
// set methods
void Text::setPosition(const axl::math::Vec3f& p_position)
{
	this->text_position = p_position;
}
void Text::setScale(const axl::math::Vec3f& p_scale)
{
	this->text_scale = p_scale;
}
void Text::setRotation(const axl::math::Vec3f& p_rotation)
{
	this->text_rotation = p_rotation;
}
void Text::setTransform(const axl::math::Mat4f& p_transform)
{
	this->text_transform = p_transform;
}
void Text::setBoxSize(const axl::math::Vec2f& p_box_size)
{
	this->text_box_size = p_box_size;
}
void Text::setText(const axl::util::WString& p_wstring)
{
	this->text_wstring = p_wstring;
}
void Text::setFont(const Font* p_font)
{
	this->text_font = p_font;
}
void Text::setProgram(const axl::gl::gfx::Program* text_shader_program)
{
	this->text_program = text_shader_program;
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
const axl::math::Vec2f& Text::getBoxSize() const
{
	return this->text_box_size;
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
