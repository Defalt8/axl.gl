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
	public:
		UIElement(axl::gl::Context* ptr_context = 0);
		virtual ~UIElement();
		virtual bool isValid() const;
		virtual void setContext(axl::gl::Context* ptr_context);
		virtual bool icreate();
		virtual bool idestroy();
	protected:
		virtual bool irender(const camera::Camera3Df* camera) = 0;
	public:
		bool render(const camera::Camera3Df* camera);
		virtual bool setSize(const axl::math::Vec2i& size);
		bool setBorderSize(const axl::math::Vec4f& border_size);
		bool setBorderColor(const axl::math::Vec4f& border_color);
		const axl::math::Vec2i& getSize() const;
		const axl::math::Vec4f& getBorderSize() const;
		const axl::math::Vec4f& getBorderColor() const;
	public:
		axl::math::Transform4f transform;
	protected:
		axl::math::Vec2i uielement_size;
		axl::math::Vec4f uielement_border_size;
		axl::math::Vec4f uielement_border_color;
		gfx::Texture2D uielement_texture;
		axl::gl::gfx::UIFrameBuffer uielement_frame_buffer;
	private:
		axl::gl::gfx::UIElement::Program m_program;
		axl::glfl::GLuint m_vertex_array;
		axl::glfl::GLuint m_vertex_buffer;
		axl::glfl::GLint m_uloc_projection;
		axl::glfl::GLint m_uloc_view;
		axl::glfl::GLint m_uloc_model;
		axl::glfl::GLint m_uloc_size;
		axl::glfl::GLint m_uloc_border;
		axl::glfl::GLint m_uloc_border_color;

};

} // axl.gl.gfx
} // axl.gl
} // axl
