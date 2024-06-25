#include "RE/B/BSMouseDevice.h"

namespace RE
{
	BSMouseDevice::BSMouseDevice() :
		BSInputDevice()
	{
		backgroundMouse = false;
		device = INPUT_DEVICE::kMouse;
		const char* mouseFile = *reinterpret_cast<const char**>(RELOCATION_ID(511918, 388490).address());
		LoadControlsDefinitionFile(mouseFile);
	}
}
