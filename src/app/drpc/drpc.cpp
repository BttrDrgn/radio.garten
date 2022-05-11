#include "logger/logger.hpp"
#include "global.hpp"
#include "audio/audio.hpp"
#include "menus/menus.hpp"

#include "drpc.hpp"

discord::Core* core{};
discord::Activity activity{};

void drpc::init()
{
	if (!drpc::initalized)
	{
		discord::Result result = discord::Core::Create(971461199919202344, DiscordCreateFlags_NoRequireDiscord, &core);
		if (result == discord::Result::Ok)
		{
			core->SetLogHook(discord::LogLevel::Info, [](auto, auto message)
			{
				logger::log("DISCORD", message);
			});

			core->SetLogHook(discord::LogLevel::Warn, [](auto, auto message)
			{
				logger::log("DISCORD", message);

			});

			core->SetLogHook(discord::LogLevel::Error, [](auto, auto message)
			{
				logger::log("DISCORD", message);
			});

			drpc::change_large_image("logo", "Radio.Garten");
			activity.GetTimestamps().SetStart(std::time(nullptr));

			drpc::update();
			drpc::initalized = true;

			std::thread([]() -> void
			{
				while (menus::show_drpc)
				{
					core->RunCallbacks();

					drpc::change_large_image("logo", &logger::va("%s, %s", &audio::currently_playing.region.city[0], &audio::currently_playing.region.country[0])[0]);
					drpc::change_details(logger::va("Listening to %s", &audio::currently_playing.title[0]));
					drpc::change_status(logger::va("on %s", &audio::currently_playing.station.title[0]));

					std::this_thread::sleep_for(50ms);
				}
			}).detach();
		}
	}
}

void drpc::deinit()
{
	if (drpc::initalized)
	{
		core = {};
		activity = {};
		drpc::initalized = false;
	}
}

void drpc::change_status(const std::string& text)
{
	activity.SetState(text.c_str());

	drpc::update();
}

void drpc::change_details(const std::string& text)
{
	activity.SetDetails(text.c_str());

	drpc::update();
}

void drpc::change_large_image(const char* image, const char* text)
{
	activity.GetAssets().SetLargeImage(image);
	activity.GetAssets().SetLargeText(text);

	drpc::update();
}

void drpc::change_small_image(const std::string& image, const std::string& text)
{
	activity.GetAssets().SetSmallImage(image.c_str());
	activity.GetAssets().SetSmallText(text.c_str());

	drpc::update();
}

void drpc::update()
{
	core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
}

bool drpc::initalized = false;