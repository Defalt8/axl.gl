#include "common.hpp"
#include <axl.gl/lib.hpp>
#include <axl.gl/input/Touch.hpp>

namespace axl {
namespace gl {
namespace input {

bool Touch::isPresent()
{
	UINT num_devices = 0;
	RAWINPUTDEVICELIST *input_devices;
	if (GetRawInputDeviceList(nullptr, &num_devices, sizeof(RAWINPUTDEVICELIST)) != 0 || num_devices == 0)
		return false;
	input_devices = new  RAWINPUTDEVICELIST[num_devices];
	if (GetRawInputDeviceList(input_devices, &num_devices, sizeof(RAWINPUTDEVICELIST)) == (UINT)-1)
	{
		delete[] input_devices;
		return false;
	}
	for(UINT i = 0; i < num_devices; ++i)
	{
		if(input_devices[i].dwType == RIM_TYPEHID)
			return true;
	}
	return false;
}

} // namespace axl.gl.input
} // namespace axl.gl
} // namespace axl