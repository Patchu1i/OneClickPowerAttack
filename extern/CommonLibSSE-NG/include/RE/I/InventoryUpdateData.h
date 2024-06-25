#pragma once

#include "RE/I/IUIMessageData.h"

namespace RE
{
	class TESBoundObject;

	class InventoryUpdateData : public IUIMessageData
	{
	public:
		inline static constexpr auto RTTI = RTTI_InventoryUpdateData;

		~InventoryUpdateData() override = default;  // 00

		// members
		RefHandle     inventoryRef;  // 10
		std::uint32_t pad14;         // 14
		TESForm*      unk18;         // 18
	private:
		KEEP_FOR_RE()
	};
	static_assert(sizeof(InventoryUpdateData) == 0x20);
}
