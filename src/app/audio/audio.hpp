#pragma once

#include "api/api.hpp"

#define BASS_SYNC_HLS_SEGMENT	0x10300
#define BASS_TAG_HLS_EXTINF		0x14000

struct playing_t
{
	std::string title;
	station_t station;
	place_t region;
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

	static std::int32_t req;
	static std::int32_t chan;

	static playing_t currently_playing;
};