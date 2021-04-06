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
		virtual void irender(const camera::Camera3Df* camera) = 0;
		bool render(const camera::Camera3Df* camera);
	public:
		bool setSize(const axl::math::Vec2i& bounds);
		const axl::math::Vec2i& getSize();
	public:
		axl::math::Transform4f transform;
		axl::gl::gfx::UIFrameBuffer* frame_buffer;
	protected:
		axl::math::Vec2i uielement_size;
		gfx::Texture2D uielement_texture;
	private:
		axl::glfl::GLuint m_vertex_array;
		axl::glfl::GLuint m_vertex_buffer;
		Program m_program;
		axl::glfl::GLint uloc_projection;
		axl::glfl::GLint uloc_view;
		axl::glfl::GLint uloc_model;

};

} // axl.gl.gfx
} // axl.gl
} // axl
