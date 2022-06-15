#include "logger/logger.hpp"
#include "global.hpp"

#include "audio.hpp"
#include "player.hpp"
#include "fs/fs.hpp"
#include "hook/hook.hpp"

void audio::init_overlay()
{
	if (HIWORD(BASS_GetVersion()) != BASSVERSION)
	{
		global::msg_box("ECM BASS", "An incorrect version of BASS.DLL was loaded!");
		global::shutdown = true;
	}

	if (!BASS_Init(-1, 44100, 0, global::hwnd, 0))
	{
		global::msg_box("ECM BASS", "Can't initialize device!\nNo audio will play for this session!");
	}

	BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST, 1); // enable playlist processing
	audio::set_volume(audio::volume);

	audio::update();
}

void audio::update()
{
	std::thread([] {
		while (true)
		{
			std::uint32_t state = BASS_ChannelIsActive(audio::chan);
			logger::log_debug(logger::va("%i", state));

			switch (state)
			{
			case BASS_ACTIVE_STOPPED:
				audio::play_next_song();
				break;
			}

			std::this_thread::sleep_for(1s);
		}
	}).detach();
}

void audio::play_file(const std::string& file)
{
	std::thread([file]
	{
		std::string title = file;
		logger::rem_path_info(title, audio::playlist_dir);
		audio::currently_playing.title = title;
		::play_file(file.c_str());
	}).detach();
}

void audio::stop()
{
	BASS_StreamFree(audio::chan);
	audio::chan = 0;
	audio::currently_playing.title = "N/A";
}

void audio::shuffle()
{
	static std::random_device rd;
	static std::mt19937 mt(rd());
	static std::uniform_int_distribution<std::int32_t> range(0, audio::playlist_files.size() - 1);

	audio::playlist_order.clear();

	for (int i = 0; i < audio::playlist_files.size(); ++i)
	{
		int random_index = range(mt);

		while (std::count(audio::playlist_order.begin(), audio::playlist_order.end(), random_index))
		{
			random_index = range(mt);
		}

		audio::playlist_order.emplace_back(random_index);
	}
}

void audio::enumerate_playlist(const std::string& path)
{
	std::vector<std::string> files = fs::get_all_files(path, audio::supported_files);
	for (std::string file : files)
	{
		audio::playlist_files.emplace_back(file);
	}
}

void audio::play_next_song()
{
	audio::current_song_index++;

	//If our playlist has ended
	if (audio::current_song_index > audio::playlist_order.size() - 1)
	{
		audio::shuffle();
		audio::current_song_index = 0;
	}

	if (audio::playlist_order.size() > 0)
	{
		int next = audio::playlist_order[audio::current_song_index];

		if (next > audio::playlist_files.size() - 1)
		{
			next = audio::playlist_files.size() - 1;
		}

		audio::play_file(audio::playlist_files[next]);
	}
}

void audio::set_volume(std::int32_t vol_in)
{
	BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, vol_in * 100);
}

std::int32_t audio::req;
std::int32_t audio::chan;
std::int32_t audio::volume = 25;
playing_t audio::currently_playing = {"N/A"};
std::string audio::last_played_file = fs::get_pref_dir().append("last_played.txt");
std::string audio::playlist_dir = "./Music";
std::vector<std::string> audio::playlist_files;
std::vector<int> audio::playlist_order;
int audio::current_song_index = 0;
std::initializer_list<std::string> audio::supported_files { "wav", "mp1", "mp2", "mp3", "ogg", "aif"};