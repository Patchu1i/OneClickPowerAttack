#pragma once

#include "RE/B/BSPointerHandle.h"
#include "RE/I/IMenu.h"
#include "RE/N/NiSmartPointer.h"
#include <SKSE/Version.h>

namespace RE
{
	class TESObjectREFR;

	// menuDepth = 12
	// flags = kPausesGame | kAlwaysOpen | kUsesCursor | kAllowSaving
	// context = kConsole
	class Console : public IMenu
	{
	public:
		inline static constexpr auto      RTTI = RTTI_Console;
		constexpr static std::string_view MENU_NAME = "Console";

		struct RUNTIME_DATA
		{
#define RUNTIME_DATA_CONTENT       \
	void*         opcode; /* 00 */ \
	std::uint64_t unk38;  /* 08 */ \
	std::uint64_t unk40;  /* 10 */ \
	std::uint64_t unk48;  /* 18 */ \
	std::uint64_t unk50;  /* 20 */

			RUNTIME_DATA_CONTENT
		};
		static_assert(sizeof(RUNTIME_DATA) == 0x28);

		struct AE_RUNTIME_DATA
		{
#define AE_RUNTIME_DATA_CONTENT                                        \
	void*         opcode;                 /* 00 */                     \
	std::uint64_t unk38;                  /* 08 */                     \
	std::uint64_t unk40;                  /* 10 */                     \
	std::uint32_t unk48;                  /* 48 */                     \
	std::uint8_t  unk4c;                  /* 4c*/                      \
	bool          showAchievementWarning; /* 4d -- only used in ctor*/ \
	bool          ctrlKeyHeld;            /* 4e*/                      \
	std::uint8_t  pad4f;                  /* 4f*/

			AE_RUNTIME_DATA_CONTENT
		};
		static_assert(sizeof(AE_RUNTIME_DATA) == 0x20);

		~Console() override;  // 00

		// override (IMenu)
		void               Accept(CallbackProcessor* a_processor) override;  // 01
		UI_MESSAGE_RESULTS ProcessMessage(UIMessage& a_message) override;    // 04

		static NiPointer<TESObjectREFR> GetSelectedRef();
		static ObjectRefHandle          GetSelectedRefHandle();

		void SetSelectedRef(NiPointer<TESObjectREFR> a_refPtr);
		void SetSelectedRef(TESObjectREFR* a_ref);
		void SetSelectedRef(ObjectRefHandle a_handle);

		[[nodiscard]] inline RUNTIME_DATA& GetRuntimeData() noexcept
		{
			return REL::RelocateMember<RUNTIME_DATA>(this, 0x30, 0x40);
		}

		[[nodiscard]] inline const RUNTIME_DATA& GetRuntimeData() const noexcept
		{
			return REL::RelocateMember<RUNTIME_DATA>(this, 0x30, 0x40);
		}

		[[nodiscard]] inline AE_RUNTIME_DATA* GetAERuntimeData() noexcept
		{
			if SKYRIM_REL_CONSTEXPR (REL::Module::IsAE()) {
				if (REL::Module::get().version().compare(SKSE::RUNTIME_SSE_1_6_1130) != std::strong_ordering::less) {
					return REL::RelocateMember<AE_RUNTIME_DATA*>(this, 0x30);
				}
			}
			return nullptr;
		}

		[[nodiscard]] inline const AE_RUNTIME_DATA* GetAERuntimeData() const noexcept
		{
			if SKYRIM_REL_CONSTEXPR (REL::Module::IsAE()) {
				if (REL::Module::get().version().compare(SKSE::RUNTIME_SSE_1_6_1130) != std::strong_ordering::less) {
					return REL::RelocateMember<AE_RUNTIME_DATA*>(this, 0x30);
				}
			}
			return nullptr;
		}

		// members
#ifndef SKYRIM_CROSS_VR
		RUNTIME_DATA_CONTENT;  // 30, 40
#endif

	protected:
		void
			SetSelectedRef_Impl(ObjectRefHandle& a_handle);

	private:
		KEEP_FOR_RE()
	};
#if !defined(ENABLE_SKYRIM_VR)
#	if defined(ENABLE_SKYRIM_AE)
	static_assert(sizeof(Console) == 0x68);
#	else
	static_assert(sizeof(Console) == 0x58);
	char (*__kaboom)[sizeof(Console)] = 1;
#endif
#elif !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
	static_assert(sizeof(Console) == 0x68);
#endif
}
#undef RUNTIME_DATA_CONTENT
#undef AE_RUNTIME_DATA_CONTENT
