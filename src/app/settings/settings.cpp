#include "global.hpp"

#include "logger/logger.hpp"
#include "fs/fs.hpp"

#include "settings.hpp"
#include "menus/menus.hpp"
#include "api/api.hpp"

void settings::init()
{
	settings::update();
}

void settings::update()
{
	if(fs::exists(settings::config_file)) settings::config = ini_load(settings::config_file);
	else if (!fs::exists(settings::config_file))
	{
		const char* ini_default =
			"[core]\n"
			"UseGPU  =	false\n"
			"[startup]\n"
			"Refresh =	false\n"
			"Discord =	false\n"
			"Snow	 =	false\n";

		settings::config = ini_create(ini_default, strlen(ini_default));

		ini_save(settings::config, settings::config_file); 
	}

	global::use_hardware = settings::get_boolean(ini_get(settings::config, "core", "UseGPU"));
	menus::show_drpc = settings::get_boolean(ini_get(settings::config, "startup", "Discord"));
	menus::show_snow = settings::get_boolean(ini_get(settings::config, "startup", "Snow"));
	if (settings::get_boolean(ini_get(settings::config, "startup", "Refresh")))
	{
		api::get_places();
	}

	ini_free(settings::config);
}

bool settings::get_boolean(const char* bool_text)
{
	if (!strcmp(bool_text, "true")) return true;
	else return false;
}

const char* settings::config_file = "config.ini";
const char* settings::favorites_file = "stations.fav";
ini_t* settings::config;
