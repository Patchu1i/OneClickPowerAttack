#pragma once

#include <Keycode.h>
#include <PCH.h>
#include <Settings.h>

namespace OCPA
{
	namespace Utility
	{

		static inline bool IsRidingHorse(RE::Actor* a_actor)
		{
			return (a_actor->AsActorState()->actorState1.sitSleepState == RE::SIT_SLEEP_STATE::kRidingMount);
		};

		static inline bool IsInKillmove(RE::Actor* a_actor)
		{
			return a_actor->GetActorRuntimeData().boolFlags.all(RE::Actor::BOOL_FLAGS::kIsInKillMove);
		};

		static inline bool IsValidWeapon(RE::TESObjectWEAP* a_weapon)
		{
			if (a_weapon == NULL) {
				return false;
			}

			if (!a_weapon->IsWeapon() || a_weapon->IsBow() || a_weapon->IsCrossbow() || a_weapon->IsStaff()) {
				return false;
			}

			return true;
		};

		static inline uint32_t GetKeycode(RE::ButtonEvent* a_event)
		{
			auto device = a_event->device;

			uint32_t keyMask = a_event->idCode;
			uint32_t keyCode;

			if (device == RE::INPUT_DEVICE::kMouse) {
				keyCode = KEY_OFFSETS::kTotal + keyMask;
			} else if (device == RE::INPUT_DEVICE::kGamepad) {
				keyCode = GamepadMaskToKeycode(keyMask);
			} else {  // Keyboard
				keyCode = keyMask;
			}

			return keyCode;
		}

		static inline bool IsPowerAttack(RE::ButtonEvent* a_event)
		{
			auto* config = Settings::GetSingleton()->GetConfig();
			uint32_t keyCode = GetKeycode(a_event);

			if (keyCode == (uint32_t)config->paKey) {
				return true;
			}

			return false;
		};

		static inline bool IsNormalAttack(RE::ButtonEvent* a_event)
		{
			auto* config = Settings::GetSingleton()->GetConfig();
			uint32_t keyCode = GetKeycode(a_event);
			uint32_t attackKey = config->attackKey[a_event->device.get()];

			if (keyCode == attackKey) {
				return true;
			}

			return false;
		};

		static inline std::string SplitString(const std::string str, const std::string delimiter, std::string& remainder)
		{
			std::string ret;
			size_t i = str.find(delimiter);
			if (i == std::string::npos) {
				ret = str;
				remainder = "";
				return ret;
			}

			ret = str.substr(0, i);
			remainder = str.substr(i + 1);
			return ret;
		}

		static inline RE::TESForm* GetFormFromMod(std::string modname, uint32_t formid)
		{
			if (!modname.length() || !formid)
				return nullptr;
			RE::TESDataHandler* dh = RE::TESDataHandler::GetSingleton();
			RE::TESFile* modFile = nullptr;
			for (auto it = dh->files.begin(); it != dh->files.end(); ++it) {
				RE::TESFile* f = *it;
				if (strcmp(f->fileName, modname.c_str()) == 0) {
					modFile = f;
					break;
				}
			}
			if (!modFile)
				return nullptr;
			uint8_t modIndex = modFile->compileIndex;
			uint32_t id = formid;
			if (modIndex < 0xFE) {
				id |= ((uint32_t)modIndex) << 24;
			} else {
				uint16_t lightModIndex = modFile->smallFileCompileIndex;
				if (lightModIndex != 0xFFFF) {
					id |= 0xFE000000 | (uint32_t(lightModIndex) << 12);
				}
			}
			return RE::TESForm::LookupByID(id);
		}

		template <class T = RE::TESForm>
		static inline T* GetFormFromConfigString(const std::string str)
		{
			std::string formIDstr;
			std::string plugin = SplitString(str, "|", formIDstr);
			if (formIDstr.length() != 0) {
				uint32_t formID = std::stoi(formIDstr, 0, 16);
				T* form = skyrim_cast<T*, RE::TESForm>(GetFormFromMod(plugin, formID));
				if (form->formType == T::FORMTYPE)
					return form;
			}
			return nullptr;
		}

		static inline void PlayDebugSound(RE::BGSSoundDescriptorForm* sound, RE::Actor* player)
		{
			RE::BSSoundHandle handle;
			RE::BSAudioManager::GetSingleton()->BuildSoundDataFromDescriptor(handle, sound->soundDescriptor);
			handle.SetVolume(1.f);
			handle.SetObjectToFollow(player->Get3D());
			handle.Play();
		}

	}
}