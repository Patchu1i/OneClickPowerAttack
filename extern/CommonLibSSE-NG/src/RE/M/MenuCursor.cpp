#include "RE/M/MenuCursor.h"

namespace RE
{
	MenuCursor* MenuCursor::GetSingleton()
	{
		REL::Relocation<MenuCursor**> singleton{ RELOCATION_ID(517043, 403551) };
		return *singleton;
	}

	void MenuCursor::SetCursorVisibility(bool a_visible)
	{
		if (a_visible && cursorDisplayCount < 0) {
			do {
				cursorDisplayCount = WinAPI::ShowCursor(true);
			} while (cursorDisplayCount < 0);
		} else if (!a_visible && cursorDisplayCount >= 0) {
			do {
				cursorDisplayCount = WinAPI::ShowCursor(false);
			} while (cursorDisplayCount >= 0);
		}
	}
}
