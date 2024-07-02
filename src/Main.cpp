#include "Main.h"
#include "Hooks.h"
#include "Settings.h"
#include "Utility.h"

namespace OCPA
{

	// Called by Settings::Load to grab references.
	void Main::Load()
	{
		player = RE::PlayerCharacter::GetSingleton();
		controlMap = RE::ControlMap::GetSingleton();
		menu = RE::UI::GetSingleton();
		config = Settings::GetSingleton()->GetConfig();
	}

	bool Main::IsPAQueued()
	{
		return *ptr_engineTime - paQueueTime <= queuePAExpire;
	}

	void Main::PerformAction(RE::BGSAction* action, RE::Actor* a)
	{
		SKSE::GetTaskInterface()->AddTask([this, action, a]() {
			std::unique_ptr<RE::TESActionData> data(RE::TESActionData::Create());
			data->source = RE::NiPointer<RE::TESObjectREFR>(a);
			data->action = action;
			typedef bool func_t(RE::TESActionData*);
			REL::Relocation<func_t> func{ RELOCATION_ID(40551, 41557) };
			bool succ = func(data.get());
			if (succ) {
				if (config->debugPAActivate && action->formID != 0x13005) {
					Utility::PlayDebugSound(debugPAActivateSound, player);
				}
				if (IsPAQueued() && action == queuePAAction) {
					paQueueTime = 0.f;
				}
			} else {
				if (config->queuePA) {
					paQueueTime = *ptr_engineTime;
					queuePAAction = action;
				}
			}
		});
	}

	void Main::Attack(RE::Actor* a_actor)
	{
		if (config->debugPAPress) {
			Utility::PlayDebugSound(debugPAPressSound, a_actor);
		}

		bool isJumping = false;
		a_actor->GetGraphVariableBool("bInJumpState", isJumping);

		bool isBlocking = false;
		a_actor->GetGraphVariableBool("IsBlocking", isBlocking);

		PerformAction(actionRightAttack, a_actor);
	}

