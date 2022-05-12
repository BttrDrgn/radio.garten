#pragma once

#include "api/api.hpp"

#define BASS_SYNC_HLS_SEGMENT	0x10300
#define BASS_TAG_HLS_EXTINF		0x14000

struct playing_t
{
	std::string title;
	station_t station;
	place_t region;
	std::string url;
};

class audio
{
public:
#ifdef OVERLAY
	static void init_overlay(HWND hwnd);
#else
	static void init();
#endif
	static void play(const std::string& url);
	static void stop(bool hard = false);
	static void set_volume(std::int32_t vol_in);

	static std::int32_t req;
	static std::int32_t chan;

	static std::string last_played_file;

	static bool paused;
	static std::int32_t volume;

	static playing_t currently_playing;
};