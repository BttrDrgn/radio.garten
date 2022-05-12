#pragma once

#define API_URL "http://radio.garden"
#define VERSION 1
#define VERSION_HASH "\"bd23e54\""	//radio.garden git commit hash
#define PLACES_ENDPOINT "/api/ara/content/places"
#define PLACE_DETAIL_ENDPOINT(...) &logger::va("/api/ara/content/page/%s/channels", __VA_ARGS__)[0]
#define STATION_ENDPOINT(...) &logger::va("/api/ara/content/channel/%s", __VA_ARGS__)[0]
#define AUDIO_ENDPOINT(...) &logger::va("/api/ara/content/listen/%s/channel.mp3", __VA_ARGS__)[0]

struct place_t
{
	std::string country, city, id;
};

struct station_t
{
	std::string title, id;
	place_t place;
};

class api
{
public:
	static void get_places();
	static void filter_place(const std::string& key);
	static void get_details(const place_t& place_in);
	static void get_station(const std::string& id);
	static std::string get_final_redirect(const std::string& entry_url);

	static nl::json places_json;
	static std::vector<place_t> places;
	static std::vector<place_t> filtered_places;
	static bool places_done;

	static nl::json details_json;
	static bool detail_done;

	static nl::json stations_json;
	static std::vector<station_t> stations;
	static bool stations_done;
	static std::vector<station_t> favorite_stations;

	static std::string current_place_id;

	static std::vector<std::string> all_countries;
};