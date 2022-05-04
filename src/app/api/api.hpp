#pragma once

#define API_URL "http://radio.garden"
#define VERSION 1
#define VERSION_HASH "\"2dda17c\""
#define PLACES_ENDPOINT "/api/ara/content/places"
#define PLACE_DETAIL_ENDPOINT(...) &logger::va("/api/ara/content/page/%s/channels", __VA_ARGS__)[0]
#define STATION_ENDPOINT(...) &logger::va("/api/ara/content/channel/%s", __VA_ARGS__)[0]
#define AUDIO_ENDPOINT(...) &logger::va("/api/ara/content/listen/%s/channel.mp3", __VA_ARGS__)[0]

struct place_t
{
	std::string country, city, id;
};

struct city_t
{
	std::string country, city;
};

struct station_t
{
	std::string title, id;
};

class api
{
public:
	static void get_places();
	static void get_details(const std::string& id);
	static void get_station(const std::string& id);

	static nl::json places;
	static std::vector<place_t> place;
	static bool places_done;

	static nl::json details;
	static bool detail_done;

	static nl::json stations;
	static std::vector<station_t> station;
	static bool stations_done;

	static std::string current_place_id;

	static std::vector<std::string> all_countries;
};