	//For Jumping Attack/Vanguard
	void Main::AltPowerAttack(RE::Actor* a_actor)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(200));  // the fuck
		PerformAction(actionRightPowerAttack, a_actor);
	}

	// Right-hand Power Attack
	void Main::PowerAttack(RE::Actor* a_actor)
	{
		if (config->debugPAPress)
			Utility::PlayDebugSound(debugPAPressSound, a_actor);

		bool isJumping = false;
		a_actor->GetGraphVariableBool("bInJumpState", isJumping);

		bool isBlocking = false;
		a_actor->GetGraphVariableBool("IsBlocking", isBlocking);

		if (isJumping || isBlocking) {
			PerformAction(actionRightAttack, a_actor);
			std::thread thread(&Main::AltPowerAttack, Main::GetSingleton(), a_actor);
			thread.detach();
		} else {
			PerformAction(actionRightPowerAttack, a_actor);
		}
	}

	// Left-hand Power Attack (Not necessarily dual-wield specific, I learned).
	void Main::DualPowerAttack(RE::Actor* a_actor)
	{
		PerformAction(actionDualPowerAttack, a_actor);
	}

	// Recevies animation graph events from sink, used to flag isAttacking and attackWindow.
	void Main::ReceiveAnimEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_source)
	{
		(void)a_source;  // unused

		RE::Actor* actor = stl::unrestricted_cast<RE::Actor*>(a_event->holder);

		if (!actor) {
			return;
		}

		// Determining attack state based on animation graph.
		RE::ATTACK_STATE_ENUM currentAnimState = (actor->AsActorState()->actorState1.meleeAttackState);
		if (currentAnimState >= RE::ATTACK_STATE_ENUM::kDraw && currentAnimState <= RE::ATTACK_STATE_ENUM::kBash) {
			isAttacking = true;
		} else {
			isAttacking = false;
		}

		// MCO Specific implementation to flag attack window for succession.
		if (a_event->tag == config->MCOWinOpen.c_str()) {
			attackWindow = true;
			if (config->notifyWindow) {
				if (config->notifyFX) {
					player->ApplyEffectShader(config->notifyFX, config->notifyDuration);
				} else {
					logger::critical("NotifyEffect is not valid!");
				}
			}
		} else if (a_event->tag == config->MCOWinClose.c_str()) {
			attackWindow = false;
		} else if (IsPAQueued() && (a_event->tag == config->MCOPowerWinOpen.c_str() ||
									   a_event->tag == config->MCOPowerWinClose.c_str())) {
			PerformAction(queuePAAction, player);
		}
	}

	// Determine if the player is able to power attack through OCPA.
	bool Main::CanPowerAttack()
	{
		if (!player) {
			return false;
		}

		if (Utility::IsRidingHorse(player) || Utility::IsInKillmove(player)) {
			return false;
		}

		uint32_t controlFlag = (uint32_t)RE::UserEvents::USER_EVENT_FLAG::kMovement &
		                       (uint32_t)RE::UserEvents::USER_EVENT_FLAG::kLooking;

		if (menu->numPausesGame > 0) {
			return false;
		}
		if ((controlMap->enabledControls.underlying() & controlFlag) != controlFlag) {
			return false;
		}
		if (menu->IsMenuOpen("Dialogue Menu"sv)) {
			return false;
		}
		if (player->AsActorState()->actorState1.sitSleepState != RE::SIT_SLEEP_STATE::kNormal) {
			return false;
		}
		if (!config->allowZeroStamina && player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStamina) <= 0) {
			return false;
		}

		return true;
	}

	// Hijack the attack/block handler under these conditions:
	// A) The player is valid, and not in a killmove or riding a horse.
	// B) The event is a normal attack or power attack, and not a block.
	// C) The player is not in a menu, dialogue, knock state, or flystate
	// D) The player is not sleeping
	// E) The player is using a valid weapon
	bool Main::IsAttackEvent(RE::ButtonEvent* a_event)
	{
		if (config->onlyDuringAttack == true && !isAttacking) {
			return false;
		}

		bool normalAttack = Utility::IsNormalAttack(a_event);
		bool powerAttack = Utility::IsPowerAttack(a_event, keyComboPressed, isAttacking);

		// Check keycode against current bindings.
		if (!normalAttack && !powerAttack) {
			return false;
		}

		if (player == nullptr || player->IsInKillMove() || Utility::IsRidingHorse(player)) {
			return false;
		}

		bool isBlocking = false;
		player->GetGraphVariableBool("IsBlocking", isBlocking);

		if (isBlocking) {
			return false;
		}

		RE::ATTACK_STATE_ENUM currentState = (player->AsActorState()->actorState1.meleeAttackState);
		if (currentState >= RE::ATTACK_STATE_ENUM::kNone && currentState < RE::ATTACK_STATE_ENUM::kBash) {
			// kNone = 0,
			// kDraw = 1,
			// kSwing = 2,
			// kHit = 3,
			// kNextAttack = 4,
			// kFollowThrough = 5,
		} else {
			return false;  // kBash = 6,
		}

		auto actorState = player->AsActorState();
		if (actorState == NULL || actorState && (actorState->GetWeaponState() != RE::WEAPON_STATE::kDrawn ||
													actorState->actorState1.sitSleepState != RE::SIT_SLEEP_STATE::kNormal ||
													actorState->actorState1.knockState != RE::KNOCK_STATE_ENUM::kNormal ||
													actorState->actorState1.flyState != RE::FLY_STATE::kNone)) {
			return false;
		}

		uint32_t controlFlag = (uint32_t)RE::UserEvents::USER_EVENT_FLAG::kMovement &
		                       (uint32_t)RE::UserEvents::USER_EVENT_FLAG::kLooking;

		if (menu->numPausesGame > 0) {
			return false;
		}
		if ((controlMap->enabledControls.underlying() & controlFlag) != controlFlag) {
			return false;
		}
		if (menu->IsMenuOpen("Dialogue Menu"sv)) {
			return false;
		}

		// TODO: Implement dual wield checks (?)
		//auto weaponLeft = reinterpret_cast<RE::TESObjectWEAP*>(player->GetEquippedObject(true));
		// if (player->GetEquippedObject(true) && !Utility::IsValidWeapon(weaponLeft)) {
		// 	logger::critical("Invalid weapon equipped in left hand.");
		// 	return false;
		// }

		auto weaponRight = reinterpret_cast<RE::TESObjectWEAP*>(player->GetEquippedObject(false));
		if (player->GetEquippedObject(false) && !Utility::IsValidWeapon(weaponRight)) {
			logger::critical("Invalid weapon equipped in right hand.");
			return false;
		}

		return true;
	}

	// Simply intercept block events and return false if the player is attacking.
	// This is to prevent the player from blocking during an attack if disableBlockDuringAttack == true.
	// Return (true) to allow non-captured events to fire.
	bool Main::ProcessBlockEvent(RE::ButtonEvent* a_event, void* a_data)
	{
		(void)a_data;

		bool isBlocking = false;
		player->GetGraphVariableBool("IsBlocking", isBlocking);

		if (isBlocking) {
			return true;  // early out
		}

		RE::ATTACK_STATE_ENUM currentState = (player->AsActorState()->actorState1.meleeAttackState);
		if (currentState == RE::ATTACK_STATE_ENUM::kBash) {
			return true;  // early out
		} else if (currentState == RE::ATTACK_STATE_ENUM::kHit) {
			return true;  // MCO recovery fires here.
		}

		bool isBlockKey = config->blockKey[a_event->device.get()];
		if (isBlockKey) {
			if (config->disableBlockDuringAttack == true && isAttacking) {
				return false;
			}
		}

		return true;
	}

	bool Main::ProcessAttackEvent(RE::ButtonEvent* a_event, void* a_data)
	{
		(void)a_data;

		bool isAttackKey = config->attackKey[a_event->device.get()];

		// Allow for power attack to be queued during attack.
		if (isAttackKey && a_event->IsUp()) {
			player->NotifyAnimationGraph("attackRelease");
		}

		if (Utility::IsNormalAttack(a_event)) {
			switch (config->longPressMode) {
			case Settings::LongPressMode::kVanilla:  // 0
				if (a_event->IsHeld() && a_event->HeldDuration() >= fInitialPowerAttackDelay->GetFloat()) {
					PowerAttack(player);
				} else if (a_event->IsDown()) {
					Attack(player);
				}
				break;
			case Settings::LongPressMode::kDisable:  // 1
				if (a_event->heldDownSecs >= fInitialPowerAttackDelay->GetFloat()) {
					return false;
				}

				if (a_event->IsDown()) {
					Attack(player);
				}
				break;
			case Settings::LongPressMode::kRepeat:  // 2
				if (a_event->IsDown() && !isAttacking) {
					Attack(player);
				} else if (a_event->IsHeld() && isAttacking) {
					Attack(player);
				}
				break;
			default:  // kDisable
				if (a_event->IsDown()) {
					Attack(player);
				}
				break;
			}
		}

		return true;
	}

	// This function is responsible for handling alternative power attack keybinds
	// by listening for all keycodes, and processing powerattack inputs.
	// We do this outside of AttackBlockHandler to handle power attacks as a "post condition".
	void Main::ReceiveInputEvents(const InputEvents* a_events, RE::BSTEventSource<InputEvents>* a_source)
	{
		(void)a_source;

		if (!CanPowerAttack()) {
			return;
		}

		// Iterate over all input events pushed to sink.
		for (RE::InputEvent* a_event = *a_events; a_event; a_event = a_event->next) {
			switch (a_event->eventType.get()) {
			case RE::INPUT_EVENT_TYPE::kButton:
				RE::ButtonEvent* b_event = a_event->AsButtonEvent();
				uint32_t keyCode = Utility::GetKeycode(b_event);

				bool wantPowerAttack = false;
				bool wantDualPowerAttack = false;

				if (keyCode == (uint32_t)config->modifierKey && b_event->IsDown()) {
					keyComboPressed = true;
				}
				if (keyCode == (uint32_t)config->modifierKey && b_event->IsUp()) {
					keyComboPressed = false;
				}

				// On keypress, if onlyDuringAttack check if isAttacking.
				// If modifierKey is set, check if keyComboPressed.
				// if onlyFirstAttack, check if isAttacking.
				if (keyCode == (uint32_t)config->paKey && b_event->IsDown()) {
					if (isAttacking && config->onlyDuringAttack || config->onlyDuringAttack == false) {
						if (config->modifierKey >= 2) {
							if (config->onlyFirstAttack) {
								if (isAttacking) {
									wantPowerAttack = true;  // Allow without keyCombo
								}

								if (!isAttacking && keyComboPressed) {  // Require keyCombo
									wantPowerAttack = true;
								}
							} else {
								if (keyComboPressed) {
									wantPowerAttack = true;
								}
							}
						} else {
							wantPowerAttack = true;
						}
					}
				}

				if (keyCode == (uint32_t)config->dualModifierKey && b_event->IsDown())
					dualKeyComboPressed = true;
				if (keyCode == (uint32_t)config->dualModifierKey && b_event->IsUp())
					dualKeyComboPressed = false;

				// Repeat for dual wield
				if (keyCode == (uint32_t)config->dualPaKey && b_event->IsDown()) {
					if (isAttacking && config->onlyDuringAttack || config->onlyDuringAttack == false) {
						if (config->dualModifierKey >= 2) {
							if (config->onlyFirstAttack) {
								if (isAttacking) {
									wantDualPowerAttack = true;  // Allow without keyCombo
								}

								if (!isAttacking && dualKeyComboPressed) {  // Require keyCombo
									wantDualPowerAttack = true;
								}
							} else {
								if (dualKeyComboPressed) {
									wantDualPowerAttack = true;
								}
							}
						} else {
							wantDualPowerAttack = true;
						}
					}
				}

				// If both power attacks are bound to the same key, check for modifier key.
				// If both power attacks have the same modifier, just send PowerAttack
				// otherwise send the power attack corresponding to the modifier key.
				if (wantPowerAttack && wantDualPowerAttack) {
					if (config->modifierKey >= 2 && config->dualModifierKey <= 2) {
						PowerAttack(player);
						break;
					} else if (config->modifierKey <= 2 && config->dualModifierKey >= 2) {
						DualPowerAttack(player);
						break;
					} else {
						PowerAttack(player);
						break;
					}
				} else {
					if (wantPowerAttack) {
						PowerAttack(player);
						break;
					}
					if (wantDualPowerAttack) {
						DualPowerAttack(player);
						break;
					}
				}
			}
		}
	}
}