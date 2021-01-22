#pragma once
#include "../lib.hpp"
#include <axl.math/vec/Vec2i.hpp>

namespace axl {
namespace gl {
namespace input {

enum MouseButton {
	MBTN_LEFT,
	MBTN_RIGHT,
	MBTN_MIDDLE,
	MBTN_EXTRA
}; // enum MouseButton

class AXLGLCXXAPI Mouse 
{
	public:
		static bool isPresent();
		static bool isButtonDown(MouseButton button);
		static bool setCursorPosition(int x, int y);
		static bool setCursorPosition(const axl::math::Vec2i& cur_pos);
	public:
		class AXLGLCXXAPI Listener
		{
			public:
				static bool init();
				static void release();
				static bool isValid();
			public:
				static void (*onMouse)(MouseButton button, int x, int y, bool is_down);
				static void (*onMouseMove)(int x, int y);
				static void (*onMouseWheel)(bool is_vertical, int delta, int x, int y);
				static const void*const reserved;
			private:
				static void* m_reserved;
			private:
				Listener();
				~Listener();
		};
	private:
		Mouse();
		~Mouse();
};

} // namespace axl.gl.input
} // namespace axl.gl
} // namespace axl