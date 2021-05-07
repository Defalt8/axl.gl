#include <axl.gl/gfx/UI/Group.hpp>
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

Group::Group(axl::gl::Context* ptr_context) :
	axl::gl::gfx::UIElement(axl::gl::gfx::UIElement::T_Group, ptr_context),
	grp_children(),
	grp_layout(),
	grp_background_color(0.0f,0.0f,0.0f,0.0f)
{}
Group::~Group()
{
	this->destroy();
}

bool Group::contains(axl::gl::gfx::UIElement* ui_element) const
{
	axl::util::ds::UniList<axl::gl::gfx::UIElement*>::Iterator it = this->grp_children.first();
	while(!it.isNull())
	{
		if(ui_element == *it)
			return true;
		++it;
	}
	return false;
}
bool Group::add(axl::gl::gfx::UIElement* ui_element, bool reorganize)
{
	if(!this->isValid() || !ui_element || this->contains(ui_element))
		return false;
	switch(ui_element->type)
	{
		default:
		case UIElement::T_UIElement:
		case UIElement::T_Group:
		case UIElement::T_TextView:
			return ui_element->setUIGroup(this) && this->grp_children.insertLast(ui_element);
	}
	if(reorganize)
		this->organize();
	return true;
}
bool Group::remove(axl::gl::gfx::UIElement* ui_element, bool reorganize)
{
	if(!this->isValid() || !ui_element || !this->grp_children.remove(ui_element))
		return false;
	ui_element->setUIGroup(0);
	if(reorganize)
		this->organize();
	return true;
}
const axl::util::ds::UniList<axl::gl::gfx::UIElement*>& Group::getChildren() const
{
	return this->grp_children;
}
void Group::setBackgroundColor(const axl::math::Vec4f& background_color)
{
	this->grp_background_color = background_color;
	this->uielement_modified = true;
}
const axl::math::Vec4f& Group::getBackgroundColor() const
{
	return this->grp_background_color;
}
void Group::setPadding(const axl::math::Vec4f& padding)
{
	this->grp_padding = padding;
	this->organize();
}
const axl::math::Vec4f& Group::getPadding() const
{
	return this->grp_padding;
}

void Group::setLayout(axl::gl::gfx::ui::Layout* ptr_layout)
{
	this->grp_layout = ptr_layout;
	this->organize();
}
axl::gl::gfx::ui::Layout* Group::getLayout() const
{
	return this->grp_layout;
}

axl::math::Vec2i Group::getMinSize() const
{
	return this->uielement_size;
}
// axl::math::Vec2i Group::getMaxSize() const
// {
// 	if(this->grp_layout)
// 	{
// 		// return this->grp_layout->getMaxSize(*this, this->grp_children);
// 	}
// 	else
// 	{
// 		return axl::math::Vec2i(
// 			this->uielement_size.x + (int)(this->uielement_border_size.x + this->uielement_border_size.z),
// 			this->uielement_size.y + (int)(this->uielement_border_size.y + this->uielement_border_size.w)
// 			);
// 	}
}

bool Group::isValid() const
{
	return axl::gl::gfx::UIElement::isValid();
}
bool Group::icreate()
{
	if(!axl::gl::gfx::UIElement::icreate())
		return false;
	axl::util::ds::UniList<axl::gl::gfx::UIElement*>::Iterator it = this->grp_children.first();
	bool all_succeeded = true;
	while(!it.isNull())
	{
		(*it)->setContext(this->ctx_context);
		if(!(*it)->create())
			all_succeeded = false;
		++it;
	}
	return all_succeeded;
}
bool Group::idestroy()
{
	if(!axl::gl::gfx::UIElement::idestroy())
		return false;
	axl::util::ds::UniList<axl::gl::gfx::UIElement*>::Iterator it = this->grp_children.first();
	bool all_succeeded = true;
	while(!it.isNull())
	{
		if(!(*it)->destroy())
			all_succeeded = false;
		++it;
	}
	return all_succeeded;
}
bool Group::irender(const camera::Camera3Df* camera)
{
	using namespace GL;
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);
	glClearColor(this->grp_background_color.x, this->grp_background_color.y, this->grp_background_color.z, this->grp_background_color.w);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	axl::util::ds::UniList<axl::gl::gfx::UIElement*>::Iterator it = this->grp_children.first();
	bool all_succeeded = true;
	while(!it.isNull())
	{
		if(!(*it)->render(camera, &this->uielement_frame_buffer))
			all_succeeded = false;
		++it;
	}
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	return all_succeeded;
}
void Group::organize()
{
	this->uielement_modified = true;
	// if(this->grp_layout)
		// this->grp_layout->organize(*this, this->grp_children);
}

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
