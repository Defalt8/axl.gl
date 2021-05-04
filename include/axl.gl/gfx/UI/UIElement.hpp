#pragma once
#include "../../lib.hpp"
#include "../../camera/Camera3D.hpp"
#include "../buffer/FrameBuffer.hpp"
#include "../texture/Texture2D.hpp"
#include "../shader/Program.hpp"
#include "UIFrameBuffer.hpp"
#include <axl.math/vec/Vec2i.hpp>
#include <axl.math/transform/Transform4f.hpp>
#include <axl.glfl/GL.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {
	class AXLGLCXXAPI Layout;
	class AXLGLCXXAPI Group;
}
class AXLGLCXXAPI UIManager;

class AXLGLCXXAPI UIElement : public ContextObject
{
	public:
		class AXLGLCXXAPI Program : public axl::gl::gfx::Program
		{
			public:
				Program(axl::gl::Context* ptr_context = 0);
				~Program();
				bool icreate();
				bool idestroy();
		};
		class AXLGLCXXAPI ShadowProgram : public axl::gl::gfx::Program
		{
			public:
				ShadowProgram(axl::gl::Context* ptr_context = 0);
				~ShadowProgram();
				bool icreate();
				bool idestroy();
		};
	public:
		enum Quality {
			Q_LOW,
			Q_MEDIUM,
			Q_HIGH
		};
		enum Type {
			T_UIElement,
			T_Group,
			T_TextView,
		};
		enum Wrap {
			W_MATCH_PARENT,
			W_WRAP_CONTENT,
			W_FIXED_SIZE
		};
	public:
		UIElement(Type type = UIElement::T_UIElement, axl::gl::Context* ptr_context = 0);
		virtual ~UIElement();
		virtual bool isValid() const;
		virtual void setContext(axl::gl::Context* ptr_context);
		virtual axl::math::Vec2i getMinSize() const = 0;
		virtual axl::math::Vec2i getMaxSize() const = 0;
	protected:
		virtual bool icreate();
		virtual bool idestroy();
	protected:
		friend class AXLGLCXXAPI axl::gl::gfx::ui::Layout;
		friend class AXLGLCXXAPI axl::gl::gfx::ui::Group;
		friend class AXLGLCXXAPI axl::gl::gfx::UIManager;
		virtual bool setUIManager(axl::gl::gfx::UIManager* ui_manager);
		virtual bool setUIGroup(axl::gl::gfx::ui::Group* ui_group);
		virtual bool irender(const camera::Camera3Df* camera) = 0;
	public:
		bool render(const camera::Camera3Df* camera, const axl::gl::gfx::FrameBuffer* ptr_frame_buffer = 0);
		virtual bool setSize(const axl::math::Vec2i& size);
		virtual bool setQuality(Quality quality);
		virtual bool setBackgroundTexture(axl::gl::gfx::Texture2D* texture);
		void setHorizontalWrapMethod(Wrap horizontal_wrap);
		void setVerticalWrapMethod(Wrap vertical_wrap);
		void setWrapMethods(Wrap horizontal_wrap, Wrap vertical_wrap);
		Wrap getHorizontalWrapMethod() const;
		Wrap getVerticalWrapMethod() const;
		bool setBorderSize(const axl::math::Vec4f& border_size);
		bool setBorderColor(const axl::math::Vec4f& border_color);
		bool setShadowColor(const axl::math::Vec4f& shadow_color);
		bool setShadowOffset(const axl::math::Vec4f& shadow_offset);
		const axl::math::Vec2i& getSize() const;
		const axl::gl::gfx::Texture2D* getBackgroundTexture() const;
		const axl::math::Vec4f& getBorderSize() const;
		const axl::math::Vec4f& getBorderColor() const;
		const axl::math::Vec4f& getShadowColor() const;
		const axl::math::Vec4f& getShadowOffset() const;
	public:
		const Type type;
		bool enable_shadow;
		axl::math::Transform4f transform;
	protected:
		bool uielement_modified;
		axl::gl::gfx::UIManager* uielement_ui_manager;
		axl::gl::gfx::ui::Group* uielement_ui_group;
		Wrap uielement_horizontal_wrap, uielement_vertical_wrap;
		axl::math::Vec2i uielement_size;
		axl::math::Vec4f uielement_border_size;
		axl::math::Vec4f uielement_border_color;
		axl::math::Vec4f uielement_shadow_color;
		axl::math::Vec4f uielement_shadow_offset;
		axl::gl::gfx::UIFrameBuffer uielement_frame_buffer;
		gfx::Texture2D uielement_render_texture;
		gfx::Texture2D* uielement_bg_texture;
	private:
		axl::gl::gfx::UIElement::Program m_program;
		axl::gl::gfx::UIElement::ShadowProgram m_shadow_program;
		axl::glfl::GLuint m_vertex_array;
		axl::glfl::GLuint m_vertex_buffer;
		axl::glfl::GLint m_uloc_texture_bound;
		axl::glfl::GLint m_uloc_texture;
		axl::glfl::GLint m_uloc_texture1;
		axl::glfl::GLint m_uloc_projection;
		axl::glfl::GLint m_uloc_view;
		axl::glfl::GLint m_uloc_model;
		axl::glfl::GLint m_uloc_size;
		axl::glfl::GLint m_uloc_border;
		axl::glfl::GLint m_uloc_border_color;
		axl::glfl::GLint m_uloc_shadow_texture_bound;
		axl::glfl::GLint m_uloc_shadow_texture;
		axl::glfl::GLint m_uloc_shadow_projection;
		axl::glfl::GLint m_uloc_shadow_view;
		axl::glfl::GLint m_uloc_shadow_model;
		axl::glfl::GLint m_uloc_shadow_color;
		axl::glfl::GLint m_uloc_shadow_offset;
};

} // axl.gl.gfx
} // axl.gl
} // axl
