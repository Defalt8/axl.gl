#pragma once
#include "../../lib.hpp"
#include "../../Context.hpp"
#include "../../ContextObject.hpp"
#include "../../camera/Camera3D.hpp"
#include "../../gfx/buffer/FrameBuffer.hpp"
#include "../../gfx/buffer/RenderBuffer.hpp"
#include "../../gfx/texture/Texture2D.hpp"
#include "../../gfx/shader/Program.hpp"
#include <axl.util/ds/List.hpp>
#include <axl.math/vec/Vec2i.hpp>
#include <axl.math/vec/Vec3f.hpp>
#include <axl.math/vec/Vec4f.hpp>
#include <axl.math/transform/Transform4f.hpp>

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
		class AXLGLCXXAPI FrameBuffer : public axl::gl::gfx::FrameBuffer
		{
			public:
				FrameBuffer(axl::gl::Context* ptr_context = 0);
				~FrameBuffer();
				bool isValid() const;
				void setContext(axl::gl::Context* ptr_context);
			protected:
				bool icreate();
				bool idestroy();
			public:
				bool setSize(const axl::math::Vec2i& size);
				const axl::math::Vec2i& getSize() const;
			protected:
				axl::gl::gfx::Texture2D fb_render_texture;
				axl::gl::gfx::RenderBuffer fb_render_buffer_depth;
				axl::math::Vec2i fb_size;
				friend class AXLGLCXXAPI axl::gl::gfx::ui::Component;
		};
		class AXLGLCXXAPI Program : public axl::gl::gfx::Program
		{
			public:
				Program(axl::gl::Context* ptr_context = 0);
				~Program();
				bool icreate();
				bool idestroy();
			protected:
				axl::glfl::GLint uloc_texture0;
				axl::glfl::GLint uloc_projection;
				axl::glfl::GLint uloc_view;
				axl::glfl::GLint uloc_model;
				friend class AXLGLCXXAPI axl::gl::gfx::ui::Component;
		};
	public:
		Component(Type type, axl::gl::Context* ptr_context,
			axl::gl::gfx::ui::Container* container,
			const axl::math::Vec3f& position,
			const axl::math::Vec2i& size,
			const axl::math::Vec4f& margin,
			const axl::math::Vec4f& padding);
		virtual ~Component();
	protected:
		virtual bool icreate();
		virtual bool idestroy();
	public:
		virtual bool isValid() const;
		virtual void setContext(axl::gl::Context* ptr_context);
		virtual bool setContainer(axl::gl::gfx::ui::Container* container);
		void setVisiblity(bool is_visible);
		bool setSize(const axl::math::Vec2i& size);
		bool setMargin(const axl::math::Vec4f& margin);
		void setPadding(const axl::math::Vec4f& padding);
		void setBackgroundColor(const axl::math::Vec4f& background_color);
		void setForegroundColor(const axl::math::Vec4f& foreground_color);
		axl::gl::gfx::ui::Container* getContainer() const;
		bool isVisible() const;
		const axl::math::Vec2i& getSize() const;
		const axl::math::Vec4f& getMargin() const;
		const axl::math::Vec4f& getPadding() const;
		const axl::math::Vec4f& getBackgroundColor() const;
		const axl::math::Vec4f& getForegroundColor() const;
		const axl::gl::gfx::Texture2D& getTexture() const;
		axl::math::Vec2i getClientSize() const;
		bool render(axl::gl::camera::Camera3Df* camera = 0, const axl::gl::gfx::FrameBuffer* ptr_frame_buffer = 0);
	protected:
		virtual bool iRender(axl::gl::camera::Camera3Df* camera) = 0;
	public:
		const Type component_type;
		axl::math::Transform4f transform;
	private:
		Component::FrameBuffer m_framebuffer;
		Component::Program m_program;
		axl::glfl::GLuint m_vertex_array;
		axl::glfl::GLuint m_vertex_buffer;
	protected:
		axl::gl::gfx::ui::Container* component_container;
		bool component_is_visible;
		bool component_is_modified;
		axl::math::Vec2i component_size;
		axl::math::Vec4f component_margin;
		axl::math::Vec4f component_padding;
		axl::math::Vec4f component_background_color;
		axl::math::Vec4f component_foreground_color;
		friend class axl::gl::gfx::ui::Container;
};

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
