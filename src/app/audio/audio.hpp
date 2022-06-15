#pragma once

#define BASS_SYNC_HLS_SEGMENT	0x10300
#define BASS_TAG_HLS_EXTINF		0x14000

using namespace std::literals;

struct playing_t
{
	std::string title;
};

class audio
{
public:
	static void init_overlay();
	static void play_file(const std::string& file);
	static void stop();
	static void set_volume(std::int32_t vol_in);
	static void enumerate_playlist(const std::string& path);
	static void update();
	static void shuffle();
	static void play_next_song();

	static std::string playlist_dir;
	static std::vector<std::string> playlist_files;
	static std::vector<int> playlist_order;
	static int current_song_index;

	static std::initializer_list<std::string> supported_files;

	static std::int32_t req;
	static std::int32_t chan;

	static std::int32_t volume;

	static playing_t currently_playing;
};