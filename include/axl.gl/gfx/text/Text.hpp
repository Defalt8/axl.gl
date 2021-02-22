#pragma once
#include "../../lib.hpp"
#include "../../ContextObject.hpp"
#include "../../camera/Camera3D.hpp"
#include "../shader/Program.hpp"
#include <axl.glfl/gl.hpp>
#include <axl.util/String.hpp>
#include <axl.util/WString.hpp>
#include <axl.math/vec/Vec3f.hpp>
#include <axl.math/vec/Vec4f.hpp>
#include <axl.math/mat/Mat4f.hpp>

namespace axl {
namespace gl {

class AXLGLCXXAPI Context;

namespace gfx {

class AXLGLCXXAPI Font;

class AXLGLCXXAPI Text : public ContextObject
{
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
		Text(axl::gl::Context* ptr_context = (axl::gl::Context*)0);
		virtual ~Text();
		bool icreate();
		bool idestroy();
		bool isValid() const;
		bool render(const axl::gl::camera::Camera3Df* camera = 0) const;
		void updateTransform();
		void updateBuffers();
		void updateAlignment();
		// set methods
		void setPosition(const axl::math::Vec3f& position, bool update = true);
		void setScale(const axl::math::Vec3f& scale, bool update = true);
		void setRotation(const axl::math::Vec3f& rotation, bool update = true);
		void setColor(const axl::math::Vec4f& color);
		bool setSpacing(const axl::math::Vec2f& spacing);
		bool setStorageSize(axl::util::size_t size);
		bool setText(const axl::util::WString& wstring);
		void setFont(const Font* font);
		void setProgram(const axl::gl::gfx::Program* text_shader_program);
		void setAlignment(AlignmentFlag alignment_flags);
		void setHorizontalAlignment(HorizontalAlignment horizontal_alignment);
		void setVerticalAlignment(VerticalAlignment vertical_alignment);
		// get methods
		const axl::math::Vec3f& getPosition() const;
		const axl::math::Vec3f& getScale() const;
		const axl::math::Vec3f& getRotation() const;
		const axl::math::Mat4f& getTransform() const;
		const axl::math::Vec4f& getColor() const;
		const axl::math::Vec2f& getOffset() const;
		const axl::math::Vec2f& getBox() const;
		const axl::math::Vec2f& getSpacing() const;
		const axl::util::WString& getText() const;
		const axl::gl::gfx::Font* getFont() const;
		const axl::gl::gfx::Program* getProgram() const;
		AlignmentFlag getAlignment() const;
		HorizontalAlignment getHorizontalAlignment() const;
		VerticalAlignment getVerticalAlignment() const;
	protected:
		bool updateBuffers(const axl::util::WString& p_wstring, bool text_size_changed = false, bool font_attribute_altered = false);
	protected:
		axl::math::Vec3f text_position;
		axl::math::Vec3f text_scale;
		axl::math::Vec3f text_rotation;
		axl::math::Mat4f text_transform;
		axl::math::Vec4f text_color;
		axl::math::Vec2f text_offset;
		axl::math::Vec2f text_box;
		axl::math::Vec2f text_spacing;
		axl::util::WString text_wstring;
		const axl::gl::gfx::Font* text_font;
		const axl::gl::gfx::Program* text_program;
		AlignmentFlag text_alignment;
	private:
		unsigned int actual_text_length;
		axl::glfl::GLuint vertex_array_id;
		axl::glfl::GLuint vertex_buffer_id;
		axl::glfl::GLuint element_array_id;
		axl::glfl::GLint attribute_location_position;
		axl::glfl::GLint attribute_location_UV;
		axl::glfl::GLint uniform_location_projection_matrix;
		axl::glfl::GLint uniform_location_view_matrix;
		axl::glfl::GLint uniform_location_model_matrix;
		axl::glfl::GLint uniform_location_text_offset;
		axl::glfl::GLint uniform_location_text_color;
		axl::math::Vec2i rec_font_size;
};

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
