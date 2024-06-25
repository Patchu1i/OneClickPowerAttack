#pragma once

#include <PCH.h>

namespace OCPA
{
	namespace Hooks
	{
		void Install();

		class AnimEvent : public RE::BSTEventSink<RE::BSAnimationGraphEvent>
		{
		public:
			static inline AnimEvent* GetSingleton()
			{
				static AnimEvent singleton;
				return &singleton;
			}

			bool IsPlayerRegistered = false;
			void RegisterActor(RE::Actor* a_actor);

		private:
			RE::BSEventNotifyControl ProcessEvent(const RE::BSAnimationGraphEvent* a_event,
				RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource) override;
		};

		using InputEvents = RE::InputEvent*;
		class InputHook : public RE::BSTEventSink<InputEvents>
		{
		public:
			virtual RE::BSEventNotifyControl ProcessEvent(const InputEvents* evns, RE::BSTEventSource<InputEvents>* dispatcher) override;

			static inline InputHook* GetSingleton()
			{
				static InputHook singleton;
				return &singleton;
			}
		};

		class HookAttackBlockHandler
		{
		public:
			typedef void (HookAttackBlockHandler::*FnProcessButton)(RE::ButtonEvent*, void*);
			void ProcessButton(RE::ButtonEvent* a_event, void* a_data);
			static void Hook();

		private:
			static inline std::unordered_map<uintptr_t, FnProcessButton> fnHash;
		};

	}
}