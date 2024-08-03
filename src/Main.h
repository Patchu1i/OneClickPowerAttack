#pragma once

#include "Hooks.h"
#include "Settings.h"
#include <PCH.h>

namespace OCPA
{
	using InputEvents = RE::InputEvent*;

	class Main
	{
	public:
		void PerformAction(RE::BGSAction* action, RE::Actor* a);
		void AltPowerAttack(RE::Actor* player);
		void PowerAttack(RE::Actor* player);
		void Attack(RE::Actor* player);
		void DualPowerAttack(RE::Actor* player);
		bool IsPAQueued();

		bool IsAttackEvent(RE::ButtonEvent* a_event);
		bool CanPowerAttack();

		void ReceiveAnimEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_source);
		void ReceiveInputEvents(const InputEvents* a_events, RE::BSTEventSource<InputEvents>* a_source);
		bool ProcessAttackEvent(RE::ButtonEvent* a_event, void* a_data);
		bool ProcessBlockEvent(RE::ButtonEvent* a_event, void* a_data);

		static inline Main* GetSingleton()
		{
			static Main singleton;
			return &singleton;
		}

		void Load();

		bool sendEvent = false;

		// Saved Globals
		RE::PlayerCharacter* player;
		RE::ControlMap* controlMap;
		RE::UI* menu;
		OCPA::Settings::Config* config;

		// State & Timers
		bool attackWindow = false;
		bool hitWindow = false;
		bool isAttacking = false;
		float paQueueTime = 0.f;
		float queuePAExpire = 0.2f;  // ?
		RE::BGSAction* queuePAAction = nullptr;

		// Persistent keypress state
		bool dualKeyComboPressed = false;
		bool keyComboPressed = false;

		// RE
		RE::BGSAction* actionRightAttack = (RE::BGSAction*)RE::TESForm::LookupByID(0x13005);
		RE::BGSAction* actionRightPowerAttack = (RE::BGSAction*)RE::TESForm::LookupByID(0x13383);
		RE::BGSAction* actionDualPowerAttack = (RE::BGSAction*)RE::TESForm::LookupByID(0x2E2F7);
		RE::BGSSoundDescriptorForm* debugPAPressSound = (RE::BGSSoundDescriptorForm*)RE::TESForm::LookupByID(0x3F3F8);
		RE::BGSSoundDescriptorForm* debugPAActivateSound = (RE::BGSSoundDescriptorForm*)RE::TESForm::LookupByID(0x3F3FA);

		REL::Relocation<float*> ptr_engineTime{ RELOCATION_ID(517597, 404125) };
		REL::Relocation<RE::Setting*> fInitialPowerAttackDelay{ RELOCATION_ID(509496, 381954) };
	};
}