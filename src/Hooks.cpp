#include "Hooks.h"
#include "Keycode.h"
#include "Main.h"
#include "Settings.h"
#include "Utility.h"

namespace OCPA
{

	namespace Hooks
	{

		// Register PlayerCharacter for animation events.
		void AnimEvent::RegisterActor(RE::Actor* a_actor)
		{
			logger::info("Player Registered for Animation Events.");
			if (!a_actor || a_actor->IsDead()) {
				return;
			}

			a_actor->AddAnimationGraphEventSink(this);
		}

		// Listen for player animation events. (Attack, Block, etc.)
		RE::BSEventNotifyControl AnimEvent::ProcessEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
		{
			if (!a_event || !a_eventSource) {
				return RE::BSEventNotifyControl::kContinue;
			}

			Main::GetSingleton()->ReceiveAnimEvent(a_event, a_eventSource);
			return RE::BSEventNotifyControl::kContinue;
		}

		// Not really a hook, lol. Just a sink for input events.
		RE::BSEventNotifyControl InputHook::ProcessEvent(const InputEvents* evns, RE::BSTEventSource<InputEvents>* dispatcher)
		{
			if (!*evns || !dispatcher) {
				return RE::BSEventNotifyControl::kContinue;
			}

			Main::GetSingleton()->ReceiveInputEvents(evns, dispatcher);
			return RE::BSEventNotifyControl::kContinue;
		}

		// Receive and process attack and block events.
		// Events not considered attack or block are passed to the original handler.
		void HookAttackBlockHandler::ProcessButton(RE::ButtonEvent* a_event, void* a_data)
		{
			// TODO: Lazy load player registration.
			if (!AnimEvent::GetSingleton()->IsPlayerRegistered) {
				AnimEvent::GetSingleton()->RegisterActor(RE::PlayerCharacter::GetSingleton());
				AnimEvent::GetSingleton()->IsPlayerRegistered = true;
			}

			// If player is in first person, pass all events to the original handler.
			if (auto player = RE::PlayerCharacter::GetSingleton()) {
				if (!player->Is3rdPersonVisible()) {
					FnProcessButton fn = fnHash.at(*(uintptr_t*)this);

					if (fn) {
						(this->*fn)(a_event, a_data);
						return;
					}
				}
			}

			auto main = Main::GetSingleton();
			auto config = Settings::GetSingleton()->GetConfig();

			auto isBlockKey = config->blockKey[a_event->device.get()];
			auto keyCode = Utility::GetKeycode(a_event->AsButtonEvent());

			if (keyCode == isBlockKey && config->disableBlockDuringAttack) {
				if (auto player = RE::PlayerCharacter::GetSingleton()) {
					RE::ATTACK_STATE_ENUM state = player->AsActorState()->actorState1.meleeAttackState;
					if (state > RE::ATTACK_STATE_ENUM::kNone && state < RE::ATTACK_STATE_ENUM::kBash) {
						return;
					}
				}
			}

			if (main->IsAttackEvent(a_event)) {
				if (!main->ProcessAttackEvent(a_event, a_data)) {
					return;
				}
			}

			FnProcessButton fn = fnHash.at(*(uintptr_t*)this);

			if (fn) {
				(this->*fn)(a_event, a_data);
			}
		}

		// Hook into the AttackBlockHandler vtable to process attack and block events.
		void HookAttackBlockHandler::Hook()
		{
			REL::Relocation<uintptr_t> vtable{ RE::VTABLE_AttackBlockHandler[0] };
			FnProcessButton fn =
				stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &HookAttackBlockHandler::ProcessButton));
			fnHash.insert(std::pair<uintptr_t, FnProcessButton>(vtable.address(), fn));
		}

		//Fired on menu events. Used to lazy load ini settings after MCM and load screen.
		class MenuWatcher : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
		{
		public:
			virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* evn, RE::BSTEventSource<RE::MenuOpenCloseEvent>* dispatcher) override
			{
				(void)dispatcher;

				//Load Settings after load screen.
				if (evn->menuName == RE::InterfaceStrings::GetSingleton()->loadingMenu && evn->opening) {
					Settings::GetSingleton()->Load();
				}

				//Load key settings after closing MCM. (journal).
				else if (evn->menuName == RE::InterfaceStrings::GetSingleton()->journalMenu && !evn->opening) {
					Settings::GetSingleton()->Load();
				}

				return RE::BSEventNotifyControl::kContinue;
			}
		};

		void Install()
		{
			HookAttackBlockHandler::Hook();

			MenuWatcher* mw = new MenuWatcher();
			RE::UI::GetSingleton()->AddEventSink(mw);

			auto _InputSink = RE::BSInputDeviceManager::GetSingleton();
			_InputSink->AddEventSink(InputHook::GetSingleton());
		};
	}

}