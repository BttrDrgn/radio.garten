
#include "logger/logger.hpp"
#include "fs/fs.hpp"

#include "settings.hpp"
#include "menus/menus.hpp"
#include "hook/hook.hpp"
#include "audio/audio.hpp"

void settings::init()
{
	settings::config_file = fs::get_self_path() + settings::config_file;

	settings::update();
}

void settings::update()
{
	if(fs::exists(settings::config_file)) settings::config = ini_load(&settings::config_file[0]);
	else if (!fs::exists(settings::config_file))
	{
		const char* ini_default =
			"[core]\n"
			"playlist = \"Music\"";

		settings::config = ini_create(ini_default, strlen(ini_default));

		ini_save(settings::config, &settings::config_file[0]);
	}

	audio::playlist_name = ini_get(config, "core", "playlist");
	audio::playlist_dir = audio::playlist_name;

	ini_free(settings::config);
}

bool settings::get_boolean(const char* bool_text)
{
	if (!std::strcmp(bool_text, "true")) return true;
	else return false;
}

std::string settings::config_file = "config.ini";
ini_t* settings::config;
