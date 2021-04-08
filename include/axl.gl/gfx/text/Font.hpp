#pragma once
#include "../../lib.hpp"
#include "../../ContextObject.hpp"
#include "../texture/Texture2D.hpp"
#include <axl.glfl/gl.hpp>
#include <axl.util/ds/Array.hpp>
#include <axl.util/String.hpp>
#include <axl.util/WString.hpp>
#include <axl.math/vec/Vec2i.hpp>
#include <axl.math/vec/Vec4f.hpp>

namespace axl {
namespace gl {

class AXLGLCXXAPI Context;

namespace gfx {

class AXLGLCXXAPI Text;

struct AXLGLCXXAPI GlyphData {
	short width, height;
	short horiBearingX, horiBearingY;
	short horiAdvance;
	axl::math::Vec4f UV;
};

template class AXLGLCXXAPI axl::util::ds::Array<axl::gl::gfx::GlyphData>;

class AXLGLCXXAPI Font : public ContextObject
{
	public:
		enum Quality {
			Q_LOW,
			Q_MEDIUM,
			Q_HIGH
		};
	public:
		Font(axl::gl::Context* ptr_context = (axl::gl::Context*)0);
		virtual ~Font();
		bool icreate();
		bool idestroy();
		bool isValid() const;
		bool loadFromFile(const axl::util::String& filepath, const axl::math::Vec2i& font_size);
		bool setSize(const axl::math::Vec2i& font_size);
		bool setQuality(Quality quality);
		unsigned int getCharIndex(axl::util::WString::char_t unicode_char) const;
	private:
		bool loadGlyphs(const axl::math::Vec2i& font_size, int level = 0, unsigned long count = -1);
	public:
		static bool init();
		static bool cleanup();
	public:
		const axl::gl::gfx::Texture2D& texture;
		const axl::util::ds::Array<GlyphData>& glyphs;
		const axl::math::Vec2i& size;
	protected:
		axl::gl::gfx::Texture2D font_texture;
		axl::util::ds::Array<GlyphData> font_glyphs;
		axl::math::Vec2i font_size;
		void* font_reserved;
};


} // namespace axl.gl.gfx
} // namespace axl.gl
} // namespace axl
