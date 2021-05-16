#pragma once
#include "../../lib.hpp"
#include "Component.hpp"

namespace axl {
namespace gl {
namespace gfx {
namespace ui {

class AXLGLCXXAPI Layout;

class AXLGLCXXAPI Container : public axl::gl::gfx::ui::Component
{
	public:
		Container(axl::gl::Context* ptr_context = 0,
			axl::gl::gfx::ui::Container* container = 0,
			Layout* layout = 0,
			const axl::math::Vec3f& position = axl::math::Vec3f(0.0f,0.0f,0.0f),
			const axl::math::Vec2i& size = axl::math::Vec2i(0,0),
			const axl::math::Vec4f& padding = axl::math::Vec4f(0.0f,0.0f,0.0f,0.0f));
		virtual ~Container();
	public:
		virtual bool addComponent(axl::gl::gfx::ui::Component* component);
		virtual bool removeComponent(axl::gl::gfx::ui::Component* component);
		bool containsComponent(axl::gl::gfx::ui::Component* component);
		const axl::util::ds::UniList<axl::gl::gfx::ui::Component*>& getComponents() const;
		void organize() const;
		Layout* getLayout() const;
		void setLayout(Layout* layout);
	protected:
		virtual bool iRender(axl::gl::camera::Camera3Df* camera);
	private:
		axl::util::ds::UniList<axl::gl::gfx::ui::Component*> m_components;
		Layout *m_layout;
};

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
