#pragma once

#include "RE/H/HeldStateHandler.h"

namespace RE
{
	struct TogglePOVHandler : public HeldStateHandler
	{
	public:
		inline static constexpr auto RTTI = RTTI_TogglePOVHandler;

		~TogglePOVHandler() override;  // 00

		// override (PlayerInputHandler)
		bool CanProcess(InputEvent* a_event) override;                                  // 01
		void ProcessButton(ButtonEvent* a_event, PlayerControlsData* a_data) override;  // 04

		// members
		bool          pressRegistered;  // 18
		std::uint8_t  pad19;            // 19
		std::uint16_t pad1A;            // 1A
		std::uint32_t pad1C;            // 1C
	private:
		KEEP_FOR_RE()
	};
#if !defined(ENABLE_SKYRIM_VR)
	static_assert(sizeof(TogglePOVHandler) == 0x20);
#elif !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
	static_assert(sizeof(TogglePOVHandler) == 0x38);
#endif
}
