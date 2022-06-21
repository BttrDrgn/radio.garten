/*
	BASS internet radio example
	Copyright (c) 2002-2021 Un4seen Developments Ltd.
*/

#include "logger/logger.hpp"
#include "fs/fs.hpp"
#include "global.hpp"
#include "audio.hpp"
#include "hook/hook.hpp"
#include "settings/settings.hpp"

void play_file(const char* file, int channel)
{
	audio::chan[channel] = BASS_StreamCreateFile(0, file, 0, 0, BASS_SAMPLE_FLOAT | BASS_STREAM_PRESCAN);

	if (audio::chan != 0 && BASS_ChannelPlay(audio::chan[channel], false))
	{
		audio::playing = true;
		std::string title = "N/A";
		std::string artist = "N/A";

		std::string temp = file;
		logger::rem_path_info(temp, audio::playlist_dir);
		std::vector<std::string> info = logger::split(temp, " - ");

		if (info.size() == 1)
		{
			title = info[0];
		}
		else if (info.size() == 2)
		{
			artist = info[0];
			title = info[1].erase(0, 2);
		}

		hook::SummonChyron(title.c_str(), artist.c_str(), audio::playlist_name.c_str());
	}
}
