#include <axl.gl/lib.hpp>
#include <axl.gl/gfx/UI/UIManager.hpp>

namespace axl {
namespace gl {
namespace gfx {

UIManager::UIManager(axl::gl::Context* ptr_context) :
	axl::gl::ContextObject(ptr_context),
	uiman_uielements()
{}
UIManager::~UIManager()
{
	this->destroy();
}
void UIManager::setContext(axl::gl::Context* ptr_context)
{
	axl::gl::ContextObject::setContext(ptr_context);
}
bool UIManager::isValid() const
{
	return this->ctx_context && this->ctx_context->isValid() && this->isCreated();
}
bool UIManager::icreate()
{
	return true;
}
bool UIManager::idestroy()
{
	while(!this->uiman_uielements.isEmpty())
	{
		axl::gl::gfx::UIElement* uielement =  this->uiman_uielements.removeFirst();
		uielement->setUIManager(0);
	}
	return true;
}
const axl::util::ds::UniList<axl::gl::gfx::UIElement*>& UIManager::getUIElements() const
{
	return this->uiman_uielements;
}
bool UIManager::contains(axl::gl::gfx::UIElement* ui_element) const
{
	axl::util::ds::UniList<axl::gl::gfx::UIElement*>::Iterator it = this->uiman_uielements.first();
	while(!it.isNull())
	{
		if(ui_element == *it)
			return true;
	}
	return false;
}
bool UIManager::add(axl::gl::gfx::UIElement* ui_element)
{
	if(!this->isValid() || !ui_element || this->contains(ui_element) || !this->uiman_uielements.insertLast(ui_element))
		return false;
	ui_element->setUIManager(this);
	switch(ui_element->type)
	{
		default:
		case UIElement::T_UIElement: break;
		case UIElement::T_TextView: break;
	}
	return true;
}
bool UIManager::remove(axl::gl::gfx::UIElement* ui_element)
{
	if(!this->isValid() || !ui_element || !this->uiman_uielements.remove(ui_element))
		return false;
	ui_element->setUIManager(0);
	switch(ui_element->type)
	{
		default:
		case UIElement::T_UIElement: break;
		case UIElement::T_TextView: break;
	}
	return true;
}

} // axl.gl.gfx
} // axl.gl
} // axl
