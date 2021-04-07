#include <axl.gl/lib.hpp>
#include <axl.gl/gfx/UI/TextView.hpp>
#include <axl.glfl/glCoreARB.hpp>

namespace GL {
	using namespace axl::glfl;
	using namespace axl::glfl::core::GL;
}

namespace axl {
namespace gl {
namespace gfx {
namespace ui {

TextView::TextView(axl::gl::Context* ptr_context, axl::gl::gfx::Font* ptr_font, axl::gl::gfx::Program* ptr_program, axl::math::Vec2i size) :
	axl::gl::gfx::UIElement(ptr_context),
	axl::gl::gfx::Text(ptr_context),
	txtv_background_color(axl::math::Vec4f(0.9f,0.9f,0.95f,1.0f))
{
	axl::gl::gfx::Text::setColor(axl::math::Vec4f(0.1f,0.1f,0.1f,1.0f));
	axl::gl::gfx::Text::setFont(ptr_font);
	axl::gl::gfx::Text::setProgram(ptr_program);
	axl::gl::gfx::UIElement::setSize(size);
}
TextView::~TextView()
{
	axl::gl::gfx::UIElement::destroy();
}
bool TextView::isValid() const
{
	return axl::gl::gfx::UIElement::isValid() &&
		axl::gl::gfx::Text::isValid();
}
void TextView::setContext(axl::gl::Context* ptr_context)
{
	axl::gl::gfx::UIElement::setContext(ptr_context);
	axl::gl::gfx::Text::setContext(ptr_context);
}
bool TextView::icreate()
{
	if(!axl::gl::gfx::UIElement::icreate() || !axl::gl::gfx::Text::icreate())
		return false;
	axl::gl::gfx::Text::setAlignment(axl::gl::gfx::Text::TAL_CENTER);
	axl::gl::gfx::Text::setText(L"Hello World!");
	axl::gl::gfx::Text::text_transform.setPosition(axl::math::Vec2f((uielement_size.x) / 2.0f, (uielement_size.y) / 2.0f));
	return true;
}
bool TextView::idestroy()
{
	return axl::gl::gfx::UIElement::idestroy() && axl::gl::gfx::Text::idestroy();
}
void TextView::irender(const axl::gl::camera::Camera3Df* camera)
{
	using namespace GL;
	if(!this->isValid()) return;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
	glClearColor(this->txtv_background_color.x, this->txtv_background_color.y, this->txtv_background_color.z, this->txtv_background_color.w);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	this->text_transform.setPosition(axl::math::Vec2f((uielement_size.x) / 2.0f, (uielement_size.y) / 2.0f));
	axl::gl::gfx::Text::render_text(camera);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
}

void TextView::setBackgroundColor(const axl::math::Vec4f& color)
{
	txtv_background_color = color;
}
const axl::math::Vec4f& TextView::getBackgroundColor() const
{
	return this->txtv_background_color;
}

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
