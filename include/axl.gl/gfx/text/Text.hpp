#pragma once
#include "../../lib.hpp"
#include "../../ContextObject.hpp"
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
		bool render() const;
		// set methods
		void setPosition(const axl::math::Vec3f& position);
		void setScale(const axl::math::Vec3f& scale);
		void setRotation(const axl::math::Vec3f& rotation);
		void setTransform(const axl::math::Mat4f& transform);
		void setBoxSize(const axl::math::Vec2f& box_size);
		void setText(const axl::util::WString& wstring);
		void setFont(const Font* font);
		void setProgram(const axl::gl::gfx::Program* text_shader_program);
		void setOrientation(Orientation orientation);
		void setAlignment(AlignmentFlag alignment_flags);
		// get methods
		const axl::math::Vec3f& getPosition() const;
		const axl::math::Vec3f& getScale() const;
		const axl::math::Vec3f& getRotation() const;
		const axl::math::Mat4f& getTransform() const;
		const axl::math::Vec2f& getBoxSize() const;
		const axl::util::WString& getText() const;
		const axl::gl::gfx::Font* getFont() const;
		const axl::gl::gfx::Program* getProgram() const;
		Orientation getOrientation() const;
		AlignmentFlag getAlignment() const;
	protected:
		axl::math::Vec3f text_position;
		axl::math::Vec3f text_scale;
		axl::math::Vec3f text_rotation;
		axl::math::Mat4f text_transform;
		axl::math::Vec2f text_box_size;
		axl::util::WString text_wstring;
		const axl::gl::gfx::Font* text_font;
		const axl::gl::gfx::Program* text_program;
		Orientation text_orientation;
		AlignmentFlag text_alignment;
	private:
		axl::glfl::GLuint vertex_array_id;
		axl::glfl::GLuint vertex_buffer_id;
};

} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
