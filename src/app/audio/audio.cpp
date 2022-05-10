#include "logger/logger.hpp"
#include "global.hpp"

#include "audio.hpp"
#include "api/api.hpp"
#include "streamer.hpp"

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
}
#else
void audio::init_overlay(HWND hwnd)
{
	global::hwnd = hwnd;
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
}
#endif

void audio::play(const std::string& url)
{
	std::thread([url]
	{
		std::string final_url = logger::va("%s%s", API_URL, AUDIO_ENDPOINT(&url[0]));
		audio::currently_playing.title = "N/A";
		open_url(&final_url[0]);
	}).detach();
}

std::int32_t audio::req;
std::int32_t audio::chan;
playing_t audio::currently_playing = {"N/A", "N/A"};