#pragma once

namespace OCPA
{
	class Settings
	{
	public:
		void LoadGlobals();
		void Load();

		enum LongPressMode : uint32_t
		{
			kVanilla = 0,
			kDisable = 1,
			kRepeat = 2
		};

		struct Globals
		{
			RE::PlayerCharacter* player;
		};

		struct Config
		{
			int normalKey = 0xFF;
			int blockKeybind = 0xFF;
			int paKey = 0xFF;
			int modifierKey = 0xFF;
			int longPressMode = 0xFF;
			bool allowZeroStamina = false;
			bool onlyFirstAttack = false;
			bool onlyDuringAttack = false;
			bool disableBlockDuringAttack = false;
			bool disableBlockKey = false;

			int dualPaKey = 0xFF;
			int dualModifierKey = 0xFF;
			bool dualOnlyFirstAttack = false;
			bool dualOnlyDuringAttack = false;

			bool notifyWindow = false;
			float notifyDuration = 0.0f;
			std::string notifyFXStr;
			bool queuePA = false;
			float queuePAExpire = 0.0f;

			bool debugPAPress = false;
			bool debugPAActivate = false;
			float powerAttackCooldown = 0.0f;
			std::string MCOWinOpen;
			std::string MCOWinClose;
			std::string MCOPowerWinOpen;
			std::string MCOPowerWinClose;

			RE::TESEffectShader* notifyFX = nullptr;

			uint32_t attackKey[RE::INPUT_DEVICE::kTotal] = { 0xFF, 0xFF, 0xFF };
			uint32_t blockKey[RE::INPUT_DEVICE::kTotal] = { 0xFF, 0xFF, 0xFF };
		};

		[[nodiscard]] static inline Settings* GetSingleton()
		{
			static Settings singleton;
			return &singleton;
		}

		[[nodiscard]] inline Config* GetConfig()
		{
			return &config;
		}

		[[nodiscard]] inline Globals& GetGlobals()
		{
			return globals;
		}

	private:
		Config config;
		Globals globals;
	};
}