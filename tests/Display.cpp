
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <axl.gl/lib.hpp>
#include <axl.gl/Display.hpp>
#include <axl.math/float.hpp>
#include "Assert.hpp"

int main(int argc, char* argv[])
{
	bool verbose = argc > 1 && (0 == strcmp(argv[1], "-v") || 0 == strcmp(argv[1], "--verbose"));
	using namespace axl::gl;
	using namespace axl::gl::lib;
	printf(">> axl.gl Display test -- axl.gl %s library %u.%u.%u\n", (BUILD == Build::SHARED ? "SHARED" : "STATIC"), VERSION.major, VERSION.minor, VERSION.patch);
	puts("----------------------------------------");
	{
		int display_count = Display::count();
		Assertve(display_count > 0, verbose);
		if(verbose)
		{
			printf("Display_count: %d\n", display_count);
		}
		for(int i = 0; i < display_count; ++i)
		{
			Display display(i);
			Assertv(display.isOpen(), verbose);
			if(!display.isOpen()) continue;
			printf("Display: %s\n", display.name);
			Assertv(display.size.x > 0 && display.size.y > 0, verbose);
			Assertv((display.physical_size * display.ppmm).equals((float)display.size.x, (float)display.size.y, 0.0001f), verbose);
			if(verbose)
			{
				printf(".. Display_index: %d\n", display.index);
				printf(".. Display_position: %dx%d\n", display.position.x, display.position.y);
				printf(".. Display_size: %dx%d\n", display.size.x, display.size.y);
				printf(".. Display_physical_size: %.3fx%.3f\n", display.physical_size.x, display.physical_size.y);
				printf(".. Display_ppmm: %.3f,%.3f\n", display.ppmm.x, display.ppmm.y);
				printf(".. Display_ppi: %.3f,%.3f\n", display.ppi.x, display.ppi.y);
				printf(".. recalulated size in pixles: %.3f,%.3f\n", display.physical_size.x * display.ppmm.x, display.physical_size.y * display.ppmm.y);
				printf(".. recalulated size in inches: %.3f,%.3f\n", display.size.x / display.ppi.x, display.size.y / display.ppi.y);
				printf("..\n");
				Display::Settings settings;
				int index = 0;
				puts("Available Resoulutions: ");
				while(display.enumSettings(&settings, index++))
				{
					printf("%d-{ (%d,%d) %dbits %dHz\n", index, settings.width, settings.height, settings.bits_per_pixel, settings.frequency);
				}
			}
		}
	}
	if (axl::Assert::NUM_FAILED_ASSERTIONS) puts("----------------------------------------");
	printf("%c> %d/%d Passed!\n", (axl::Assert::NUM_FAILED_ASSERTIONS ? '*' : '\r'), (axl::Assert::NUM_TOTAL_ASSERTIONS - axl::Assert::NUM_FAILED_ASSERTIONS), axl::Assert::NUM_TOTAL_ASSERTIONS);
	return axl::Assert::NUM_FAILED_ASSERTIONS;
}