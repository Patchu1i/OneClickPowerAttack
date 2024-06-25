#include "Settings.h"
#include "Keycode.h"
#include "Main.h"
#include "Utility.h"
#include <SimpleIni.h>

namespace OCPA
{

	void Settings::Load()
	{
		Main::GetSingleton()->Load();

		CSimpleIniA ini(true, false, false);
		ini.SetUnicode();

		std::string path = "Data\\MCM\\Config\\OCPA\\settings.ini";
		if (std::filesystem::exists("Data\\MCM\\Settings\\OCPA.ini")) {
			path = "Data\\MCM\\Settings\\OCPA.ini";
		}

		SI_Error result = ini.LoadFile(path.c_str());

		if (result >= 0) {
			config.paKey = std::stoi(ini.GetValue("General", "iKeycode", "257"));
			config.modifierKey = std::stoi(ini.GetValue("General", "iModifierKey", "-1"));
			config.longPressMode = std::stoi(ini.GetValue("General", "iLongPressMode", "2"));
			config.allowZeroStamina = std::stoi(ini.GetValue("General", "bAllowZeroStamina", "1")) > 0;
			config.onlyFirstAttack = std::stoi(ini.GetValue("General", "bSkipModifierDuringCombo", "0")) > 0;
			config.onlyDuringAttack = std::stoi(ini.GetValue("General", "bOnlyDuringAttack", "1")) > 0;
			config.disableBlockDuringAttack = std::stoi(ini.GetValue("General", "bDisableBlockDuringAttack", "1")) > 0;
			config.disableBlockKey = std::stoi(ini.GetValue("General", "bDisableBlockKey", "0")) > 0;

			config.dualPaKey = std::stoi(ini.GetValue("DualAttack", "iKeycode", "257"));
			config.dualModifierKey = std::stoi(ini.GetValue("DualAttack", "iModifierKey", "42"));
			config.dualOnlyFirstAttack = std::stoi(ini.GetValue("DualAttack", "bSkipModifierDuringCombo", "0")) > 0;
			config.dualOnlyDuringAttack = std::stoi(ini.GetValue("DualAttack", "bOnlyDuringAttack", "0")) > 0;

			config.notifyWindow = std::stoi(ini.GetValue("MCO", "bNotifyAttackWindow", "1")) > 0;
			config.notifyDuration = std::stof(ini.GetValue("MCO", "fNotifyDuration", "0.05"));
			config.notifyFXStr = ini.GetValue("MCO", "sNotifyEffect", "OCPA.esl|0xD63");
			config.queuePA = std::stoi(ini.GetValue("MCO", "bQueuePowerAttack", "1")) > 0;
			config.queuePAExpire = std::stof(ini.GetValue("MCO", "fQueueExpire", "0.2"));

			config.debugPAPress = std::stoi(ini.GetValue("Debug", "bNotifyPress", "0")) > 0;
			config.debugPAActivate = std::stoi(ini.GetValue("Debug", "bNotifyActivate", "0")) > 0;
			config.powerAttackCooldown = std::stof(ini.GetValue("Debug", "fPowerAttackCooldown", "0"));
			config.MCOWinOpen = ini.GetValue("MCO", "sMCOWinOpen", "MCO_WinOpen");
			config.MCOWinClose = ini.GetValue("MCO", "sMCOWinClose", "MCO_WinClose");
			config.MCOPowerWinOpen = ini.GetValue("MCO", "sMCOPowerWinOpen", "MCO_PowerWinOpen");
			config.MCOPowerWinClose = ini.GetValue("MCO", "sMCOPowerWinClose", "MCO_PowerWinClose");

			config.notifyFX = Utility::GetFormFromConfigString<RE::TESEffectShader>(config.notifyFXStr);

			// Adjust Game PowerAttackCooldown using override.
			for (auto it = RE::GameSettingCollection::GetSingleton()->settings.begin(); it != RE::GameSettingCollection::GetSingleton()->settings.end(); ++it) {
				if (strcmp((*it).first, "fPowerAttackCoolDownTime") == 0) {
					(*it).second->data.f = config.powerAttackCooldown;
				}
			}

			// TO-DO Save this reference?
			auto controlMap = RE::ControlMap::GetSingleton();

			// Unbind block key from all devices
			// TODO: Add a way to undo this change.
			if (config.disableBlockKey) {
				for (int device = 0; device < 3; ++device) {
					for (auto it = controlMap->controlMap[RE::UserEvents::INPUT_CONTEXT_ID::kGameplay]->deviceMappings[device].begin(); it != controlMap->controlMap[RE::UserEvents::INPUT_CONTEXT_ID::kGameplay]->deviceMappings[device].end(); ++it) {
						if ((*it).eventID == RE::UserEvents::GetSingleton()->leftAttack) {
							(*it).inputKey = 0xFF;
						}
					}
				}
				RE::UserEventEnabled evn;
				evn.newUserEventFlag = controlMap->enabledControls;
				evn.oldUserEventFlag = controlMap->enabledControls;
				controlMap->SendEvent(&evn);
			}

			SetKeySettings(config.attackKey, config.blockKey);
		} else {
			logger::critical("Failed to load config.");
		}
		ini.Reset();

		logger::info("Settings Loaded");
	}
}