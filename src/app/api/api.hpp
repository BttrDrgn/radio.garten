#pragma once

#define API_URL "http://radio.garden"
#define VERSION 1
#define VERSION_HASH "\"2dda17c\""
#define PLACES_ENDPOINT "/api/ara/content/places"
#define PLACE_DETAIL_ENDPOINT "/api/ara/content/page"
#define STATION_ENDPOINT "/api/ara/content/channel"

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
	static void get_details(std::string id);
	static void get_station(std::string id);

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