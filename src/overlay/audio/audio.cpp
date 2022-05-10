#include "logger/logger.hpp"
#include "global.hpp"

#include "audio.hpp"
#include "streamer.hpp"

void audio::init(HWND hwnd)
{
	global::hwnd = hwnd;

	api::get_places();

	if (HIWORD(BASS_GetVersion()) != BASSVERSION)
	{
		MessageBoxA(global::hwnd, "An incorrect version of BASS.DLL was loaded!", "Radio.Garten BASS", 0);
		global::shutdown = true;
	}

	if (!BASS_Init(-1, 44100, 0, global::hwnd, 0))
	{
		MessageBoxA(global::hwnd, "Can't initialize device!\nNo audio will play for this session!", "Radio.Garten BASS", 0);
	}

	BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST, 1); // enable playlist processing
}

void audio::play(const std::string& url)
{
	std::string final_url = logger::va("%s%s", API_URL, AUDIO_ENDPOINT(&url[0]));
	audio::currently_playing.title = "N/A";
	open_url(&final_url[0]);
}

std::int32_t audio::req;
std::int32_t audio::chan;
playing_t audio::currently_playing = {"N/A", "N/A"};