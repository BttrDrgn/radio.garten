#pragma once

#define API_URL "http://radio.garden"
#define VERSION 1
#define VERSION_HASH "\"2dda17c\""
#define PLACES_ENDPOINT "/api/ara/content/places"

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

};

class api
{
public:
	static void get_places();
	static void get_stations(std::string id);

	static nl::json places;
	static std::vector<place_t> place;
	static bool places_done;

	static std::vector<std::string> all_countries;

	static std::vector<station_t> all_stations;
};