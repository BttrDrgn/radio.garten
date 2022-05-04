#include "logger/logger.hpp"
#include "global.hpp"

#include "audio.hpp"
#include "api/api.hpp"
#include "streamer.hpp"

void audio::init()
{
	if (HIWORD(BASS_GetVersion()) != BASSVERSION)
	{
		SDL_ShowSimpleMessageBox(0, "Radio.Garten BASS", "An incorrect version of BASS.DLL was loaded!", global::window);
		global::shutdown = true;
	}

	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(global::window, &wmInfo);
	if (!BASS_Init(-1, 44100, 0, wmInfo.info.win.window, NULL))
	{
		SDL_ShowSimpleMessageBox(0, "Radio.Garten BASS", "Can't initialize device", global::window);
		global::shutdown = true;
	}

	BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST, 1); // enable playlist processing
}

void audio::play(const std::string& url)
{
	std::string final_url = logger::va("%s%s", API_URL, AUDIO_ENDPOINT(&url[0]));
	LOG_DEBUG("Playing: %s", &final_url[0]);
	open_url(&final_url[0]);
}

std::int32_t audio::req;
std::int32_t audio::chan;