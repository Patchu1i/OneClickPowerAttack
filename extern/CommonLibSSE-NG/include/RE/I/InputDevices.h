#pragma once

namespace RE
{
	struct INPUT_DEVICES
	{
		enum INPUT_DEVICE
		{
			kNone = static_cast<std::underlying_type_t<INPUT_DEVICE>>(-1),
			kKeyboard = 0,
			kMouse,
			kGamepad,
#ifdef ENABLE_SKYRIM_VR
			kVivePrimary,
			kViveSecondary,
			kOculusPrimary,
			kOculusSecondary,
			kWMRPrimary,
			kWMRSecondary,
#endif
			kVirtualKeyboard,
			kTotal
		};
		static_assert(sizeof(INPUT_DEVICE) == 0x4);
	};
	using INPUT_DEVICE = INPUT_DEVICES::INPUT_DEVICE;
}
