#define DLLEXPORT __declspec(dllexport)

#include <Hooks.h>
#include <Main.h>
#include <Settings.h>
#include <SimpleIni.h>
#include <Xinput.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <stddef.h>

namespace
{
	void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
		case SKSE::MessagingInterface::kDataLoaded:
			OCPA::Hooks::Install();
			OCPA::Settings::GetSingleton()->Load();
			break;
		case SKSE::MessagingInterface::kPostLoad:
			break;
		}
	}
	void SetupLog()
	{
		auto logsFolder = SKSE::log::log_directory();
		if (!logsFolder)
			SKSE::stl::report_and_fail("SKSE log_directory not provided, logs disabled.");
		auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
		auto logFilePath = *logsFolder / std::format("{}.log", pluginName);
		auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
		auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));
		spdlog::set_default_logger(std::move(loggerPtr));
		spdlog::set_level(spdlog::level::trace);
		spdlog::flush_on(spdlog::level::trace);
	}
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	SetupLog();
	logger::info("Loaded plugin {} {}", Plugin::NAME, Plugin::VERSION.string());

	SKSE::Init(a_skse);

	//Install SKSE hooks.
	auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
		return false;
	}

	logger::info("Finished Loading");

	return true;
}

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() noexcept {
	SKSE::PluginVersionData v;
	v.PluginName(Plugin::NAME.data());
	v.PluginVersion(Plugin::VERSION);
	v.UsesAddressLibrary(true);
	v.HasNoStructUse();
	return v;
}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo* pluginInfo)
{
	pluginInfo->name = SKSEPlugin_Version.pluginName;
	pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
	pluginInfo->version = SKSEPlugin_Version.pluginVersion;
	return true;
}