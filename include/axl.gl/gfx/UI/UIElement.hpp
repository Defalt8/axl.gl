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
			T_TextView,
		};
	public:
		UIElement(Type type = UIElement::T_UIElement, axl::gl::Context* ptr_context = 0);
		virtual ~UIElement();
		virtual bool isValid() const;
		virtual void setContext(axl::gl::Context* ptr_context);
	protected:
		virtual bool icreate();
		virtual bool idestroy();
	protected:
		friend class AXLGLCXXAPI UIManager;
		virtual bool setUIManager(UIManager* ui_manager);
		virtual bool irender(const camera::Camera3Df* camera) = 0;
	public:
		bool render(const camera::Camera3Df* camera);
		virtual bool setSize(const axl::math::Vec2i& size);
		virtual bool setQuality(Quality quality);
		bool setBorderSize(const axl::math::Vec4f& border_size);
		bool setBorderColor(const axl::math::Vec4f& border_color);
		bool setShadowColor(const axl::math::Vec4f& shadow_color);
		bool setShadowOffset(const axl::math::Vec4f& shadow_offset);
		const axl::math::Vec2i& getSize() const;
		const axl::math::Vec4f& getBorderSize() const;
		const axl::math::Vec4f& getBorderColor() const;
		const axl::math::Vec4f& getShadowColor() const;
		const axl::math::Vec4f& getShadowOffset() const;
	public:
		const Type type;
		bool enable_shadow;
		axl::math::Transform4f transform;
	protected:
		UIManager* uielement_ui_manager;
		axl::math::Vec2i uielement_size;
		axl::math::Vec4f uielement_border_size;
		axl::math::Vec4f uielement_border_color;
		axl::math::Vec4f uielement_shadow_color;
		axl::math::Vec4f uielement_shadow_offset;
		gfx::Texture2D uielement_texture;
		axl::gl::gfx::UIFrameBuffer uielement_frame_buffer;
	private:
		axl::gl::gfx::UIElement::Program m_program;
		axl::gl::gfx::UIElement::ShadowProgram m_shadow_program;
		axl::glfl::GLuint m_vertex_array;
		axl::glfl::GLuint m_vertex_buffer;
		axl::glfl::GLint m_uloc_projection;
		axl::glfl::GLint m_uloc_view;
		axl::glfl::GLint m_uloc_model;
		axl::glfl::GLint m_uloc_size;
		axl::glfl::GLint m_uloc_border;
		axl::glfl::GLint m_uloc_border_color;
		axl::glfl::GLint m_uloc_shadow_projection;
		axl::glfl::GLint m_uloc_shadow_view;
		axl::glfl::GLint m_uloc_shadow_model;
		axl::glfl::GLint m_uloc_shadow_color;
		axl::glfl::GLint m_uloc_shadow_offset;

};

} // axl.gl.gfx
} // axl.gl
} // axl
