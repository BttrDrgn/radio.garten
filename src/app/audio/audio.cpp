#include "logger/logger.hpp"
#include "global.hpp"

#include "audio.hpp"
#include "player.hpp"
#include "fs/fs.hpp"
#include "hook/hook.hpp"
#include "defs.hpp"

void audio::init()
{
	switch (global::game)
	{
	case game_t::NFSU2:
		//WIP filter detection
		audio::mute_detection.emplace_back("LS_PSAMovie.fng");
		audio::mute_detection.emplace_back("LS_THXMovie.fng");
		audio::mute_detection.emplace_back("LS_EAlogo.fng");
		audio::mute_detection.emplace_back("LS_BlankMovie.fng");
		audio::mute_detection.emplace_back("UG_LS_IntroFMV.fng");
		break;
	}

	if (HIWORD(BASS_GetVersion()) != BASSVERSION)
	{
		global::msg_box("ECM BASS", "An incorrect version of BASS.DLL was loaded!");
		global::shutdown = true;
	}

	if (!BASS_Init(-1, 44100, 0, global::hwnd, 0))
	{
		global::msg_box("ECM BASS", "Can't initialize device!\nNo audio will play for this session!");
	}

	audio::set_volume(audio::volume);
	audio::shuffle();
	audio::pause();
	audio::update();
}

void audio::update()
{
	std::thread([] {
		while (true)
		{
			global::state = game_state;

			std::uint32_t state = BASS_ChannelIsActive(audio::chan[0]);

			switch (state)
			{
			case BASS_ACTIVE_STOPPED:
				audio::playing = false;
				break;
			}

			if (!audio::paused && !audio::playing)
			{
				audio::play_next_song();
			}

			//Setup for channel crossfading
			switch (global::state)
			{
				//Frontend
			case GameFlowState::LoadingFrontend:
			case GameFlowState::InFrontend:
				if(audio::playlist_files[audio::playlist_order[audio::current_song_index]].second == "IG")
				{
					audio::stop(0);
					audio::play_next_song();
				}
				break;

				//In-game
			case GameFlowState::UnloadingFrontend:
			case GameFlowState::Racing:
				if (audio::playlist_files[audio::playlist_order[audio::current_song_index]].second == "FE")
				{
					audio::stop(0);
					audio::play_next_song();
				}
				break;
			}

			std::this_thread::sleep_for(128ms);
		}
	}).detach();
}

void audio::play_file(const std::string& file, int channel)
{
	std::string title = file;
	logger::rem_path_info(title, audio::playlist_dir);
	audio::currently_playing.title = title;
	::play_file(file.c_str(), channel);
}

void audio::stop(int channel)
{
	audio::paused = false;

	BASS_StreamFree(audio::chan[channel]);
	audio::chan[channel] = 0;

	audio::currently_playing.title = "N/A";
}

void audio::shuffle()
{
	if (audio::playlist_files.size() > 0)
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
}

void audio::play()
{
	audio::paused = false;
	BASS_Start();
}

void audio::pause()
{
	audio::paused = true;
	BASS_Pause();
}

void audio::enumerate_playlist()
{
	std::vector<std::string> files = fs::get_all_files(audio::playlist_dir, audio::supported_files);
	for (std::string file : files)
	{
		audio::playlist_files.emplace_back(file, "N/A");
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

		switch (global::state)
		{
			//Frontend
		case GameFlowState::LoadingFrontend:
		case GameFlowState::InFrontend:
			while(audio::playlist_files[next].second == "IG")
			{
				audio::current_song_index++;

				if (audio::current_song_index > audio::playlist_order.size() - 1)
				{
					audio::shuffle();
					audio::current_song_index = 0;
				}

				next = audio::playlist_order[audio::current_song_index];
			}

			audio::play_file(audio::playlist_files[next].first, 0);
			break;

			//In-game
		case GameFlowState::LoadingTrack:
		case GameFlowState::LoadingRegion:
		case GameFlowState::Racing:
			while (audio::playlist_files[next].second == "FE")
			{
				audio::current_song_index++;

				if (audio::current_song_index > audio::playlist_order.size() - 1)
				{
					audio::shuffle();
					audio::current_song_index = 0;
				}

				next = audio::playlist_order[audio::current_song_index];
			}

			audio::play_file(audio::playlist_files[next].first, 0);
			break;
		}
	}
}

void audio::set_volume(std::int32_t vol_in)
{
	BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, vol_in * 100);
}

bool audio::paused = false;
bool audio::playing = false;
std::int32_t audio::req;
std::int32_t audio::chan[2];
std::int32_t audio::volume = 25;
playing_t audio::currently_playing = {"N/A"};
std::string audio::playlist_name = "Music";
std::string audio::playlist_dir = "Music";
std::vector<std::pair<std::string, std::string>> audio::playlist_files;
std::vector<int> audio::playlist_order;
int audio::current_song_index = 0;
std::initializer_list<std::string> audio::supported_files { "wav", "mp1", "mp2", "mp3", "ogg", "aif"};
std::vector<const char*> audio::mute_detection;
