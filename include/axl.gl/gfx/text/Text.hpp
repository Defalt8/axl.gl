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
		AXLGL_ENUM_CLASS Orientation { TOR_HORIZONTAL, TOR_VERTICAL };
		AXLGL_ENUM_CLASS Alignment { TAL_LEFT = 1, TAL_RIGHT = 2, TAL_HORIZONTAL_CENTER = 3, TAL_BOTTOM = 4, TAL_TOP = 8, TAL_VERTICAL_CENTER = 12 };
	public:
		Text(axl::gl::Context* ptr_context = (axl::gl::Context*)0);
		virtual ~Text();
		bool icreate();
		bool idestroy();
		bool isValid() const;
		bool render(const axl::gl::camera::Camera3Df* camera = 0) const;
		void updateTransform();
		// set methods
		void setPosition(const axl::math::Vec3f& position, bool update = true);
		void setScale(const axl::math::Vec3f& scale, bool update = true);
		void setRotation(const axl::math::Vec3f& rotation, bool update = true);
		void setColor(const axl::math::Vec4f& color);
		bool setStorageSize(axl::util::size_t size);
		bool setText(const axl::util::WString& wstring);
		void setFont(const Font* font);
		void setProgram(const axl::gl::gfx::Program* text_shader_program);
		void setOrientation(Orientation orientation);
		void setAlignment(AlignmentFlag alignment_flags);
		// get methods
		const axl::math::Vec3f& getPosition() const;
		const axl::math::Vec3f& getScale() const;
		const axl::math::Vec3f& getRotation() const;
		const axl::math::Mat4f& getTransform() const;
		const axl::math::Vec4f& getColor() const;
		const axl::util::WString& getText() const;
		const axl::gl::gfx::Font* getFont() const;
		const axl::gl::gfx::Program* getProgram() const;
		Orientation getOrientation() const;
		AlignmentFlag getAlignment() const;
	protected:
		bool updateBuffers(const axl::util::WString& p_wstring, bool text_size_changed = false);
	protected:
		axl::math::Vec3f text_position;
		axl::math::Vec3f text_scale;
		axl::math::Vec3f text_rotation;
		axl::math::Mat4f text_transform;
		axl::math::Vec4f text_color;
		axl::util::WString text_wstring;
		const axl::gl::gfx::Font* text_font;
		const axl::gl::gfx::Program* text_program;
		Orientation text_orientation;
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
		axl::glfl::GLint uniform_location_text_color;
};

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
