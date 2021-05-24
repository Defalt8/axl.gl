#pragma once
#include "../../lib.hpp"
#include "../../Context.hpp"
#include "../../ContextObject.hpp"
#include "../../camera/Camera3D.hpp"
#include "Layout.hpp"
#include <axl.util/ds/List.hpp>
#include <axl.math/vec/Vec2f.hpp>
#include <axl.math/vec/Vec3f.hpp>
#include <axl.math/vec/Vec4f.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {

class AXLGLCXXAPI Container;

class AXLGLCXXAPI Element : public axl::gl::gfx::ui::Component
{
	public:
		enum Type {
			OTHER,
			IMAGE_VIEW,
			TEXT_VIEW,
			TEXT_INPUT,
			BUTTON,
			LIST
		};
	public:
		Element(Element::Type type,
			axl::gl::Context* ptr_context = 0,
			axl::gl::gfx::ui::Container* container = 0,
			const axl::math::Vec3f& position = axl::math::Vec3f(0.0f,0.0f,0.0f),
			const axl::math::Vec2f& size = axl::math::Vec2f(0,0),
			const axl::math::Vec4f& padding = axl::math::Vec4f(0.0f,0.0f,0.0f,0.0f),
			axl::gl::gfx::ui::Layout::Size layout_width = axl::gl::gfx::ui::Layout::WRAP_CONTENT,
			axl::gl::gfx::ui::Layout::Size layout_height = axl::gl::gfx::ui::Layout::WRAP_CONTENT);
		virtual ~Element();
	public:
		virtual bool isValid() const;
		virtual axl::math::Vec2f getContentSize() const;
		axl::gl::gfx::ui::Layout::Size getLayoutWidth() const;
		axl::gl::gfx::ui::Layout::Size getLayoutHeight() const;
		void setLayoutWidth(axl::gl::gfx::ui::Layout::Size layout_width);
		void setLayoutHeight(axl::gl::gfx::ui::Layout::Size layout_height);
	protected:
		virtual bool iRender(axl::gl::camera::Camera3Df* camera) = 0;
	public:
		const axl::gl::gfx::ui::Element::Type element_type;
	protected:
		axl::gl::gfx::ui::Layout::Size element_layout_width;
		axl::gl::gfx::ui::Layout::Size element_layout_height;
};

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
