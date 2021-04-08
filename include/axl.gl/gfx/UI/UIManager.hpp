#pragma once
#include "../../lib.hpp"
#include "../../View.hpp"
#include "../../Context.hpp"
#include "../../ContextObject.hpp"
#include "UIElement.hpp"
#include <axl.math/vec/Vec2i.hpp>
#include <axl.util/ds/List.hpp>

namespace axl {
namespace gl {
namespace gfx {

class AXLGLCXXAPI UIManager : public axl::gl::ContextObject
{
	public:
		UIManager(axl::gl::Context* ptr_context = 0);
		~UIManager();
		void setContext(axl::gl::Context* ptr_context);
		bool isValid() const;
	protected:
		bool icreate();
		bool idestroy();
	public:
		const axl::util::ds::UniList<axl::gl::gfx::UIElement*>& getUIElements() const;
		bool contains(axl::gl::gfx::UIElement* ui_element) const;
		bool add(axl::gl::gfx::UIElement* ui_element);
		bool remove(axl::gl::gfx::UIElement* ui_element);
	protected:
		axl::util::ds::UniList<axl::gl::gfx::UIElement*> uiman_uielements;
};

} // axl.gl.gfx
} // axl.gl
} // axl
