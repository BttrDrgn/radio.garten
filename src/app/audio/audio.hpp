#pragma once

#define BASS_SYNC_HLS_SEGMENT	0x10300
#define BASS_TAG_HLS_EXTINF		0x14000

class audio
{
public:
	static void init();
	static void play(const std::string& url);

	static std::int32_t req;
	static std::int32_t chan;
};