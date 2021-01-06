#pragma once
#include "lib.hpp"
#include <axl.math/vec/Vec2i.hpp>
#include <axl.math/vec/Vec2f.hpp>

namespace axl {
namespace gl {

class Display {
public:
	Display(int index = 0);
	~Display();
	bool isOpen() const;
	bool reopen(int index);
	bool close();
	bool isConfigurable() const;
public:
	static int count();
public:
	const int& index;
	const axl::math::Vec2i& position; // position in pixels
	const axl::math::Vec2i& size; // size in pixels
	const axl::math::Vec2f& physical_size; // size in millimeters
	const axl::math::Vec2f& ppmm; // pixels per millimeter
	const axl::math::Vec2f& ppi; // pixels per inch
private:
	void* m_reserved;
	int m_index;
	axl::math::Vec2i m_position;
	axl::math::Vec2i m_size;
	axl::math::Vec2f m_physical_size;
	axl::math::Vec2f m_ppmm;
	axl::math::Vec2f m_ppi;
};

} // axl::gl
} // axl
