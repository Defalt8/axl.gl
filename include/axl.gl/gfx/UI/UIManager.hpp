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
	public:
		void onDisplayConfig(const axl::gl::Display& display);
		void onSize(int w, int h);
		void onKey(axl::gl::input::KeyCode key_code, bool is_down);
		void onChar(wchar_t char_code);
		void onPointer(int index, int x, int y, bool is_down);
		void onPointerMove(int index, int x, int y);
		void onScroll(bool is_vertical, int delta, int x, int y);
	protected:
		axl::util::ds::UniList<axl::gl::gfx::UIElement*> uiman_uielements;
		axl::util::ds::UniList<axl::gl::gfx::UIElement*> uiman_groups;
		axl::util::ds::UniList<axl::gl::gfx::UIElement*> uiman_text_views;
};

} // axl.gl.gfx
} // axl.gl
} // axl
