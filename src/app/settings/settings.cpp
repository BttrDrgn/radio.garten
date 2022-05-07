#include "global.hpp"

#include "logger/logger.hpp"
#include "fs/fs.hpp"

#include "settings.hpp"

void settings::init()
{
	settings::update();
}

void settings::update()
{
	if(fs::exists(settings::config_file)) settings::config = ini_load(settings::config_file);
	if(fs::exists(settings::favorites_file)) settings::favorites = ini_load(settings::favorites_file);
}

const char* settings::config_file = "config.ini";
const char* settings::favorites_file = "favorites.ini";
ini_t* settings::config;
ini_t* settings::favorites;