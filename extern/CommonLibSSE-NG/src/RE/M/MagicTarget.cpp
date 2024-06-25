#include "RE/M/MagicTarget.h"

#include "RE/A/ActiveEffect.h"
#include "RE/A/Actor.h"
#include "RE/B/BSTList.h"
#include "RE/E/EffectSetting.h"

namespace RE
{
	bool MagicTarget::DispelEffect(MagicItem* a_spell, BSPointerHandle<Actor>& a_caster, ActiveEffect* a_effect)
	{
		using func_t = decltype(&MagicTarget::DispelEffect);
		REL::Relocation<func_t> func{ RELOCATION_ID(33721, 34505) };
		return func(this, a_spell, a_caster, a_effect);
	}

#if defined(ENABLE_SKYRIM_VR)
	void MagicTarget::DispelEffectsWithArchetype(Archetype a_type, bool a_force)
	{
		std::vector<RE::ActiveEffect*> queued;
		VisitActiveEffects([&](ActiveEffect* effect) {
			const auto setting = effect ? effect->GetBaseObject() : nullptr;
			if (setting && setting->HasArchetype(a_type)) {
				queued.push_back(effect);
			}
			return BSContainer::ForEachResult::kContinue;
		});

		for (const auto& effect : queued) {
			effect->Dispel(a_force);
		}
	}
#endif

	Actor* MagicTarget::GetTargetAsActor()
	{
		if (MagicTargetIsActor()) {
			return static_cast<Actor*>(static_cast<void*>(this));
		}

		return nullptr;
	}

	bool MagicTarget::HasEffectWithArchetype(Archetype a_type)
	{
		auto effects = GetActiveEffectList();
		if (!effects) {
			return false;
		}

		EffectSetting* setting = nullptr;
		for (auto& effect : *effects) {
			setting = effect ? effect->GetBaseObject() : nullptr;
			if (setting && setting->HasArchetype(a_type)) {
				return true;
			}
		}
		return false;
	}

	bool MagicTarget::HasMagicEffect(EffectSetting* a_effect)
	{
		using func_t = decltype(&MagicTarget::HasMagicEffect);
		REL::Relocation<func_t> func{ Offset::MagicTarget::HasMagicEffect };
		return func(this, a_effect);
	}

	bool MagicTarget::HasMagicEffectWithKeyword(BGSKeyword* a_keyword, std::uint64_t a_arg2)
	{
		using func_t = decltype(&MagicTarget::HasMagicEffectWithKeyword);
		REL::Relocation<func_t> func{ RELOCATION_ID(33734, 34518) };
		return func(this, a_keyword, a_arg2);
	}

	void MagicTarget::VisitEffects(ForEachActiveEffectVisitor& visitor)
	{
		using func_t = decltype(&MagicTarget::VisitEffects);
		REL::Relocation<func_t> func{ RELOCATION_ID(33756, 34540) };
		return func(this, visitor);
	}
}
