#pragma once
#include "../../lib.hpp"
#include "../../Context.hpp"
#include "../shader/Program.hpp"
#include "../text/Font.hpp"
#include "UIFrameBuffer.hpp"
#include "UIElement.hpp"
#include <axl.math/vec/Vec4f.hpp>

namespace axl {
namespace gl {
namespace gfx {
namespace ui {

class AXLGLCXXAPI TextView : public axl::gl::gfx::UIElement
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
		typedef unsigned short AlignmentFlag;
		enum HorizontalAlignment {
			TAL_LEFT = 1,
			TAL_RIGHT = 2,
			TAL_HORIZONTAL_CENTER = 3
			};
		enum VerticalAlignment {
			TAL_BOTTOM = 4,
			TAL_TOP = 8,
			TAL_VERTICAL_CENTER = 12
			};
		enum Alignment {
			TAL_TOP_LEFT = 9,
			TAL_TOP_RIGHT = 10,
			TAL_TOP_CENTER = 11,
			TAL_BOTTOM_LEFT = 5,
			TAL_BOTTOM_RIGHT = 6,
			TAL_BOTTOM_CENTER = 7,
			TAL_LEFT_CENTER = 13,
			TAL_RIGHT_CENTER = 14,
			TAL_CENTER = 15
			};
	public:
		TextView(axl::gl::Context* ptr_context = 0,
			axl::gl::gfx::Font* ptr_font = 0,
			axl::gl::gfx::Program* ptr_program = 0,
			axl::math::Vec2i size = axl::math::Vec2i(100,100)
			);
		~TextView();
		bool isValid() const;
		void setContext(axl::gl::Context* ptr_context);
	protected:
		bool icreate();
		bool idestroy();
	public:
		// set methods
		bool setSize(const axl::math::Vec2i& size);
		void setBackgroundColor(const axl::math::Vec4f& bg_color);
		void setTextColor(const axl::math::Vec4f& text_color);
		void setTextShadowColor(const axl::math::Vec4f& shadow_color);
		void setTextShadowOffset(const axl::math::Vec2f& shadow_offset);
		bool setSpacing(const axl::math::Vec2f& spacing);
		bool setOrigin(const axl::math::Vec2f& origin);
		bool setPadding(const axl::math::Vec4f& padding);
		bool setStorageSize(axl::util::size_t size);
		bool setText(const axl::util::WString& wstring);
		void setFont(const Font* font);
		bool setProgram(const axl::gl::gfx::Program* text_shader_program);
		void setAlignment(AlignmentFlag alignment_flags);
		void setHorizontalAlignment(HorizontalAlignment horizontal_alignment);
		void setVerticalAlignment(VerticalAlignment vertical_alignment);
		// get methods
		const axl::math::Vec4f& getBackgroundColor() const;
		const axl::math::Vec4f& getTextColor() const;
		const axl::math::Vec4f& getTextShadowColor() const;
		const axl::math::Vec2f& getTextShadowOffset() const;
		const axl::math::Vec2f& getSpacing() const;
		const axl::math::Vec2f& getOrigin() const;
		const axl::math::Vec4f& getPadding() const;
		axl::math::Vec2i getMinSize() const;
		axl::math::Vec2i getMaxSize() const;
		const axl::util::WString& getText() const;
		const axl::gl::gfx::Font* getFont() const;
		const axl::gl::gfx::Program* getProgram() const;
		AlignmentFlag getAlignment() const;
		HorizontalAlignment getHorizontalAlignment() const;
		VerticalAlignment getVerticalAlignment() const;
	public:
		bool enable_text_shadow;
	protected:
		void updateOffset();
		void updateAlignment();
		bool updateBuffers(const axl::util::WString& p_wstring, bool text_size_changed = false, bool font_attribute_altered = false);
		bool irender(const axl::gl::camera::Camera3Df* camera);
	protected:
		axl::util::WString txtv_wstring;
		axl::math::Vec4f txtv_background_color;
		axl::math::Vec4f txtv_text_color;
		axl::math::Vec2f txtv_shadow_offset;
		axl::math::Vec4f txtv_shadow_color;
		axl::math::Vec2f txtv_spacing;
		axl::math::Vec2f txtv_origin;
		axl::math::Vec4f txtv_padding;
		axl::math::Vec2f txtv_offset;
		axl::math::Vec2i txtv_box_size;
		const axl::gl::gfx::Font* txtv_font;
		const axl::gl::gfx::Program* txtv_program;
		AlignmentFlag txtv_alignment;
		axl::math::Transform4f txtv_transform;
	private:
		unsigned int m_actual_text_length;
		axl::glfl::GLuint m_vertex_array_id;
		axl::glfl::GLuint m_vertex_buffer_id;
		axl::glfl::GLuint m_element_array_id;
		axl::glfl::GLint m_attribute_location_position;
		axl::glfl::GLint m_attribute_location_UV;
		axl::glfl::GLint m_uloc_projection_matrix;
		axl::glfl::GLint m_uloc_view_matrix;
		axl::glfl::GLint m_uloc_model_matrix;
		axl::glfl::GLint m_uloc_text_offset;
		axl::glfl::GLint m_uloc_text_color;
		axl::glfl::GLint m_uloc_depth;
};

} // axl.gl.gfx.ui
} // axl.gl.gfx
} // axl.gl
} // axl
