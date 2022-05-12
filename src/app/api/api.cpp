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

	api::places_json = {};
	api::details_json = {};
	api::places_done = false;
	api::places = {};
	api::all_countries = {};
	api::stations = {};
	api::stations = {};

	std::thread([]
	{
		httplib::Client cli(API_URL);

		if (httplib::Result res = cli.Get(PLACES_ENDPOINT))
		{
			logger::log_info(logger::va("Accessing %s%s", API_URL, PLACES_ENDPOINT));

			if (res->status == 200)
			{
				api::places_json = nl::json::parse(res->body);

				//Check version matching for future updates
				std::int32_t version = api::places_json["apiVersion"].get<std::int32_t>();
				if (version != 1)
				{
					global::msg_box("Radio.Garten Places", logger::va("apiVersion was expected to be 1 (got %i)", version));
					global::shutdown = true;
				}

				std::string version_hash = api::places_json["version"].dump();
				if (version_hash.compare(VERSION_HASH))
				{
					logger::log_warning(logger::va("Hash was expected to be %s (got %s)", VERSION_HASH, &version_hash[0]));
				}

				nl::json data = nl::json::parse(api::places_json["data"].dump());
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
				api::places_json = {};
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

	api::details_json = {};
	api::detail_done = false;
	api::stations = {};

	std::thread([place_in]
	{
		httplib::Client cli(API_URL);

		if (httplib::Result res = cli.Get(PLACE_DETAIL_ENDPOINT(&place_in.id[0])))
		{
			logger::log_info(logger::va("Accessing %s%s", API_URL, PLACE_DETAIL_ENDPOINT(&place_in.id[0])));
			if (res->status == 200)
			{
				api::details_json = nl::json::parse(res->body);

				//Check version matching for future updates
				std::int32_t version = api::details_json["apiVersion"].get<std::int32_t>();
				if (version != 1)
				{
					global::msg_box("Radio.Garten Places", logger::va("apiVersion was expected to be 1 (got %i)", version));
					global::shutdown = true;
				}

				std::string version_hash = api::details_json["version"].dump();
				if (version_hash.compare(VERSION_HASH))
				{
					logger::log_warning(logger::va("Hash was expected to be %s (got %s)", VERSION_HASH, &version_hash[0]));
				}

				nl::json data = nl::json::parse(api::details_json["data"].dump());

				for (const nl::basic_json<>::value_type& i : data["content"][0]["items"])
				{
					std::string id = i["href"].dump();
					std::string title = i["title"].dump();

					id.erase(std::remove(id.begin(), id.end(), '\"'), id.end());
					title.erase(std::remove(title.begin(), title.end(), '\"'), title.end());

					if (!id.compare("null"))
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
				api::details_json = {};
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
					api::stations_json = nl::json::parse(res->body);

					//Check version matching for future updates
					std::int32_t version = api::stations_json["apiVersion"].get<std::int32_t>();
					if (version != 1)
					{
						global::msg_box("Radio.Garten Places", logger::va("apiVersion was expected to be 1 (got %i)", version));
						global::shutdown = true;
					}

					std::string version_hash = api::stations_json["version"].dump();
					if (version_hash.compare(VERSION_HASH))
					{
						logger::log_warning(logger::va("Hash was expected to be %s (got %s)", VERSION_HASH, &version_hash[0]));
					}

					//Finish
					api::stations_done = true;
					api::stations_json = {};
				}
				else
				{
					logger::log_error("An error occured when gathering station data!");
				}
			}
		}).detach();
}

void api::filter_place(const std::string& key)
{
	api::filtered_places = {};

	for (const place_t& place : api::places)
	{
		//Look for city, country, or id with this string
		if (place.city.find(key) != std::string::npos || place.country.find(key) != std::string::npos || place.id.find(key) != std::string::npos)
		{
			api::filtered_places.emplace_back(place);
		}
	}
}

std::string api::get_final_redirect(const std::string& entry_url)
{
	std::string domain = entry_url.substr(0, strlen("http://radio.garden"));
	std::string access = entry_url.substr(strlen("http://radio.garden"), entry_url.length() - strlen("http://radio.garden"));
	
	httplib::Client cli(&domain[0]);/*  */

	if(httplib::Result res = cli.Get(&access[0]))
	{
		logger::log_info(logger::va("Accessing %s%s", domain, access));

		cli.set_follow_location(true);
		if(res = cli.Get("/"))
		{
			logger::log_debug(logger::va("Status %i", res->status));
		}
	}

	return std::string("");
}

nl::json api::places_json;
std::vector<place_t> api::places;
std::vector<place_t> api::filtered_places;
bool api::places_done = true;

nl::json api::details_json;
bool api::detail_done = true;

nl::json api::stations_json;
std::vector<station_t> api::stations;
bool api::stations_done = true;

std::vector<station_t> api::favorite_stations;

std::string api::current_place_id = "N/A";

std::vector<std::string> api::all_countries;