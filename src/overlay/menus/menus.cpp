#include "global.hpp"
#include "logger/logger.hpp"
#include "fs/fs.hpp"
#include "menus.hpp"
#include "api/api.hpp"
#include "drpc/drpc.hpp"
#include "audio/audio.hpp"

void menus::update()
{
	ImGui::SetNextWindowPos({0, 0});
	ImGui::SetNextWindowSize(global::resolution);
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoBringToFrontOnFocus| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
	if(ImGui::Begin("Radio.Garten", nullptr, flags))
	{
		menus::main_menu_bar();
		ImGui::End();
	}
}

void menus::main_menu_bar()
{
	if (ImGui::BeginMainMenuBar())
	{
		menus::actions();
		menus::places();
		menus::stations();
		menus::favorites();
		ImGui::Text("Listening: %s on %s", &audio::currently_playing.title[0], &audio::currently_playing.station.title[0]);
		ImGui::EndMainMenuBar();
	}
}

void menus::actions()
{
	if (ImGui::BeginMenu("Actions"))
	{
		if (ImGui::Button("Refresh Places"))
		{
			memset(menus::search_buffer, 0, sizeof(menus::search_buffer));
			api::get_places();
		}

		ImGui::NewLine();

		if (ImGui::Button(&logger::va("Toggle Discord RPC [%s]", &logger::get_toggle(menus::show_drpc)[0])[0]))
		{
			menus::show_drpc = !menus::show_drpc;
			switch (menus::show_drpc)
			{
			case true:
				drpc::init();
				break;
			case false:
				drpc::deinit();
				break;
			}
		}

		ImGui::NewLine();

		if (ImGui::Button("Exit"))
		{
			global::shutdown = true;
		}
		ImGui::EndMenu();
	}
}

void menus::places()
{
	if (ImGui::BeginMenu("Places"))
	{
		if (std::strlen(menus::search_buffer) == 0)
		{
			menus::filtering = false;
		}

		if (ImGui::Button("Reset Filter"))
		{
			menus::filtering = false;
			memset(menus::search_buffer, 0, sizeof(menus::search_buffer));
		}

		if (ImGui::InputText("Search", menus::search_buffer, sizeof(search_buffer)))
		{
			menus::filtering = true;
			api::filter_place(std::string(menus::search_buffer));
		}

		if (ImGui::BeginMenu("Locations"))
		{
			std::string performance_text = logger::va("There are %i places, hidden by default for performance.\nClick the button to show all stations", api::places.size());
			ImVec2 size = { ImGui::CalcTextSize(&performance_text[0]).x, 500};

			if ((!menus::show_all_stations && !menus::filtering) || menus::filtering && api::filtered_places.empty())
			{
				size.y = 75;
			}

			if (ImGui::BeginChild(ImGui::GetID("locations_frame"), size))
			{
				if (api::places_done)
				{
					if (!menus::filtering)
					{
						if (api::places.empty())
						{
							ImGui::Text("Places are empty!\nYou might need to refresh\n\nPlease click Actions -> Refresh Places.");
						}
						else
						{
							if (!menus::show_all_stations)
							{
								ImGui::Text(&performance_text[0]);
								if (ImGui::Button("Show All"))
								{
									menus::show_all_stations = true;
								}
							}
							else if (menus::show_all_stations)
							{
								if (ImGui::Button("Hide All"))
								{
									menus::show_all_stations = false;
								}

								ImGui::NewLine();

								for (auto place : api::places)
								{
									if (ImGui::Button(&logger::va("[%s] %s", &place.country[0], &place.city[0])[0]))
									{
										api::get_details(place);
										ImGui::CloseCurrentPopup();
									}
								}
							}
						}
					}
					else if (menus::filtering)
					{
						if (api::filtered_places.empty())
						{
							ImGui::Text("No results found with the search term\n%s", menus::search_buffer);
							ImGui::Text("Tip: Search filters country, city, and place ID; case sensitive");
						}
						{
							for (auto place : api::filtered_places)
							{
								if (ImGui::Button(&logger::va("[%s] %s", &place.country[0], &place.city[0])[0]))
								{
									api::get_details(place);
									ImGui::CloseCurrentPopup();
								}
							}
						}
					}
				}
				ImGui::EndChild();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
}

void menus::stations()
{
	if (ImGui::BeginMenu("Stations"))
	{
		if (api::stations.empty())
		{
			ImGui::Text("Stations are empty!\nYou might need to select a place.");
		}
		else
		{
			for (station_t station : api::stations)
			{
				if (ImGui::Button(&logger::va("%s", &station.title[0])[0]))
				{
					audio::currently_playing.station.title = station.title;
					audio::currently_playing.station.id = station.id;
					audio::currently_playing.region.city = station.place.city;
					audio::currently_playing.region.country = station.place.country;
					audio::play(station.id);
				}

				ImGui::SameLine();

				if (ImGui::Button(&logger::va("*##%s", &station.id[0])[0]))
				{
					bool has = false;
					for (station_t favorite : api::favorite_stations)
					{
						if (favorite.id == station.id)
						{
							has = true;
							break;
						}
					}
					
					if (!has)
					{
						api::favorite_stations.emplace_back(station);
					}
				}
			}
		}
		ImGui::EndMenu();
	}
}

void menus::favorites()
{
	if (ImGui::BeginMenu("Favorites"))
	{
		if (api::favorite_stations.empty())
		{
			ImGui::Text("Favorites are empty!");
			ImGui::Text("Click the [*] button next to a station!");
		}
		else
		{
			for (station_t station : api::favorite_stations)
			{
				if (ImGui::Button(&logger::va("%s ##favorite", &station.title[0])[0]))
				{
					audio::currently_playing.station.title = station.title;
					audio::currently_playing.station.id = station.id;
					audio::currently_playing.region.city = station.place.city;
					audio::currently_playing.region.country = station.place.country;
					audio::play(station.id);
				}
			}
		}
		ImGui::EndMenu();
	}
}

void menus::build_font(ImGuiIO& io)
{
	std::string font = "fonts/NotoSans-Regular.ttf";
	std::string font_jp = "fonts/NotoSansJP-Regular.ttf";
	std::string emoji = "fonts/NotoEmoji-Regular.ttf";

	if (fs::exists(font))
	{
		io.Fonts->AddFontFromFileTTF(&font[0], 18.0f);

		static ImFontConfig cfg;
		static ImWchar emoji_ranges[] = { 0x1, 0x1FFFF, 0 };

		if (fs::exists(emoji))
		{
			cfg.MergeMode = true;
			cfg.OversampleH = cfg.OversampleV = 1;
			//cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;	//Noto doesnt have color
			io.Fonts->AddFontFromFileTTF(&emoji[0], 12.0f, &cfg, emoji_ranges);
		}

		if (fs::exists(font_jp))
		{
			ImFontConfig cfg;
			cfg.OversampleH = cfg.OversampleV = 1;
			cfg.MergeMode = true;
			io.Fonts->AddFontFromFileTTF(&font_jp[0], 18.0f, &cfg, io.Fonts->GetGlyphRangesJapanese());
		}
	}
}

bool menus::show_all_stations = false;
bool menus::show_drpc;

bool menus::filtering = false;
char menus::search_buffer[64];
