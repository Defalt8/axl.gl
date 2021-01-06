#pragma once
#include "common.hpp"

struct DisplayData {
	DisplayData() : set(false), hmonitor(NULL), hdc(NULL), rect({0,0,0,0}), mil_size_x(0), mil_size_y(0), color_bits(0), refresh_rate(0) {}
	bool set;
	HMONITOR hmonitor;
	HDC hdc;
	RECT rect;
	int mil_size_x;
	int mil_size_y;
	int color_bits;
	int refresh_rate;
};