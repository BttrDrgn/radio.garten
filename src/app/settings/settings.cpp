
#include "logger/logger.hpp"
#include "fs/fs.hpp"

#include "settings.hpp"
#include "menus/menus.hpp"
#include "hook/hook.hpp"

void settings::init()
{
	settings::update();
}

void settings::update()
{
	if(fs::exists(settings::config_file)) settings::config = ini_load(&settings::config_file[0]);
	else if (!fs::exists(settings::config_file))
	{
		const char* ini_default = "";

		settings::config = ini_create(ini_default, strlen(ini_default));

		ini_save(settings::config, &settings::config_file[0]);
	}

	ini_free(settings::config);
}

bool settings::get_boolean(const char* bool_text)
{
	if (!std::strcmp(bool_text, "true")) return true;
	else return false;
}

std::string settings::config_file = logger::va("%s%s", &fs::get_pref_dir()[0], "config.ini");
ini_t* settings::config;
