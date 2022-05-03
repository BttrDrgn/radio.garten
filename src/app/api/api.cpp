#include "global.hpp"
#include "logger/logger.hpp"
#include "api.hpp"

void api::get_places()
{
	api::places_done = false;
	api::place.empty();
	api::all_countries.empty();

	std::thread([]
	{
		httplib::Client cli(API_URL);

		if (httplib::Result res = cli.Get(PLACES_ENDPOINT))
		{
			LOG_DEBUG("Accessing %s%s", API_URL, PLACES_ENDPOINT);

			if (res->status == 200)
			{
				api::places = nl::json::parse(res->body);

				//Check version matching for future updates
				std::int32_t version = api::places["apiVersion"].get<std::int32_t>();
				if (version != 1)
				{
					SDL_ShowSimpleMessageBox(0, "Radio.Garten", &logger::va("apiVersion was expected to be 1 (got %i)", version)[0], global::window);
					global::shutdown = true;
				}

				std::string version_hash = api::places["version"].dump();
				if (std::strcmp(VERSION_HASH, &version_hash[0]))
				{
					LOG_WARNING("version was expected to be %s (got %s)", VERSION_HASH, &version_hash[0]);
				}

				nl::json data = nl::json::parse(api::places["data"].dump());
				for (nl::detail::iteration_proxy_value<nl::detail::iter_impl<nl::json>>& i : data["list"].items())
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
					bool found = false;
					for (auto j : api::all_countries)
					{
						if (!std::strcmp(&j[0], &country[0]))
						{
							found = true;
							break;
						}
					}

					//If we don't add it
					if (!found)
					{
						api::all_countries.emplace_back(country);
					}

					//Add
					api::place.emplace_back(place_t{ country, city, id });
				}

				//Sort alphabetically
				std::sort(api::all_countries.begin(), api::all_countries.end());

				//Finish
				api::places_done = true;
			}
			else
			{
				LOG_ERROR("An error occured when gathering places data!");
			}
		}
	}).detach();
}

nl::json api::places;
std::vector<place_t> api::place;
bool api::places_done = true;

std::vector<std::string> api::all_countries;