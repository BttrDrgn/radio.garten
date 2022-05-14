#include "logger/logger.hpp"
#include "global.hpp"

#include "audio.hpp"
#include "api/api.hpp"
#include "streamer.hpp"
#include "fs/fs.hpp"

#ifndef OVERLAY
void audio::init()
{
	if (HIWORD(BASS_GetVersion()) != BASSVERSION)
	{
		SDL_ShowSimpleMessageBox(0, "Radio.Garten BASS", "An incorrect version of BASS.DLL was loaded!", global::window);
		global::shutdown = true;
	}

#ifdef _WIN32
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(global::window, &wmInfo);
	
	if (!BASS_Init(-1, 44100, 0, wmInfo.info.win.window, 0))
	{
#else
	if (!BASS_Init(-1, 44100, 0, 0, 0))
	{
#endif
		SDL_ShowSimpleMessageBox(0, "Radio.Garten BASS", "Can't initialize device!\nNo audio will play for this session!", global::window);
	}

	BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST, 1); // enable playlist processing
	audio::set_volume(audio::volume);
}
#else
void audio::init_overlay(HWND hwnd)
{
	if (global::hwnd != hwnd)
	{
		logger::log("AUDIO_ERR", "Global hwnd is not the same!");
		return;
	}

	if (HIWORD(BASS_GetVersion()) != BASSVERSION)
	{
		global::msg_box("Radio.Garten BASS", "An incorrect version of BASS.DLL was loaded!");
		global::shutdown = true;
	}

	if (!BASS_Init(-1, 44100, 0, global::hwnd, 0))
	{
		global::msg_box("Radio.Garten BASS", "Can't initialize device!\nNo audio will play for this session!");
	}

	BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST, 1); // enable playlist processing

	if (fs::exists(audio::last_played_file))
	{
		std::vector<std::string> contents = logger::split(fs::read(audio::last_played_file), ",");
		audio::currently_playing.station.title = contents[0];
		audio::play(contents[1]);
	}
}
#endif

void audio::play(const std::string& url)
{
	std::thread([url]
	{
		std::string final_url = logger::va("%s%s", HOST_NAME, AUDIO_ENDPOINT(&url[0]));
		audio::currently_playing.title = "N/A";
		audio::currently_playing.url = url;
		audio::paused = false;
		open_url(&final_url[0]);
	}).detach();
}

void audio::stop(bool hard)
{
	BASS_StreamFree(audio::chan);
	audio::chan = 0;

	if (hard)
	{
		audio::currently_playing = { "N/A", "N/A" };
	}
	else
	{
		audio::currently_playing.title = "N/A";
	}
}

void audio::set_volume(std::int32_t vol_in)
{
	BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, vol_in * 100);
}

std::int32_t audio::req;
std::int32_t audio::chan;
bool audio::paused = false;
std::int32_t audio::volume = 100;
playing_t audio::currently_playing = {"N/A", "N/A"};
std::string audio::last_played_file = fs::get_pref_dir().append("last_played.txt");