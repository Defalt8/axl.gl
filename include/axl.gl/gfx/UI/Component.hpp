#pragma once
#include "../../lib.hpp"
#include "../../Context.hpp"
#include "../../ContextObject.hpp"
#include "../../camera/Camera3D.hpp"
#include <axl.util/ds/List.hpp>
#include <axl.math/vec/Vec2f.hpp>
#include <axl.math/vec/Vec3f.hpp>
#include <axl.math/vec/Vec4f.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {

class AXLGLCXXAPI Container;

class AXLGLCXXAPI Component : public axl::gl::ContextObject
{
	public:
		enum Type {
			CONTAINER,
			ELEMENT
		};
	public:
		Component(Type type, axl::gl::Context* ptr_context,
			axl::gl::gfx::ui::Container* container,
			const axl::math::Vec3f& position,
			const axl::math::Vec2f& size,
			const axl::math::Vec4f& margin,
			const axl::math::Vec4f& padding);
		virtual ~Component();
	public:
		virtual bool render(axl::gl::camera::Camera3Df* camera = 0) = 0;
		virtual bool setContainer(axl::gl::gfx::ui::Container* container);
		void setVisiblity(bool is_visible);
		void setPosition(const axl::math::Vec3f& position);
		void setSize(const axl::math::Vec2f& size);
		void setMargin(const axl::math::Vec4f& margin);
		void setPadding(const axl::math::Vec4f& padding);
		axl::gl::gfx::ui::Container* getContainer() const;
		bool isVisible() const;
		const axl::math::Vec3f& getPosition() const;
		const axl::math::Vec2f& getSize() const;
		const axl::math::Vec4f& getMargin() const;
		const axl::math::Vec4f& getPadding() const;
	public:
		const Type component_type;
	protected:
		axl::gl::gfx::ui::Container* component_container;
		bool component_is_visible;
		axl::math::Vec3f component_position;
		axl::math::Vec2f component_size;
		axl::math::Vec4f component_margin;
		axl::math::Vec4f component_padding;
		friend class axl::gl::gfx::ui::Container;
};

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl

