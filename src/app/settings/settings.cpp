
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
	ini_t* config;

	if (fs::exists(settings::config_file))
	{
		config = ini_load(&settings::config_file[0]);
		std::string config_string = ini_tostring(config);

		audio::playlist_name = ini_get(config, "core", "playlist");
		audio::playlist_dir = audio::playlist_name;
		audio::playlist_dir = fs::get_self_path() + audio::playlist_dir;
		audio::enumerate_playlist();

		std::string trax_config = "[trax]\n";

		for (int i = 0; i < audio::playlist_files.size(); ++i)
		{
			std::string song = audio::playlist_files[i].first;
			song.erase(0, audio::playlist_dir.size() + 1);
			if (config_string.find(song) != std::string::npos)
			{
				const char* res = ini_get(config, "trax", song.c_str());
				audio::playlist_files[i].second = res;
			}
			else
			{
				trax_config.append(logger::va("%s = ALL\n", song.c_str()));
				audio::playlist_files[i].second = "ALL";
			}
		}

		ini_merge(config, ini_create(trax_config.c_str(), strlen(trax_config.c_str())));
		ini_save(config, settings::config_file.c_str());
	}
	else if (!fs::exists(settings::config_file))
	{
		std::string ini_default =
			"[core]\n"
			"playlist = \"Music\"\n\n"
			"[trax]\n";

		audio::playlist_name = "Music";
		audio::playlist_dir = audio::playlist_name;
		audio::playlist_dir = fs::get_self_path() + audio::playlist_dir;
		audio::enumerate_playlist();

		for (int i = 0; i < audio::playlist_files.size(); ++i)
		{
			std::string song = audio::playlist_files[i].first;
			song.erase(0, audio::playlist_dir.size() + 1);
			ini_default.append(logger::va("%s = ALL\n", song.c_str()));
		}

		config = ini_create(ini_default.c_str(), strlen(ini_default.c_str()));

		ini_save(config, settings::config_file.c_str());
	}

	ini_free(config);
}

bool settings::get_boolean(const char* bool_text)
{
	if (!std::strcmp(bool_text, "true")) return true;
	else return false;
}

//Hardcoded until x64 becomes useable
std::string settings::config_file = "ecm.x86.ini";
