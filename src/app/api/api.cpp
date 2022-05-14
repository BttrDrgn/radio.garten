#include "global.hpp"
#include "logger/logger.hpp"
#include "api.hpp"

void api::get_places()
{
	if (!api::places_done)
	{
		logger::log_debug("Refreshing places not done! Cancelling request.");
		return;
	}

	api::places_done = false;
	api::places.clear();
	api::all_countries.clear();
	api::stations.clear();

	std::thread([]
	{
		httplib::Client cli(API_URL);

		if (httplib::Result res = cli.Get(PLACES_ENDPOINT))
		{
			logger::log_info(logger::va("Accessing %s%s", API_URL, PLACES_ENDPOINT));

			if (res->status == 200)
			{
				nl::json json = nl::json::parse(res->body);

				api::version_check(json);

				nl::json data = nl::json::parse(json["data"].dump());
				for (const nl::detail::iteration_proxy_value<nl::detail::iter_impl<nl::json>>& i : data["list"].items())
				{
					//Get string
					std::string id = i.value()["id"].dump();
					std::string country = i.value()["country"].dump();
					std::string city = i.value()["title"].dump();

					//Rem ""
					id.erase(std::remove(id.begin(), id.end(), '\"'), id.end());
					country.erase(std::remove(country.begin(), country.end(), '\"'), country.end());
					city.erase(std::remove(city.begin(), city.end(), '\"'), city.end());

					//Check if we already have this country
					for (const std::string& j : api::all_countries)
					{
						if (!j.compare(country))
						{
							api::all_countries.emplace_back(country);
							break;
						}
					}

					//Add
					api::places.emplace_back(place_t{ country, city, id });
				}

				//Sort alphabetically
				std::sort(api::all_countries.begin(), api::all_countries.end());
				std::sort(api::places.begin(), api::places.end(), ([](const place_t& l, const place_t& r)
				{
					if (l.country < r.country) return true;
					if (r.country < l.country) return false;

					if (l.city < r.city) return true;
					if (r.city < l.city) return false;
				}));

				//Finish
				api::places_done = true;
				json.clear();
			}
			else
			{
				logger::log_error("An error occured when gathering places data!");
			}
		}
	}).detach();
}

void api::get_details(const place_t& place_in)
{
	if (!api::stations_done)
	{
		logger::log_debug("Refreshing stations not done! Cancelling request.");
		return;
	}
	
	api::detail_done = false;
	api::stations.clear();

	std::thread([place_in]
	{
		httplib::Client cli(API_URL);

		if (httplib::Result res = cli.Get(PLACE_DETAIL_ENDPOINT(&place_in.id[0])))
		{
			logger::log_info(logger::va("Accessing %s%s", API_URL, PLACE_DETAIL_ENDPOINT(&place_in.id[0])));
			if (res->status == 200)
			{
				nl::json json = nl::json::parse(res->body);

				api::version_check(json);

				nl::json data = nl::json::parse(json["data"].dump());

				for (const nl::basic_json<>::value_type& i : data["content"][0]["items"])
				{
					std::string id = i["href"].dump();
					std::string title = i["title"].dump();

					id.erase(std::remove(id.begin(), id.end(), '\"'), id.end());
					title.erase(std::remove(title.begin(), title.end(), '\"'), title.end());

					if (id.compare("null"))
					{
						api::stations.emplace_back(station_t{ title, id.substr(id.size() - 8), place_in });
					}

				}

				std::sort(api::stations.begin(), api::stations.end(), ([](const station_t& l, const station_t& r)
				{
					return (l.title < r.title);
				}));

				//Finish
				api::detail_done = true;
				json.clear();
			}
			else
			{
				logger::log_error("An error occured when gathering station data!");
			}
		}
	}).detach();
}

void api::get_station(const std::string& id)
{
	api::stations_done = false;

	std::thread([id]
		{
			httplib::Client cli(API_URL);

			if (httplib::Result res = cli.Get(STATION_ENDPOINT(&id[0])))
			{
				logger::log_info(logger::va("Accessing %s%s", API_URL, STATION_ENDPOINT(&id[0])));
				if (res->status == 200)
				{
					nl::json json = nl::json::parse(res->body);

					api::version_check(json);

					//Finish
					api::stations_done = true;
					json.clear();
				}
				else
				{
					logger::log_error("An error occured when gathering station data!");
				}
			}
		}).detach();
}

void api::search_stations(const std::string& str)
{
	if (!api::station_search_done)
	{
		logger::log_debug("Searching stations not done! Cancelling request.");
		return;
	}

	api::station_search_done = false;

	std::thread([str]
	{
		httplib::Client cli(API_URL);

		if (httplib::Result res = cli.Get(SEARCH_ENDPOINT(&str[0])))
		{
			logger::log_info(logger::va("Accessing %s%s", API_URL, SEARCH_ENDPOINT(&str[0])));
			if (res->status == 200)
			{
				nl::json json = nl::json::parse(res->body);

				//Check version matching for future updates
				
				api::version_check(json);

				nl::json data = nl::json::parse(json["hits"].dump());

				for (const nl::basic_json<>::value_type& i : data["hits"])
				{
					logger::log_debug(i["hits"].dump());
				}

				//Finish
				api::station_search_done = true;
				json.clear();
			}
			else
			{
				logger::log_error("An error occured when searching for stations!");
			}
		}
	}).detach();
}

void api::filter_place(const std::string& key)
{
	api::filtered_places.clear();

	for (const place_t& place : api::places)
	{
		//Look for city, country, or id with this string
		if (place.city.find(key) != std::string::npos || place.country.find(key) != std::string::npos || place.id.find(key) != std::string::npos)
		{
			api::filtered_places.emplace_back(place);
		}
	}
}

bool api::version_check(const nl::json& in_json)
{
	std::int32_t version = in_json["apiVersion"].get<std::int32_t>();
	if (version != 1)
	{
		global::msg_box("Radio.Garten Places", logger::va("apiVersion was expected to be 1 (got %i)", version));
		global::shutdown = true;
		return false;
	}

	std::string version_hash = in_json["version"].dump();
	if (version_hash.compare(VERSION_HASH))
	{
		logger::log_warning(logger::va("Hash was expected to be %s (got %s)", VERSION_HASH, &version_hash[0]));
	}

	return true;
}

std::vector<place_t> api::places;
std::vector<place_t> api::filtered_places;
bool api::places_done = true;

bool api::detail_done = true;

std::vector<station_t> api::stations;
bool api::stations_done = true;

std::vector<station_t> api::station_search_results;
bool api::station_search_done = true;

std::vector<station_t> api::favorite_stations;

std::string api::current_place_id = "N/A";

std::vector<std::string> api::all_countries;