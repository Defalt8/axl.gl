#pragma once
#include "../../lib.hpp"
#include "UIElement.hpp"
#include "Layout.hpp"

namespace axl {
namespace gl {
namespace gfx {

class AXLGLCXXAPI UIElement;

namespace ui {

class AXLGLCXXAPI Group : public axl::gl::gfx::UIElement
{
	public:
		Group(axl::gl::Context* ptr_context = 0);
		virtual ~Group();
	public:
		bool contains(axl::gl::gfx::UIElement* ui_element) const;
		bool add(axl::gl::gfx::UIElement* ui_element, bool reorganize = true);
		bool remove(axl::gl::gfx::UIElement* ui_element, bool reorganize = true);
		const axl::util::ds::UniList<axl::gl::gfx::UIElement*>& getChildren() const;
		void setBackgroundColor(const axl::math::Vec4f& background_color);
		const axl::math::Vec4f& getBackgroundColor() const;
		void setPadding(const axl::math::Vec4f& padding);
		const axl::math::Vec4f& getPadding() const;
		void setLayout(axl::gl::gfx::ui::Layout* ptr_layout);
		axl::gl::gfx::ui::Layout* getLayout() const;
		axl::math::Vec2i getMinSize() const;
		axl::math::Vec2i getMaxSize() const;
		void organize();
	protected:
		bool isValid() const;
		bool icreate();
		bool idestroy();
		bool irender(const camera::Camera3Df* camera);
	protected:
		friend class axl::gl::gfx::ui::Layout;
		axl::util::ds::UniList<axl::gl::gfx::UIElement*> grp_children;
		axl::gl::gfx::ui::Layout* grp_layout;
		axl::math::Vec4f grp_background_color;
		axl::math::Vec4f grp_padding;
};

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
