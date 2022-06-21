#pragma once

#define BASS_SYNC_HLS_SEGMENT	0x10300
#define BASS_TAG_HLS_EXTINF		0x14000

using namespace std::literals;

struct playing_t
{
	std::string title, where;
};

class audio
{
public:
	static void init();
	static void play_file(const std::string& file, int channel);
	static void stop(int channel);
	static void set_volume(std::int32_t vol_in);
	static void enumerate_playlist();
	static void update();
	static void shuffle();
	static void play_next_song();
	static void pause();
	static void play();

	static std::string playlist_name;
	static std::string playlist_dir;
	static std::vector<std::pair<std::string, std::string>> playlist_files;
	static std::vector<int> playlist_order;
	static int current_song_index;

	static std::vector<const char*> mute_detection;

	static std::initializer_list<std::string> supported_files;

	static bool paused;
	static bool playing;
	static std::int32_t req;
	static std::int32_t chan[2];

	static std::int32_t volume;

	static playing_t currently_playing;
};
