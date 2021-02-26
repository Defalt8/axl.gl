#pragma once
#include "lib.hpp"
#include <axl.math/vec/Vec2i.hpp>
#include <axl.math/vec/Vec2f.hpp>
#include <axl.util/ds/List.hpp>

namespace axl {
namespace gl {

class AXLGLCXXAPI View;

template class AXLGLCXXAPI axl::util::ds::UniList<axl::gl::View*>;
template class AXLGLCXXAPI axl::util::ds::UniList<axl::gl::View*>::Iterator;

class AXLGLCXXAPI Display {
	public:
		struct AXLGLCXXAPI Settings
		{
			enum Orientation { OR_PORTRAIT, OR_LANDSCAPE };
			enum Index { DI_CURRENT = -2, DI_DEFAULT = -1 };
			enum Rotation { ROT_DEFAULT = 0, ROT_CCW_0 = 0, ROT_CCW_90 = 1, ROT_CCW_180 = 2, ROT_CCW_270 = 3 };
			Settings(int bits_per_pixel = -1, int width = -1, int height = -1, int frequency = -1, Rotation rotation = ROT_DEFAULT);
			Settings& operator=(const Display::Settings& settings);
			int bits_per_pixel;
			int width;
			int height;
			int frequency;
			Rotation rotation;
			Orientation orientation;
		};
	public:
		Display(int index = 0);
		~Display();
		bool isOpen() const;
		bool reopen(int index);
		bool close();
		bool isConfigurable() const;
		const axl::util::ds::UniList<axl::gl::View*>& getViews() const;
	protected:
		bool addView(View* view);
		bool removeView(View* view);
		friend class AXLGLCXXAPI axl::gl::View;
	public:
		static axl::math::Vec2i getDefaultSize();
		// Enumerate and return the <i>th supported display setting.
		// Pass i=DI_DEFAULT for the default settings and i=DI_CURRENT for the current settings.
		static bool enumSettings(Settings* display_setting, int i = 0);
		// Sets the display settings.
		// Set Display::Setting member values to -1 to ignore them.
		static bool setSettings(const Settings& display_setting, bool test = false);
		// Restores the default display settings
		static bool restoreSettings();
		static int count();
	public:
		const int& index;
		const axl::math::Vec2i& position; // position in pixels
		const axl::math::Vec2i& size; // size in pixels
		const axl::math::Vec2f& physical_size; // size in millimeters
		const axl::math::Vec2f& ppmm; // pixels per millimeter
		const axl::math::Vec2f& ppi; // pixels per inch
		const char*const name;
		const Settings& settings;
	private:
		void* m_reserved;
		int m_index;
		char m_name[128];
		Settings m_settings;
		axl::math::Vec2i m_position;
		axl::math::Vec2i m_size;
		axl::math::Vec2f m_physical_size;
		axl::math::Vec2f m_ppmm;
		axl::math::Vec2f m_ppi;
		axl::util::ds::UniList<axl::gl::View*> m_views;
};

} // axl::gl
} // axl
