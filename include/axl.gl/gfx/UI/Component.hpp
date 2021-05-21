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
			public:
				virtual bool isValid() const;
				void setContext(axl::gl::Context* ptr_context);
			protected:
				bool iCreate();
				bool iDestroy();
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
				bool iCreate();
				bool iDestroy();
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
			const axl::math::Vec4f& padding);
		virtual ~Component();
	protected:
		virtual bool iCreate();
		virtual bool iDestroy();
	public:
		virtual bool isValid() const;
		virtual void setContext(axl::gl::Context* ptr_context);
		bool setComponentProgram(const axl::gl::gfx::ui::Component::Program* ptr_program);
		virtual bool setContainer(axl::gl::gfx::ui::Container* container);
		virtual void setVisiblity(bool is_visible);
		virtual bool setSize(const axl::math::Vec2i& size);
		virtual bool setPadding(const axl::math::Vec4f& padding);
		virtual void setBackgroundColor(const axl::math::Vec4f& background_color);
		virtual void setForegroundColor(const axl::math::Vec4f& foreground_color);
		const axl::gl::gfx::ui::Component::Program* getComponentProgram() const;
		axl::gl::gfx::ui::Container* getContainer() const;
		bool isVisible() const;
		bool hasInputFocus() const;
		bool requestInputFocus() const;
		bool loseInputFocus() const;
		const axl::math::Vec2i& getSize() const;
		const axl::math::Vec4f& getPadding() const;
		const axl::math::Vec4f& getBackgroundColor() const;
		const axl::math::Vec4f& getForegroundColor() const;
		const axl::gl::gfx::Texture2D& getTexture() const;
		axl::math::Vec2i getClientSize() const;
		bool render(axl::gl::camera::Camera3Df* camera = 0, const axl::gl::gfx::FrameBuffer* ptr_frame_buffer = 0);
		void update();
	protected:
		// Sub-component's render implementation method. Implement in child class.
		virtual bool iRender(axl::gl::camera::Camera3Df* camera) = 0;
		// Called when component has undergone change
		virtual void onModify();
		// Called when component requests input focus.
		// Implement and return true if child accepts inputs that require focus, false otherwise.
		// By default it returns false.
		virtual bool onInputFocusRequest() const;
		// Called when input focus has changed to another component.
		virtual void onInputFocusLost();
		// Called when input focus has changed to this component.
		virtual void onInputFocusGain();
	public:
		const Type component_type;
		axl::math::Transform4f transform;
	private:
		axl::glfl::GLuint m_vertex_array;
		axl::glfl::GLuint m_vertex_buffer;
	protected:
		Component::FrameBuffer component_framebuffer;
		const Component::Program* component_program_ptr;
		axl::gl::gfx::ui::Container* component_container;
		bool component_is_visible;
		bool component_is_modified;
		axl::math::Vec2i component_size;
		axl::math::Vec4f component_padding;
		axl::math::Vec4f component_background_color;
		axl::math::Vec4f component_foreground_color;
		friend class axl::gl::gfx::ui::Container;
};

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl

