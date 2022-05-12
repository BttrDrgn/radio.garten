
#include "logger/logger.hpp"
#include "fs/fs.hpp"

#include "settings.hpp"
#include "menus/menus.hpp"
#include "api/api.hpp"

void settings::init()
{
	settings::update();
	settings::favorites_update();
}

void settings::update()
{
	if(fs::exists(settings::config_file)) settings::config = ini_load(&settings::config_file[0]);
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

		ini_save(settings::config, &settings::config_file[0]);
	}

	menus::show_drpc = settings::get_boolean(ini_get(settings::config, "startup", "Discord"));

#ifndef OVERLAY
	global::use_hardware = settings::get_boolean(ini_get(settings::config, "core", "UseGPU"));
	menus::show_snow = settings::get_boolean(ini_get(settings::config, "startup", "Snow"));
#endif

	if (settings::get_boolean(ini_get(settings::config, "startup", "Refresh")))
	{
		api::get_places();
	}

	ini_free(settings::config);
}

void settings::favorites_update()
{
	if (!fs::exists(settings::favorites_file))
	{
		fs::write(settings::favorites_file, "", false);
	}

	std::string fav_contents = fs::read(settings::favorites_file);
	std::vector<std::string> stations = logger::split(fs::read(settings::favorites_file), '|');

	if (stations.size() > 0)
	{
		api::favorite_stations.clear();

		for (const std::string& station : stations)
		{
			std::vector<std::string> temp = logger::split(station, ',');

			if (temp.size() == 5)
			{
				station_t s;

				s.title = temp[0];

				s.place.country = temp[1];
				s.place.city = temp[2];
				s.place.id = temp[3];

				s.id = temp[4];

				api::favorite_stations.emplace_back(s);
			}
		}
	}
}

void settings::add_favorite(station_t station)
{
	std::string contents;

	contents.append(station.title);
	contents.append(",");

	contents.append(station.place.country);
	contents.append(",");
	contents.append(station.place.city);
	contents.append(",");
	contents.append(station.place.id);
	contents.append(",");

	contents.append(station.id);

	contents.append("|");

	logger::va("%s", &contents[0]);

	fs::write(settings::favorites_file, contents, true);
}

void settings::remove_favorite(station_t station)
{
	std::string contents = fs::read(settings::favorites_file);
	std::vector<station_t> stations;

	std::vector<std::string> contents_split = logger::split(contents, '|');

	for (const std::string& c : contents_split)
	{
		std::vector<std::string> temp = logger::split(c, ',');

		if (temp.size() == 5)
		{
			if (station.id.compare(temp[4]))
			{
				station_t s;

				s.title = temp[0];

				s.place.country = temp[1];
				s.place.city = temp[2];
				s.place.id = temp[3];

				s.id = temp[4];

				stations.emplace_back(s);
			}
		}
	}

	//Wipe
	fs::write(settings::favorites_file, "", false);

	for (const station_t& s : stations)
	{
		//Add
		settings::add_favorite(s);
	}
}

bool settings::get_boolean(const char* bool_text)
{
	if (!std::strcmp(bool_text, "true")) return true;
	else return false;
}

std::string settings::config_file = logger::va("%s%s", &fs::get_pref_dir()[0], "config.ini");
std::string settings::favorites_file = logger::va("%s%s", &fs::get_pref_dir()[0], "stations.fav");;
ini_t* settings::config;
